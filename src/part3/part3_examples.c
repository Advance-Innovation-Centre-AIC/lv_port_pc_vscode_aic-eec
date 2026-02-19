/*******************************************************************************
 * File Name:   part3_examples.c
 *
 * Description: Part 3 Examples - Oscilloscope & Signal Processing
 *              Embedded C for IoT Course
 *
 * Author:      Assoc. Prof. Wiroon Sriborrirux
 *              Embedded Systems Engineering, Faculty of Engineering,
 *              Burapha University
 *
 * Part 3 Examples:
 *   1. Waveform Generator      - Software waveform generation (Sine/Square/Tri)
 *   2. Noise Generator         - LFSR pseudo-random noise
 *   3. Audio Waveform Display  - Audio visualization (simulated)
 *   4. Microphone Visualizer   - Audio input with level meter
 *   5. Full Oscilloscope UI    - Complete scope with measurements
 *   6. Spectrum Analyzer       - FFT-based spectrum display
 *
 * =============================================================================
 * SCREEN & COORDINATE SYSTEM (PSoC Edge E84 Eval Kit)
 * =============================================================================
 *       X=0           X=240          X=479
 *   Y=0  +-----------------------------+
 *        | (0,0)                       |
 *        |         X+ -------->        |
 *   Y=136|         | Y+    * CENTER    | (Note: Height 272 for Part 3)
 *        |         v     (240,136)     |
 *   Y=271+-----------------------------+
 *
 *   Display: 480 x 272 pixels (SCREEN_HEIGHT defined as 272)
 *
 * =============================================================================
 * COMMON LAYOUT PATTERNS IN WEEK 5
 * =============================================================================
 *
 *   Ex1: Waveform Generator
 *   +------------------------------------------+
 *   |        "Waveform Generator"              | TOP_MID, y=10, color=#00ff88
 *   | +--------------------------------------+ |
 *   | |       [CHART 400x180]                | | CENTER, y=-10
 *   | |  Line chart, 200 points              | | Background: #001100
 *   | |  Green line on dark green grid       | |
 *   | +--------------------------------------+ |
 *   | [Wave Type v]     [===Freq Slider===]    | BOTTOM_LEFT/MID, y=-40
 *   |              "Freq: 1000 Hz"             | BOTTOM_MID, y=-65
 *   |                    "Waveform: Sine"      | BOTTOM_RIGHT, y=-45
 *   +------------------------------------------+
 *
 *   Ex5: Full Oscilloscope UI
 *   +------------------------------+----------+
 *   |    "Digital Oscilloscope"    |          | TOP_MID, y=5
 *   | +------------------------+   | Controls | Chart: 360x160
 *   | |    [CHART 360x160]     |   | 100x160  | TOP_LEFT, x=10, y=35
 *   | |  8 div Y, 10 div X     |   |[Dropdown]|
 *   | |  Green on black grid   |   |[Slider]  |
 *   | +------------------------+   |"1000 Hz" |
 *   |                              |"1 ms/div"|
 *   |                              |"1 V/div" |
 *   +------------------------------+----------+
 *   | [=====Measurements Panel (full width)===]| BOTTOM_MID, y=-25
 *   | Vpp: --    Freq: -- Hz    RMS: --        | Flex row, 50px height
 *   +------------------------------------------+
 *
 *   Ex6: Spectrum Analyzer
 *   +------------------------------------------+
 *   |       "FFT Spectrum Analyzer"            | TOP_MID, y=10, color=#ff6600
 *   | [Wave v]          "Dominant: -- Hz"      | TOP_LEFT/RIGHT, y=35/40
 *   | +--------------------------------------+ |
 *   | |       [BAR CHART 400x180]            | | CENTER, y=0
 *   | |  FFT_BINS (128) bars                 | | Type: LV_CHART_TYPE_BAR
 *   | |  Gradient green bars                 | |
 *   | +--------------------------------------+ |
 *   |  "0 Hz"                     "24000 Hz"   | OUT_BOTTOM_LEFT/RIGHT of chart
 *   +------------------------------------------+
 *
 * =============================================================================
 * CHART CONFIGURATION GUIDELINES FOR OSCILLOSCOPE
 * =============================================================================
 *   - Point count: 200 (CHART_POINTS) for smooth waveforms
 *   - Chart size: 400x180 typical, 360x160 for scope with control panel
 *   - Y-range: 0-100 (scaled from int16 signal data)
 *   - Colors: Green (#00ff00) on black/dark green (#001100/#003300)
 *   - Hide points (size=0) for clean line display
 *
 * =============================================================================
 * TIMER UPDATE RATES
 * =============================================================================
 *   - Waveform display: 100ms (10 Hz)
 *   - Noise display: 50ms (20 Hz) for more dynamic feel
 *   - Audio display: 33ms (30 Hz) for smooth animation
 *   - Oscilloscope: 50ms (20 Hz) with measurements
 *   - Spectrum: 100ms (10 Hz) - FFT is computationally intensive
 *
 * =============================================================================
 * OSCILLOSCOPE API (aic-eec/scope.h)
 * =============================================================================
 *   Waveform Generation:
 *     aic_scope_init()                    - Initialize scope subsystem
 *     aic_scope_generate_wave(buf, n, cfg) - Generate waveform samples
 *     aic_scope_generate_noise(buf, n, amp) - Generate LFSR noise
 *     aic_scope_wave_name(type)           - Get wave type name string
 *
 *   Audio Input:
 *     aic_audio_in_init(sample_rate)      - Initialize audio input
 *     aic_audio_in_start()                - Start audio capture
 *     aic_audio_in_get_samples(buf, n)    - Get audio samples
 *     aic_audio_in_get_level()            - Get audio level (0-100)
 *     aic_audio_in_set_simulated(type, f) - Set simulated audio source
 *
 *   FFT:
 *     aic_fft_init(size)                  - Initialize FFT engine
 *     aic_fft_calculate(in, out)          - Calculate FFT spectrum
 *     aic_fft_dominant_frequency(...)     - Find dominant frequency
 *
 *   Signal Analysis:
 *     aic_signal_peak_to_peak(buf, n)     - Calculate Vpp
 *     aic_signal_frequency(buf, n, sr)    - Measure signal frequency
 *     aic_signal_rms(buf, n)              - Calculate RMS value
 *
 *   Scale Strings:
 *     aic_scope_time_div_str(idx)         - Get time/div string
 *     aic_scope_volt_div_str(idx)         - Get volt/div string
 *
 ******************************************************************************/

#include "part3_examples.h"
#include "../aic-eec/aic-eec.h"
#include "../aic-eec/scope.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * Configuration
 ******************************************************************************/

/* Display size (adjust for your screen) */
#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   272

/* Chart configuration */
#define CHART_POINTS    200
#define CHART_WIDTH     400
#define CHART_HEIGHT    180

/* Sample rates */
#define DISPLAY_SAMPLE_RATE     48000
#define DEFAULT_FREQUENCY       1000

/* FFT configuration */
#define FFT_SIZE        256
#define FFT_BINS        (FFT_SIZE / 2)
#define FFT_CHART_BINS  64      /* Displayed bins (decimated for better visibility) */

/*******************************************************************************
 * Static Variables
 ******************************************************************************/

/* Waveform data buffer */
static int16_t waveform_buffer[CHART_POINTS];

/* FFT output buffer */
static uint16_t fft_output[FFT_BINS];

/* Current settings */
static aic_wave_type_t current_wave_type = AIC_WAVE_SINE;
static uint32_t current_frequency = DEFAULT_FREQUENCY;
static int16_t current_amplitude = 16000;

/* UI element references */
static lv_obj_t *main_chart = NULL;
static lv_chart_series_t *chart_series = NULL;
static lv_obj_t *freq_label = NULL;
static lv_obj_t *info_label = NULL;

/* Timer for updates */
static lv_timer_t *update_timer = NULL;

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

static void update_chart_data(void)
{
    if (main_chart == NULL || chart_series == NULL) {
        return;
    }

    /* Generate waveform data */
    aic_wavegen_config_t config = {
        .type = current_wave_type,
        .frequency_hz = current_frequency,
        .sample_rate_hz = DISPLAY_SAMPLE_RATE,
        .amplitude = current_amplitude,
        .dc_offset = 0,
        .duty_percent = 50
    };

    aic_scope_generate_wave(waveform_buffer, CHART_POINTS, &config);

    /* Update chart */
    for (int i = 0; i < CHART_POINTS; i++) {
        /* Scale from int16 to chart range (0-100 centered at 50) */
        int32_t scaled = 50 + (waveform_buffer[i] * 40) / 32767;
        lv_chart_set_value_by_id(main_chart, chart_series, i, scaled);
    }

    lv_chart_refresh(main_chart);
}

/*******************************************************************************
 * Example 1: Waveform Generator
 ******************************************************************************/

static void wave_type_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t *dropdown = lv_event_get_target(e);
    current_wave_type = (aic_wave_type_t)lv_dropdown_get_selected(dropdown);

    if (info_label != NULL) {
        lv_label_set_text_fmt(info_label, "Waveform: %s",
                              aic_scope_wave_name(current_wave_type));
    }

    update_chart_data();
}

static void freq_slider_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);

    /* Map slider (0-100) to frequency (100-10000 Hz) logarithmically */
    current_frequency = 100 + (value * value);  /* Quadratic mapping */

    if (freq_label != NULL) {
        lv_label_set_text_fmt(freq_label, "Freq: %u Hz",
                              (unsigned int)current_frequency);
    }

    update_chart_data();
}

static void wave_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    update_chart_data();
}

void part3_ex1_waveform_generator(void)
{
    /* Initialize scope subsystem */
    aic_scope_init();

    /* Set up screen */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Waveform Generator");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00ff88), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Chart */
    main_chart = lv_chart_create(scr);
    lv_obj_set_size(main_chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_align(main_chart, LV_ALIGN_CENTER, 0, -10);
    lv_chart_set_type(main_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(main_chart, CHART_POINTS);
    lv_chart_set_range(main_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Chart style - oscilloscope green */
    lv_obj_set_style_bg_color(main_chart, lv_color_hex(0x001100), 0);
    lv_obj_set_style_line_color(main_chart, lv_color_hex(0x003300), LV_PART_MAIN);

    /* Add series */
    chart_series = lv_chart_add_series(main_chart, lv_color_hex(0x00ff00),
                                        LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(main_chart, chart_series, 50);

    /* Waveform type dropdown */
    lv_obj_t *dropdown = lv_dropdown_create(scr);
    lv_dropdown_set_options(dropdown, "Square\nSine\nTriangle\nSawtooth\nNoise");
    lv_dropdown_set_selected(dropdown, 1);  /* Sine default */
    lv_obj_align(dropdown, LV_ALIGN_BOTTOM_LEFT, 20, -40);
    lv_obj_add_event_cb(dropdown, wave_type_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Frequency slider */
    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 150);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 30, LV_ANIM_OFF);  /* ~1000 Hz */
    lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_add_event_cb(slider, freq_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Frequency label */
    freq_label = lv_label_create(scr);
    lv_label_set_text_fmt(freq_label, "Freq: %u Hz", (unsigned int)current_frequency);
    lv_obj_set_style_text_color(freq_label, lv_color_hex(0xffffff), 0);
    lv_obj_align(freq_label, LV_ALIGN_BOTTOM_MID, 0, -65);

    /* Info label */
    info_label = lv_label_create(scr);
    lv_label_set_text(info_label, "Waveform: Sine");
    lv_obj_set_style_text_color(info_label, lv_color_hex(0x888888), 0);
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_RIGHT, -20, -45);

    /* Footer */
    aic_create_footer(scr);

    /* Initial data */
    current_wave_type = AIC_WAVE_SINE;
    update_chart_data();

    /* Create update timer */
    update_timer = lv_timer_create(wave_timer_cb, 100, NULL);

    printf("[Week5] Example 1: Waveform Generator started\r\n");
}

/*******************************************************************************
 * Example 2: Noise Generator
 ******************************************************************************/

static void noise_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (main_chart == NULL || chart_series == NULL) return;

    /* Generate new noise data */
    aic_scope_generate_noise(waveform_buffer, CHART_POINTS, current_amplitude);

    /* Update chart */
    for (int i = 0; i < CHART_POINTS; i++) {
        int32_t scaled = 50 + (waveform_buffer[i] * 40) / 32767;
        lv_chart_set_value_by_id(main_chart, chart_series, i, scaled);
    }

    lv_chart_refresh(main_chart);
}

void part3_ex2_noise_generator(void)
{
    /* Initialize scope */
    aic_scope_init();

    /* Set up screen */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Noise Generator - White Noise");
    lv_obj_set_style_text_color(title, lv_color_hex(0xff8800), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Chart */
    main_chart = lv_chart_create(scr);
    lv_obj_set_size(main_chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_align(main_chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(main_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(main_chart, CHART_POINTS);
    lv_chart_set_range(main_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Chart style */
    lv_obj_set_style_bg_color(main_chart, lv_color_hex(0x111111), 0);

    /* Add series (orange for noise) */
    chart_series = lv_chart_add_series(main_chart, lv_color_hex(0xff6600),
                                        LV_CHART_AXIS_PRIMARY_Y);

    /* Info label */
    info_label = lv_label_create(scr);
    lv_label_set_text(info_label, "LFSR-based pseudo-random noise");
    lv_obj_set_style_text_color(info_label, lv_color_hex(0x888888), 0);
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_MID, 0, -40);

    /* Footer */
    aic_create_footer(scr);

    /* Create timer for continuous noise update */
    update_timer = lv_timer_create(noise_timer_cb, 50, NULL);

    printf("[Week5] Example 2: Noise Generator started\r\n");
}

/*******************************************************************************
 * Example 3: Audio Waveform Display
 ******************************************************************************/

static bool audio_playing = false;

static void play_btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);

    audio_playing = !audio_playing;

    if (audio_playing) {
        lv_label_set_text(label, LV_SYMBOL_PAUSE " Pause");
        /* Start audio output simulation */
        aic_audio_in_set_simulated(AIC_WAVE_SINE, 440);  /* A4 note */
    } else {
        lv_label_set_text(label, LV_SYMBOL_PLAY " Play");
    }
}

static void audio_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (!audio_playing || main_chart == NULL || chart_series == NULL) {
        return;
    }

    /* Get audio samples (simulated or real) */
    aic_audio_in_get_samples(waveform_buffer, CHART_POINTS);

    /* Update chart */
    for (int i = 0; i < CHART_POINTS; i++) {
        int32_t scaled = 50 + (waveform_buffer[i] * 40) / 32767;
        lv_chart_set_value_by_id(main_chart, chart_series, i, scaled);
    }

    lv_chart_refresh(main_chart);
}

void part3_ex3_audio_waveform(void)
{
    /* Initialize audio */
    aic_scope_init();
    aic_audio_in_init(48000);

    /* Set up screen */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Audio Waveform Display");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00aaff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Chart */
    main_chart = lv_chart_create(scr);
    lv_obj_set_size(main_chart, CHART_WIDTH, CHART_HEIGHT);
    lv_obj_align(main_chart, LV_ALIGN_CENTER, 0, -10);
    lv_chart_set_type(main_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(main_chart, CHART_POINTS);
    lv_chart_set_range(main_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Chart style */
    lv_obj_set_style_bg_color(main_chart, lv_color_hex(0x001122), 0);

    /* Add series (blue for audio) */
    chart_series = lv_chart_add_series(main_chart, lv_color_hex(0x0088ff),
                                        LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(main_chart, chart_series, 50);

    /* Play button */
    lv_obj_t *play_btn = lv_btn_create(scr);
    lv_obj_set_size(play_btn, 120, 40);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_add_event_cb(play_btn, play_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(play_btn);
    lv_label_set_text(btn_label, LV_SYMBOL_PLAY " Play");
    lv_obj_center(btn_label);

    /* Info label */
    info_label = lv_label_create(scr);
    lv_label_set_text(info_label, "Press Play to start audio simulation (440 Hz A4)");
    lv_obj_set_style_text_color(info_label, lv_color_hex(0x888888), 0);
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_MID, 0, -15);

    /* Footer */
    aic_create_footer(scr);

    /* Create update timer */
    update_timer = lv_timer_create(audio_timer_cb, 33, NULL);

    printf("[Week5] Example 3: Audio Waveform started\r\n");
}

/*******************************************************************************
 * Example 4: Microphone Visualizer
 ******************************************************************************/

static lv_obj_t *level_bar = NULL;
static lv_obj_t *level_label = NULL;

static void mic_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (main_chart == NULL || chart_series == NULL) return;

    /* Get microphone samples */
    aic_audio_in_get_samples(waveform_buffer, CHART_POINTS);

    /* Update chart */
    for (int i = 0; i < CHART_POINTS; i++) {
        int32_t scaled = 50 + (waveform_buffer[i] * 40) / 32767;
        lv_chart_set_value_by_id(main_chart, chart_series, i, scaled);
    }
    lv_chart_refresh(main_chart);

    /* Calculate and update level */
    uint8_t level = aic_audio_in_get_level();
    if (level_bar != NULL) {
        lv_bar_set_value(level_bar, level, LV_ANIM_ON);
    }
    if (level_label != NULL) {
        lv_label_set_text_fmt(level_label, "Level: %d%%", level);
    }
}

void part3_ex4_mic_visualizer(void)
{
    /* Initialize audio input */
    aic_scope_init();
    aic_audio_in_init(48000);
    aic_audio_in_set_simulated(AIC_WAVE_SINE, 500);  /* Simulated 500 Hz */
    aic_audio_in_start();

    /* Set up screen */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Microphone Visualizer");
    lv_obj_set_style_text_color(title, lv_color_hex(0xff00ff), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Chart */
    main_chart = lv_chart_create(scr);
    lv_obj_set_size(main_chart, CHART_WIDTH - 60, CHART_HEIGHT - 20);
    lv_obj_align(main_chart, LV_ALIGN_CENTER, -30, 0);
    lv_chart_set_type(main_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(main_chart, CHART_POINTS);
    lv_chart_set_range(main_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Chart style */
    lv_obj_set_style_bg_color(main_chart, lv_color_hex(0x110022), 0);

    /* Add series (magenta for mic) */
    chart_series = lv_chart_add_series(main_chart, lv_color_hex(0xff00ff),
                                        LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(main_chart, chart_series, 50);

    /* Level bar */
    level_bar = lv_bar_create(scr);
    lv_obj_set_size(level_bar, 30, CHART_HEIGHT - 20);
    lv_obj_align(level_bar, LV_ALIGN_CENTER, CHART_WIDTH / 2 - 30, 0);
    lv_bar_set_range(level_bar, 0, 100);
    lv_bar_set_value(level_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(level_bar, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_bg_color(level_bar, lv_color_hex(0x00ff00), LV_PART_INDICATOR);

    /* Level label */
    level_label = lv_label_create(scr);
    lv_label_set_text(level_label, "Level: 0%");
    lv_obj_set_style_text_color(level_label, lv_color_hex(0xffffff), 0);
    lv_obj_align(level_label, LV_ALIGN_BOTTOM_RIGHT, -20, -40);

    /* Footer */
    aic_create_footer(scr);

    /* Create update timer */
    update_timer = lv_timer_create(mic_timer_cb, 33, NULL);

    printf("[Week5] Example 4: Microphone Visualizer started\r\n");
}

/*******************************************************************************
 * Example 5: Full Oscilloscope UI
 ******************************************************************************/

static lv_obj_t *vpp_label = NULL;
static lv_obj_t *freq_meas_label = NULL;
static lv_obj_t *rms_label = NULL;
static uint8_t time_div_idx = 6;
static uint8_t volt_div_idx = 5;

static void scope_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (main_chart == NULL || chart_series == NULL) return;

    /* Generate waveform */
    aic_wavegen_config_t config = {
        .type = current_wave_type,
        .frequency_hz = current_frequency,
        .sample_rate_hz = DISPLAY_SAMPLE_RATE,
        .amplitude = current_amplitude,
        .dc_offset = 0,
        .duty_percent = 50
    };
    aic_scope_generate_wave(waveform_buffer, CHART_POINTS, &config);

    /* Update chart */
    for (int i = 0; i < CHART_POINTS; i++) {
        int32_t scaled = 50 + (waveform_buffer[i] * 40) / 32767;
        lv_chart_set_value_by_id(main_chart, chart_series, i, scaled);
    }
    lv_chart_refresh(main_chart);

    /* Update measurements */
    int32_t vpp = aic_signal_peak_to_peak(waveform_buffer, CHART_POINTS);
    uint32_t freq = aic_signal_frequency(waveform_buffer, CHART_POINTS, DISPLAY_SAMPLE_RATE);
    int16_t rms = aic_signal_rms(waveform_buffer, CHART_POINTS);

    if (vpp_label) {
        lv_label_set_text_fmt(vpp_label, "Vpp: %d", (int)(vpp / 327));
    }
    if (freq_meas_label) {
        lv_label_set_text_fmt(freq_meas_label, "Freq: %u Hz", (unsigned int)freq);
    }
    if (rms_label) {
        lv_label_set_text_fmt(rms_label, "RMS: %d", (int)(rms / 327));
    }
}

static void scope_wave_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t *dropdown = lv_event_get_target(e);
    current_wave_type = (aic_wave_type_t)lv_dropdown_get_selected(dropdown);
}

static void scope_freq_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    current_frequency = 100 + (value * value);

    if (freq_label) {
        lv_label_set_text_fmt(freq_label, "%u Hz", (unsigned int)current_frequency);
    }
}

void part3_ex5_oscilloscope_ui(void)
{
    aic_scope_init();

    /* Set up screen */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x0a0a0a), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Digital Oscilloscope");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00ff00), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    /* Chart (oscilloscope display) */
    main_chart = lv_chart_create(scr);
    lv_obj_set_size(main_chart, 360, 160);
    lv_obj_align(main_chart, LV_ALIGN_TOP_LEFT, 10, 35);
    lv_chart_set_type(main_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(main_chart, CHART_POINTS);
    lv_chart_set_range(main_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_div_line_count(main_chart, 8, 10);

    /* Chart style - classic oscilloscope green on black */
    lv_obj_set_style_bg_color(main_chart, lv_color_hex(0x001100), 0);
    lv_obj_set_style_line_color(main_chart, lv_color_hex(0x003300), LV_PART_MAIN);

    /* Add series */
    chart_series = lv_chart_add_series(main_chart, lv_color_hex(0x00ff00),
                                        LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_all_value(main_chart, chart_series, 50);

    /* Control panel (right side) */
    lv_obj_t *ctrl_panel = lv_obj_create(scr);
    lv_obj_set_size(ctrl_panel, 100, 160);
    lv_obj_align(ctrl_panel, LV_ALIGN_TOP_RIGHT, -5, 35);
    lv_obj_set_style_bg_color(ctrl_panel, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_pad_all(ctrl_panel, 5, 0);

    /* Wave type dropdown */
    lv_obj_t *wave_dd = lv_dropdown_create(ctrl_panel);
    lv_dropdown_set_options(wave_dd, "Square\nSine\nTriangle");
    lv_obj_set_width(wave_dd, 85);
    lv_obj_align(wave_dd, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(wave_dd, scope_wave_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Frequency control */
    lv_obj_t *freq_slider = lv_slider_create(ctrl_panel);
    lv_obj_set_width(freq_slider, 85);
    lv_slider_set_range(freq_slider, 0, 100);
    lv_slider_set_value(freq_slider, 30, LV_ANIM_OFF);
    lv_obj_align(freq_slider, LV_ALIGN_TOP_MID, 0, 45);
    lv_obj_add_event_cb(freq_slider, scope_freq_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    freq_label = lv_label_create(ctrl_panel);
    lv_label_set_text_fmt(freq_label, "%u Hz", (unsigned int)current_frequency);
    lv_obj_set_style_text_color(freq_label, lv_color_hex(0xffffff), 0);
    lv_obj_align(freq_label, LV_ALIGN_TOP_MID, 0, 65);

    /* Time/div label */
    lv_obj_t *tdiv_label = lv_label_create(ctrl_panel);
    lv_label_set_text_fmt(tdiv_label, "%s", aic_scope_time_div_str(time_div_idx));
    lv_obj_set_style_text_color(tdiv_label, lv_color_hex(0xffff00), 0);
    lv_obj_align(tdiv_label, LV_ALIGN_TOP_MID, 0, 90);

    /* Volt/div label */
    lv_obj_t *vdiv_label = lv_label_create(ctrl_panel);
    lv_label_set_text_fmt(vdiv_label, "%s", aic_scope_volt_div_str(volt_div_idx));
    lv_obj_set_style_text_color(vdiv_label, lv_color_hex(0x00ffff), 0);
    lv_obj_align(vdiv_label, LV_ALIGN_TOP_MID, 0, 110);

    /* Measurements panel (bottom) */
    lv_obj_t *meas_panel = lv_obj_create(scr);
    lv_obj_set_size(meas_panel, SCREEN_WIDTH - 20, 50);
    lv_obj_align(meas_panel, LV_ALIGN_BOTTOM_MID, 0, -25);
    lv_obj_set_style_bg_color(meas_panel, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_flex_flow(meas_panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(meas_panel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Measurement labels */
    vpp_label = lv_label_create(meas_panel);
    lv_label_set_text(vpp_label, "Vpp: --");
    lv_obj_set_style_text_color(vpp_label, lv_color_hex(0x00ff00), 0);

    freq_meas_label = lv_label_create(meas_panel);
    lv_label_set_text(freq_meas_label, "Freq: -- Hz");
    lv_obj_set_style_text_color(freq_meas_label, lv_color_hex(0xffff00), 0);

    rms_label = lv_label_create(meas_panel);
    lv_label_set_text(rms_label, "RMS: --");
    lv_obj_set_style_text_color(rms_label, lv_color_hex(0x00ffff), 0);

    /* Footer */
    aic_create_footer(scr);

    /* Initial wave */
    current_wave_type = AIC_WAVE_SINE;
    lv_dropdown_set_selected(wave_dd, 1);

    /* Create update timer */
    update_timer = lv_timer_create(scope_timer_cb, 50, NULL);

    printf("[Week5] Example 5: Oscilloscope UI started\r\n");
}

/*******************************************************************************
 * Example 6: Spectrum Analyzer
 * Copy from Ex7 FFT Panel - uses dedicated variables
 ******************************************************************************/

/* Ex6 dedicated variables (not shared with other examples) */
static lv_obj_t *ex6_fft_chart = NULL;
static lv_chart_series_t *ex6_fft_series = NULL;
static lv_obj_t *ex6_peak_label = NULL;
static lv_obj_t *ex6_gain_slider = NULL;
static lv_timer_t *ex6_timer = NULL;
static uint8_t ex6_fft_gain = 50;

static void ex6_fft_gain_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *slider = lv_event_get_target(e);
    ex6_fft_gain = (uint8_t)lv_slider_get_value(slider);
}

static void ex6_wave_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *dropdown = lv_event_get_target(e);
    current_wave_type = (aic_wave_type_t)lv_dropdown_get_selected(dropdown);
}

static void ex6_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    if (ex6_fft_chart == NULL || ex6_fft_series == NULL) return;

    /* Generate waveform for FFT */
    aic_wavegen_config_t config = {
        .type = current_wave_type,
        .frequency_hz = current_frequency,
        .sample_rate_hz = DISPLAY_SAMPLE_RATE,
        .amplitude = 16000,
        .dc_offset = 0,
        .duty_percent = 50
    };

    int16_t fft_input[FFT_SIZE];
    aic_scope_generate_wave(fft_input, FFT_SIZE, &config);

    /* Calculate FFT */
    aic_fft_calculate(fft_input, fft_output);

    /* Find max value for normalization (skip DC bin) */
    uint16_t fft_max = 1;
    for (int i = 1; i < FFT_BINS; i++) {
        if (fft_output[i] > fft_max) fft_max = fft_output[i];
    }

    /* Find dominant frequency */
    uint32_t dominant = aic_fft_dominant_frequency(fft_output, FFT_BINS, DISPLAY_SAMPLE_RATE);

    /* Update chart with normalized and gained values */
    for (int i = 0; i < FFT_CHART_BINS; i++) {
        int idx = i * (FFT_BINS / FFT_CHART_BINS);
        int32_t val = (fft_output[idx] * 100 * ex6_fft_gain) / (fft_max * 50);
        if (val > 100) val = 100;
        if (val < 0) val = 0;
        lv_chart_set_value_by_id(ex6_fft_chart, ex6_fft_series, i, val);
    }
    lv_chart_refresh(ex6_fft_chart);

    /* Update peak frequency label */
    if (ex6_peak_label) {
        lv_label_set_text_fmt(ex6_peak_label, "Peak: %u Hz", (unsigned int)dominant);
    }
}

void part3_ex6_spectrum_analyzer(void)
{
    aic_scope_init();
    aic_fft_init(FFT_SIZE);

    /* Reset Ex6 variables */
    ex6_fft_chart = NULL;
    ex6_fft_series = NULL;
    ex6_peak_label = NULL;
    ex6_fft_gain = 50;

    /* Set up screen - dark theme like Ex7 */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x0a0a1e), 0);

    /* Title */
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "FFT Spectrum Analyzer");
    lv_obj_set_style_text_color(title, lv_color_hex(0xff6600), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /*=========================================================================
     * FFT BAR CHART - Copied from Ex7 FFT Panel (WORKING CODE)
     *=========================================================================*/
    ex6_fft_chart = lv_chart_create(scr);
    lv_obj_set_size(ex6_fft_chart, 400, 180);
    lv_obj_align(ex6_fft_chart, LV_ALIGN_CENTER, 0, -10);
    lv_chart_set_type(ex6_fft_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_point_count(ex6_fft_chart, FFT_CHART_BINS);
    lv_chart_set_range(ex6_fft_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Cyan theme for FFT - EXACT copy from Ex7 */
    lv_obj_set_style_bg_color(ex6_fft_chart, lv_color_hex(0x001a1a), 0);
    lv_obj_set_style_line_color(ex6_fft_chart, lv_color_hex(0x003333), LV_PART_MAIN);
    lv_chart_set_div_line_count(ex6_fft_chart, 4, 5);
    lv_obj_set_style_border_width(ex6_fft_chart, 1, 0);
    lv_obj_set_style_border_color(ex6_fft_chart, lv_color_hex(0x006666), 0);
    lv_obj_set_style_pad_column(ex6_fft_chart, 2, 0);  /* CRITICAL: Space between bars */

    /* Add series - CYAN color */
    ex6_fft_series = lv_chart_add_series(ex6_fft_chart, lv_color_hex(0x00ffff), LV_CHART_AXIS_PRIMARY_Y);

    /* Initialize all bars to 0 */
    for (int i = 0; i < FFT_CHART_BINS; i++) {
        lv_chart_set_value_by_id(ex6_fft_chart, ex6_fft_series, i, 0);
    }
    lv_chart_refresh(ex6_fft_chart);

    /*=========================================================================
     * Controls
     *=========================================================================*/
    /* Frequency scale labels */
    lv_obj_t *freq_0 = lv_label_create(scr);
    lv_label_set_text(freq_0, "0 Hz");
    lv_obj_set_style_text_color(freq_0, lv_color_hex(0x888888), 0);
    lv_obj_align_to(freq_0, ex6_fft_chart, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 5);

    lv_obj_t *freq_max = lv_label_create(scr);
    lv_label_set_text_fmt(freq_max, "%u Hz", (unsigned int)(DISPLAY_SAMPLE_RATE / 2));
    lv_obj_set_style_text_color(freq_max, lv_color_hex(0x888888), 0);
    lv_obj_align_to(freq_max, ex6_fft_chart, LV_ALIGN_OUT_BOTTOM_RIGHT, -5, 5);

    /* Wave type dropdown */
    lv_obj_t *wave_dd = lv_dropdown_create(scr);
    lv_dropdown_set_options(wave_dd, "Square\nSine\nTriangle\nSawtooth");
    lv_dropdown_set_selected(wave_dd, 1);
    lv_obj_align(wave_dd, LV_ALIGN_TOP_LEFT, 10, 35);
    lv_obj_add_event_cb(wave_dd, ex6_wave_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Gain slider */
    lv_obj_t *gain_lbl = lv_label_create(scr);
    lv_label_set_text(gain_lbl, "Gain:");
    lv_obj_set_style_text_color(gain_lbl, lv_color_hex(0x00ffff), 0);
    lv_obj_align(gain_lbl, LV_ALIGN_BOTTOM_LEFT, 10, -50);

    ex6_gain_slider = lv_slider_create(scr);
    lv_obj_set_size(ex6_gain_slider, 140, 20);
    lv_slider_set_range(ex6_gain_slider, 10, 100);
    lv_slider_set_value(ex6_gain_slider, 50, LV_ANIM_OFF);
    lv_obj_align(ex6_gain_slider, LV_ALIGN_BOTTOM_LEFT, 60, -50);
    lv_obj_add_event_cb(ex6_gain_slider, ex6_fft_gain_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Peak frequency label */
    ex6_peak_label = lv_label_create(scr);
    lv_label_set_text(ex6_peak_label, "Peak: -- Hz");
    lv_obj_set_style_text_color(ex6_peak_label, lv_color_hex(0xffff00), 0);
    lv_obj_set_style_text_font(ex6_peak_label, &lv_font_montserrat_16, 0);
    lv_obj_align(ex6_peak_label, LV_ALIGN_TOP_RIGHT, -20, 40);

    current_wave_type = AIC_WAVE_SINE;
    current_frequency = 1000;

    /* Footer */
    aic_create_footer(scr);

    /* Create update timer */
    ex6_timer = lv_timer_create(ex6_timer_cb, 100, NULL);

    printf("[Week5] Example 6: Spectrum Analyzer started (Ex7 style)\r\n");
}

/* [] END OF FILE */
