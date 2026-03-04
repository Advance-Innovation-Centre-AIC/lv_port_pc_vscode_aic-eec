/*******************************************************************************
 * File Name:   part3_scope_example.c
 *
 * Description: Part 3 - Custom Panel Scope Application (Simulated)
 *              Embedded C for IoT Course
 *
 * Author:      Assoc. Prof. Wiroon Sriborrirux
 *              Embedded Systems Engineering, Faculty of Engineering,
 *              Burapha University
 *
 * This file implements the AIC-EEC Scope application which integrates:
 *   - Part 1 patterns: Button, Slider+Label, LED status, Switch
 *   - Part 2 patterns: Chart+Timer, Faded Area Effect
 *   - Part 3 API: scope.h waveform generation and FFT
 *
 * Uses custom panels instead of TabView for maximum chart display area.
 *
 * =============================================================================
 * SCREEN & COORDINATE SYSTEM (PSoC Edge E84 Eval Kit)
 * =============================================================================
 *       X=0           X=240          X=479
 *   Y=0  +-----------------------------+
 *        |                             |
 *        +----+------------------------+
 *        |    |                        |
 *        |Nav |    Chart Area          |
 *        |Bar |    (Waveform/FFT)      |
 *        | 70 |                        |
 *        |    +------------------------+
 *        |    |    Controls + Measure  |
 *   Y=319+----+------------------------+
 *
 * =============================================================================
 * CUSTOM PANEL SCOPE UI LAYOUT (480 x 320)
 * =============================================================================
 *
 *   +----+-----------------------------------------------+
 *   |NAV |  AIC-EEC Scope v1.0              [LED] Run    |
 *   +----+-----------------------------------------------+
 *   |    | +-------------------------------------------+ |
 *   |Scp |         CHART: Waveform (200 pts)           | | Chart: ~210px
 *   |    | |         with faded area effect            | |
 *   |Gen | +-------------------------------------------+ |
 *   |    |                                               |
 *   |FFT | Wave: [▼ Sine]    Freq: [=====●=====] 1kHz    | Controls: ~88px
 *   |    | [Vpp: 2.0V]  [Freq: 1kHz]  [RMS: 0.7V]        |
 *   | 70 |                                               |
 *   +----+-----------------------------------------------+
 *   |          (C) 2026 AIC-EEC.com                      | Footer: 22px
 *   +----------------------------------------------------+
 *
 * =============================================================================
 * TIMER UPDATE RATES
 * =============================================================================
 *   - Scope Panel: 33ms (30 FPS) for smooth waveform
 *   - Generator Panel: 33ms (30 FPS) for preview
 *   - FFT Panel: 100ms (10 FPS) - computationally intensive
 *
 ******************************************************************************/

#include "part3_scope_example.h"
#include "part3_examples.h"
#include "../aic-eec/aic-eec.h"
#include "../aic-eec/scope.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*******************************************************************************
 * Configuration
 ******************************************************************************/

#define SCOPE_CHART_POINTS      200

/* Widget sizes - LARGER for touch-friendly UI */
#define DROPDOWN_WIDTH          110
#define DROPDOWN_HEIGHT         35
#define SLIDER_WIDTH            140
#define SLIDER_HEIGHT           25      /* Increased for better touch */
#define SLIDER_EXT_CLICK        15      /* Extended click area for touch */
#define SWITCH_WIDTH            55
#define SWITCH_HEIGHT           28
#define BUTTON_WIDTH            60
#define BUTTON_HEIGHT           32
#define LED_SIZE                20

#define FFT_SIZE                256
#define FFT_BINS                (FFT_SIZE / 2)
#define FFT_CHART_BINS          64      /* Displayed bins (decimated) */

#define SAMPLE_RATE             48000
#define GEN_SAMPLE_RATE         10000   /* Lower rate for Gen Tab preview (better duty visibility) */
#define DEFAULT_FREQUENCY       1000
#define DEFAULT_AMPLITUDE       16000

#define SCOPE_UPDATE_MS         33      /* 30 FPS */

/*******************************************************************************
 * Static Variables
 ******************************************************************************/

/* Navigation buttons and panels */
static lv_obj_t *ex7_nav_btns[3] = {NULL, NULL, NULL};
static lv_obj_t *ex7_panels[3] = {NULL, NULL, NULL};

/* Scope panel widgets */
static lv_obj_t *ex7_chart = NULL;
static lv_chart_series_t *ex7_series = NULL;
static lv_obj_t *ex7_dropdown = NULL;
static lv_obj_t *ex7_freq_slider = NULL;
static lv_obj_t *ex7_freq_label = NULL;
static lv_obj_t *ex7_run_btn = NULL;
static lv_obj_t *ex7_run_led = NULL;
static lv_obj_t *ex7_vpp_label = NULL;
static lv_obj_t *ex7_freq_meas_label = NULL;
static lv_obj_t *ex7_rms_label = NULL;

/* Generator panel widgets */
static lv_obj_t *ex7_gen_chart = NULL;
static lv_chart_series_t *ex7_gen_series = NULL;
static lv_obj_t *ex7_gen_dropdown = NULL;
static lv_obj_t *ex7_gen_freq_slider = NULL;
static lv_obj_t *ex7_gen_freq_label = NULL;
static lv_obj_t *ex7_gen_duty_slider = NULL;
static lv_obj_t *ex7_gen_duty_label = NULL;
static lv_obj_t *ex7_gen_switch = NULL;

/* FFT panel widgets (no dropdown - analyzes current waveform) */
static lv_obj_t *ex7_fft_chart = NULL;
static lv_chart_series_t *ex7_fft_series = NULL;
static lv_obj_t *ex7_fft_gain_slider = NULL;
static lv_obj_t *ex7_fft_dominant_label = NULL;

/* Timer and state */
static lv_timer_t *ex7_timer = NULL;
static uint8_t ex7_active_panel = 0;

/* Waveform state */
static aic_wave_type_t ex7_wave_type = AIC_WAVE_SINE;
static uint32_t ex7_frequency = DEFAULT_FREQUENCY;
static int16_t ex7_amplitude = DEFAULT_AMPLITUDE;
static uint8_t ex7_duty = 50;
static bool ex7_is_running = true;
static uint8_t ex7_fft_gain = 50;

/* Generator-specific frequency (10-500 Hz for clear duty cycle visualization) */
static uint32_t ex7_gen_frequency = 100;

/* Runtime dimensions (set in main function) */
static int32_t ex7_content_width = 0;
static int32_t ex7_content_height = 0;
static int32_t ex7_chart_width = 0;
static int32_t ex7_chart_height = 0;

/* Buffers */
static int16_t ex7_waveform_buffer[SCOPE_CHART_POINTS];
static uint16_t ex7_fft_output[FFT_BINS];

/*******************************************************************************
 * Faded Area Draw Callback - DISABLED for PC Simulator (LVGL 9.5 API change)
 * Note: lv_draw_triangle_dsc_t.bg_grad was removed in LVGL 9.5
 ******************************************************************************/
#if 0  /* Disabled for PC simulator - LVGL version incompatibility */
static void ex7_area_draw_cb(lv_event_t *e)
{
    /* ... original code ... */
}
#endif

/*******************************************************************************
 * Navigation Button Callback
 ******************************************************************************/
static void ex7_nav_btn_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    int panel_idx = (int)(intptr_t)lv_event_get_user_data(e);

    /* Hide all panels, show selected */
    for (int i = 0; i < 3; i++) {
        if (ex7_panels[i]) {
            if (i == panel_idx) {
                lv_obj_remove_flag(ex7_panels[i], LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(ex7_panels[i], LV_OBJ_FLAG_HIDDEN);
            }
        }
        /* Update nav button styles */
        if (ex7_nav_btns[i]) {
            if (i == panel_idx) {
                /* Active button: bright green */
                lv_obj_set_style_bg_color(ex7_nav_btns[i], lv_color_hex(0x00ff88), 0);
                lv_obj_set_style_text_color(ex7_nav_btns[i], lv_color_hex(0x000000), 0);
            } else {
                /* Inactive button: dark */
                lv_obj_set_style_bg_color(ex7_nav_btns[i], lv_color_hex(0x1a1a2e), 0);
                lv_obj_set_style_text_color(ex7_nav_btns[i], lv_color_hex(0xAAAAAA), 0);
            }
        }
    }
    ex7_active_panel = panel_idx;
}

/*******************************************************************************
 * Timer Callback - Updates active panel (Scope/Gen/FFT)
 ******************************************************************************/
static void ex7_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    /* Touch Keep-Alive: Prevent touch controller sleep/timeout
     * Reset input device every ~1 second (30 * 33ms) to keep touch responsive
     * This fixes issue where touch stops working after screen left on for a while
     */
    static uint32_t touch_keepalive_cnt = 0;
    if (++touch_keepalive_cnt >= 30) {
        touch_keepalive_cnt = 0;
        lv_indev_t *indev = lv_indev_get_next(NULL);
        while (indev) {
            if (lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER) {
                lv_indev_reset(indev, NULL);
                break;
            }
            indev = lv_indev_get_next(indev);
        }
    }

    if (!ex7_is_running) return;

    /* Update based on active panel */
    if (ex7_active_panel == 0 && ex7_chart && ex7_series) {
        /* === SCOPE PANEL === */
        aic_wavegen_config_t config = {
            .type = ex7_wave_type,
            .frequency_hz = ex7_frequency,
            .sample_rate_hz = SAMPLE_RATE,
            .amplitude = ex7_amplitude,
            .dc_offset = 0,
            .duty_percent = ex7_duty  /* Square wave now uses duty */
        };

        aic_scope_generate_wave(ex7_waveform_buffer, SCOPE_CHART_POINTS, &config);

        for (int i = 0; i < SCOPE_CHART_POINTS; i++) {
            int32_t scaled = 50 + (ex7_waveform_buffer[i] * 40) / 32767;
            lv_chart_set_value_by_id(ex7_chart, ex7_series, i, scaled);
        }
        lv_chart_refresh(ex7_chart);

        /* Update measurements */
        int32_t p2p = aic_signal_peak_to_peak(ex7_waveform_buffer, SCOPE_CHART_POINTS);
        float vpp = (float)p2p / 32767.0f * 3.3f;
        int16_t rms_raw = aic_signal_rms(ex7_waveform_buffer, SCOPE_CHART_POINTS);
        float rms = (float)rms_raw / 32767.0f * 3.3f;

        if (ex7_vpp_label) {
            lv_label_set_text_fmt(ex7_vpp_label, "Vpp: %.2fV", (double)vpp);
        }
        if (ex7_freq_meas_label) {
            lv_label_set_text_fmt(ex7_freq_meas_label, "Freq: %uHz", (unsigned int)ex7_frequency);
        }
        if (ex7_rms_label) {
            lv_label_set_text_fmt(ex7_rms_label, "RMS: %.2fV", (double)rms);
        }
    }
    else if (ex7_active_panel == 1 && ex7_gen_chart && ex7_gen_series) {
        /* === GENERATOR PANEL === (uses lower sample rate for better duty visibility) */
        aic_wavegen_config_t config = {
            .type = ex7_wave_type,
            .frequency_hz = ex7_gen_frequency,
            .sample_rate_hz = GEN_SAMPLE_RATE,  /* 10kHz for clear waveform preview */
            .amplitude = ex7_amplitude,
            .dc_offset = 0,
            .duty_percent = ex7_duty
        };

        aic_scope_generate_wave(ex7_waveform_buffer, SCOPE_CHART_POINTS, &config);

        for (int i = 0; i < SCOPE_CHART_POINTS; i++) {
            int32_t scaled = 50 + (ex7_waveform_buffer[i] * 40) / 32767;
            lv_chart_set_value_by_id(ex7_gen_chart, ex7_gen_series, i, scaled);
        }
        lv_chart_refresh(ex7_gen_chart);
    }
    else if (ex7_active_panel == 2 && ex7_fft_chart && ex7_fft_series) {
        /* === FFT PANEL === (analyzes current waveform settings) */
        aic_wavegen_config_t config = {
            .type = ex7_wave_type,
            .frequency_hz = ex7_frequency,
            .sample_rate_hz = SAMPLE_RATE,
            .amplitude = ex7_amplitude,
            .dc_offset = 0,
            .duty_percent = ex7_duty
        };

        int16_t fft_input[FFT_SIZE];
        aic_scope_generate_wave(fft_input, FFT_SIZE, &config);

        /* Calculate FFT */
        aic_fft_calculate(fft_input, ex7_fft_output);

        /* Find max value for normalization */
        uint16_t fft_max = 1;
        for (int i = 1; i < FFT_BINS; i++) {
            if (ex7_fft_output[i] > fft_max) {
                fft_max = ex7_fft_output[i];
            }
        }

        /* Find dominant frequency */
        uint32_t dominant = aic_fft_dominant_frequency(ex7_fft_output, FFT_BINS, SAMPLE_RATE);

        /* Update FFT chart with normalized values */
        for (int i = 0; i < FFT_CHART_BINS; i++) {
            int idx = i * (FFT_BINS / FFT_CHART_BINS);
            int32_t val = (ex7_fft_output[idx] * 100 * ex7_fft_gain) / (fft_max * 50);
            if (val > 100) val = 100;
            if (val < 0) val = 0;
            lv_chart_set_value_by_id(ex7_fft_chart, ex7_fft_series, i, val);
        }
        lv_chart_refresh(ex7_fft_chart);

        /* Update dominant frequency label */
        if (ex7_fft_dominant_label) {
            lv_label_set_text_fmt(ex7_fft_dominant_label, "Peak: %u Hz", (unsigned int)dominant);
        }
    }
}

/*******************************************************************************
 * Event Callbacks
 ******************************************************************************/
static void ex7_wave_type_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *dropdown = lv_event_get_target(e);
    ex7_wave_type = (aic_wave_type_t)lv_dropdown_get_selected(dropdown);
}

static void ex7_freq_slider_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t val = lv_slider_get_value(slider);
    ex7_frequency = 100 + (val * val);

    if (ex7_freq_label) {
        if (ex7_frequency >= 1000) {
            lv_label_set_text_fmt(ex7_freq_label, "%.1fkHz", (double)ex7_frequency / 1000.0);
        } else {
            lv_label_set_text_fmt(ex7_freq_label, "%uHz", (unsigned int)ex7_frequency);
        }
    }
}

static void ex7_run_btn_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    ex7_is_running = !ex7_is_running;

    if (ex7_run_led) {
        if (ex7_is_running) {
            lv_led_on(ex7_run_led);
        } else {
            lv_led_off(ex7_run_led);
        }
    }

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    if (label) {
        lv_label_set_text(label, ex7_is_running ? "Stop" : "Run");
    }
}

/* Gen panel: Frequency slider callback (10-500 Hz for clear duty cycle view) */
static void ex7_gen_freq_slider_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t val = lv_slider_get_value(slider);

    /* Linear mapping: slider 10-500 -> freq 10-500 Hz */
    ex7_gen_frequency = (uint32_t)val;
    if (ex7_gen_frequency < 10) ex7_gen_frequency = 10;

    if (ex7_gen_freq_label) {
        lv_label_set_text_fmt(ex7_gen_freq_label, "%uHz", (unsigned int)ex7_gen_frequency);
    }
}

/* Gen panel: Duty slider callback */
static void ex7_gen_duty_slider_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *slider = lv_event_get_target(e);
    ex7_duty = (uint8_t)lv_slider_get_value(slider);

    if (ex7_gen_duty_label) {
        lv_label_set_text_fmt(ex7_gen_duty_label, "%u%%", (unsigned int)ex7_duty);
    }
}

/* Gen panel: Switch callback */
static void ex7_gen_switch_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *sw = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
    printf("[Scope Ex7] Generator output: %s\r\n", enabled ? "ON" : "OFF");
}

/* FFT panel: Gain slider callback */
static void ex7_fft_gain_slider_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
    lv_obj_t *slider = lv_event_get_target(e);
    ex7_fft_gain = (uint8_t)lv_slider_get_value(slider);
}

/*******************************************************************************
 * Create Scope Panel Content
 ******************************************************************************/
static void ex7_create_scope_panel(lv_obj_t *panel)
{
    int32_t chart_w = ex7_chart_width;
    int32_t chart_h = ex7_chart_height;

    printf("[Week5] Ex7: Creating chart with size: %dx%d\r\n", (int)chart_w, (int)chart_h);

    /* === FULLSCREEN CHART at top-left (no margins) === */
    ex7_chart = lv_chart_create(panel);
    lv_obj_set_size(ex7_chart, chart_w, chart_h);
    lv_obj_align(ex7_chart, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_chart_set_type(ex7_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ex7_chart, SCOPE_CHART_POINTS);
    lv_chart_set_range(ex7_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Oscilloscope style - green on dark with grid */
    lv_obj_set_style_bg_color(ex7_chart, lv_color_hex(0x001100), 0);
    lv_obj_set_style_line_color(ex7_chart, lv_color_hex(0x003300), LV_PART_MAIN);
    lv_obj_set_style_size(ex7_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(ex7_chart, 2, LV_PART_ITEMS);
    lv_chart_set_div_line_count(ex7_chart, 4, 5);
    lv_obj_set_style_border_width(ex7_chart, 1, 0);
    lv_obj_set_style_border_color(ex7_chart, lv_color_hex(0x004400), 0);

    /* Faded area effect disabled for PC simulator (LVGL 9.5 API change) */
    /* lv_obj_add_event_cb(ex7_chart, ex7_area_draw_cb, LV_EVENT_DRAW_TASK_ADDED, NULL); */
    /* lv_obj_add_flag(ex7_chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS); */

    ex7_series = lv_chart_add_series(ex7_chart, lv_color_hex(0x00ff00), LV_CHART_AXIS_PRIMARY_Y);

    /* === Controls at bottom (88px control area) === */
    /* ROW 1 (-55): Dropdown | Freq Slider + Label | LED + Button */
    ex7_dropdown = lv_dropdown_create(panel);
    lv_dropdown_set_options(ex7_dropdown, "Square\nSine\nTriangle\nSawtooth\nNoise\nPulse");
    lv_dropdown_set_selected(ex7_dropdown, 1);
    lv_obj_set_size(ex7_dropdown, DROPDOWN_WIDTH, DROPDOWN_HEIGHT);
    lv_obj_set_style_text_font(ex7_dropdown, &lv_font_montserrat_16, 0);
    lv_obj_align(ex7_dropdown, LV_ALIGN_BOTTOM_LEFT, 5, -50);
    lv_obj_add_event_cb(ex7_dropdown, ex7_wave_type_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ex7_freq_slider = lv_slider_create(panel);
    lv_obj_set_size(ex7_freq_slider, SLIDER_WIDTH, SLIDER_HEIGHT);
    lv_slider_set_range(ex7_freq_slider, 0, 100);
    lv_slider_set_value(ex7_freq_slider, 30, LV_ANIM_OFF);
    lv_obj_align(ex7_freq_slider, LV_ALIGN_BOTTOM_MID, -30, -55);
    lv_obj_set_ext_click_area(ex7_freq_slider, SLIDER_EXT_CLICK);
    lv_obj_add_event_cb(ex7_freq_slider, ex7_freq_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ex7_freq_label = lv_label_create(panel);
    lv_label_set_text(ex7_freq_label, "1.0kHz");
    lv_obj_set_style_text_color(ex7_freq_label, lv_color_hex(0x00ff00), 0);
    lv_obj_set_style_text_font(ex7_freq_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(ex7_freq_label, ex7_freq_slider, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    ex7_run_led = lv_led_create(panel);
    lv_obj_set_size(ex7_run_led, LED_SIZE, LED_SIZE);
    lv_led_set_color(ex7_run_led, lv_palette_main(LV_PALETTE_GREEN));
    lv_obj_align(ex7_run_led, LV_ALIGN_BOTTOM_RIGHT, -75, -55);
    lv_led_on(ex7_run_led);

    ex7_run_btn = lv_button_create(panel);
    lv_obj_set_size(ex7_run_btn, BUTTON_WIDTH, BUTTON_HEIGHT);
    lv_obj_align(ex7_run_btn, LV_ALIGN_BOTTOM_RIGHT, -5, -50);
    lv_obj_t *run_label = lv_label_create(ex7_run_btn);
    lv_label_set_text(run_label, "Stop");
    lv_obj_set_style_text_font(run_label, &lv_font_montserrat_14, 0);
    lv_obj_center(run_label);
    lv_obj_set_ext_click_area(ex7_run_btn, 15);
    lv_obj_add_event_cb(ex7_run_btn, ex7_run_btn_cb, LV_EVENT_CLICKED, NULL);

    /* ROW 2 (-10): Measurements */
    ex7_vpp_label = lv_label_create(panel);
    lv_label_set_text(ex7_vpp_label, "Vpp: --");
    lv_obj_set_style_text_color(ex7_vpp_label, lv_color_hex(0x00ff00), 0);
    lv_obj_set_style_text_font(ex7_vpp_label, &lv_font_montserrat_14, 0);
    lv_obj_align(ex7_vpp_label, LV_ALIGN_BOTTOM_LEFT, 5, -10);

    ex7_freq_meas_label = lv_label_create(panel);
    lv_label_set_text(ex7_freq_meas_label, "Freq: --");
    lv_obj_set_style_text_color(ex7_freq_meas_label, lv_color_hex(0xffff00), 0);
    lv_obj_set_style_text_font(ex7_freq_meas_label, &lv_font_montserrat_14, 0);
    lv_obj_align(ex7_freq_meas_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    ex7_rms_label = lv_label_create(panel);
    lv_label_set_text(ex7_rms_label, "RMS: --");
    lv_obj_set_style_text_color(ex7_rms_label, lv_color_hex(0x00ffff), 0);
    lv_obj_set_style_text_font(ex7_rms_label, &lv_font_montserrat_14, 0);
    lv_obj_align(ex7_rms_label, LV_ALIGN_BOTTOM_RIGHT, -5, -10);
}

/*******************************************************************************
 * Create Generator Panel Content
 ******************************************************************************/
static void ex7_create_gen_panel(lv_obj_t *panel)
{
    int32_t chart_w = ex7_chart_width;
    int32_t chart_h = ex7_chart_height;

    /* === CHART: Generator waveform preview === */
    ex7_gen_chart = lv_chart_create(panel);
    lv_obj_set_size(ex7_gen_chart, chart_w, chart_h);
    lv_obj_align(ex7_gen_chart, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_chart_set_type(ex7_gen_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ex7_gen_chart, SCOPE_CHART_POINTS);
    lv_chart_set_range(ex7_gen_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Orange theme for Generator */
    lv_obj_set_style_bg_color(ex7_gen_chart, lv_color_hex(0x110800), 0);
    lv_obj_set_style_line_color(ex7_gen_chart, lv_color_hex(0x332200), LV_PART_MAIN);
    lv_obj_set_style_size(ex7_gen_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(ex7_gen_chart, 2, LV_PART_ITEMS);
    lv_chart_set_div_line_count(ex7_gen_chart, 4, 5);
    lv_obj_set_style_border_width(ex7_gen_chart, 1, 0);
    lv_obj_set_style_border_color(ex7_gen_chart, lv_color_hex(0x664400), 0);

    ex7_gen_series = lv_chart_add_series(ex7_gen_chart, lv_color_hex(0xff8800), LV_CHART_AXIS_PRIMARY_Y);

    /* === Controls at bottom (88px control area) === */
    /* ROW 1 (-55): Dropdown | Freq Slider + Label | OUT + Switch */
    ex7_gen_dropdown = lv_dropdown_create(panel);
    lv_dropdown_set_options(ex7_gen_dropdown, "Square\nSine\nTriangle\nSawtooth\nNoise\nPulse");
    lv_dropdown_set_selected(ex7_gen_dropdown, 1);
    lv_obj_set_size(ex7_gen_dropdown, DROPDOWN_WIDTH, DROPDOWN_HEIGHT);
    lv_obj_set_style_text_font(ex7_gen_dropdown, &lv_font_montserrat_16, 0);
    lv_obj_align(ex7_gen_dropdown, LV_ALIGN_BOTTOM_LEFT, 5, -50);
    lv_obj_add_event_cb(ex7_gen_dropdown, ex7_wave_type_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ex7_gen_freq_slider = lv_slider_create(panel);
    lv_obj_set_size(ex7_gen_freq_slider, SLIDER_WIDTH, SLIDER_HEIGHT);
    lv_slider_set_range(ex7_gen_freq_slider, 10, 500);  /* 10-500 Hz for clear duty view */
    lv_slider_set_value(ex7_gen_freq_slider, 100, LV_ANIM_OFF);
    lv_obj_align(ex7_gen_freq_slider, LV_ALIGN_BOTTOM_MID, -60, -55);
    lv_obj_set_ext_click_area(ex7_gen_freq_slider, SLIDER_EXT_CLICK);
    lv_obj_add_event_cb(ex7_gen_freq_slider, ex7_gen_freq_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ex7_gen_freq_label = lv_label_create(panel);
    lv_label_set_text(ex7_gen_freq_label, "100Hz");
    lv_obj_set_style_text_color(ex7_gen_freq_label, lv_color_hex(0xff8800), 0);
    lv_obj_set_style_text_font(ex7_gen_freq_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(ex7_gen_freq_label, ex7_gen_freq_slider, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    lv_obj_t *out_lbl = lv_label_create(panel);
    lv_label_set_text(out_lbl, "OUT");
    lv_obj_set_style_text_color(out_lbl, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(out_lbl, LV_ALIGN_BOTTOM_RIGHT, -70, -55);

    ex7_gen_switch = lv_switch_create(panel);
    lv_obj_set_size(ex7_gen_switch, SWITCH_WIDTH, SWITCH_HEIGHT);
    lv_obj_align(ex7_gen_switch, LV_ALIGN_BOTTOM_RIGHT, -5, -50);
    lv_obj_add_event_cb(ex7_gen_switch, ex7_gen_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* ROW 2 (-10): Duty slider */
    lv_obj_t *duty_lbl = lv_label_create(panel);
    lv_label_set_text(duty_lbl, "Duty:");
    lv_obj_set_style_text_color(duty_lbl, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(duty_lbl, &lv_font_montserrat_14, 0);
    lv_obj_align(duty_lbl, LV_ALIGN_BOTTOM_LEFT, 5, -15);

    ex7_gen_duty_slider = lv_slider_create(panel);
    lv_obj_set_size(ex7_gen_duty_slider, 120, SLIDER_HEIGHT);
    lv_slider_set_range(ex7_gen_duty_slider, 10, 90);
    lv_slider_set_value(ex7_gen_duty_slider, 50, LV_ANIM_OFF);
    lv_obj_align(ex7_gen_duty_slider, LV_ALIGN_BOTTOM_LEFT, 55, -15);
    lv_obj_set_ext_click_area(ex7_gen_duty_slider, SLIDER_EXT_CLICK);
    lv_obj_add_event_cb(ex7_gen_duty_slider, ex7_gen_duty_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    ex7_gen_duty_label = lv_label_create(panel);
    lv_label_set_text(ex7_gen_duty_label, "50%");
    lv_obj_set_style_text_color(ex7_gen_duty_label, lv_color_hex(0xff8800), 0);
    lv_obj_set_style_text_font(ex7_gen_duty_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(ex7_gen_duty_label, ex7_gen_duty_slider, LV_ALIGN_OUT_RIGHT_MID, 8, 0);
}

/*******************************************************************************
 * Create FFT Panel Content
 ******************************************************************************/
static void ex7_create_fft_panel(lv_obj_t *panel)
{
    int32_t chart_w = ex7_chart_width;
    int32_t chart_h = ex7_chart_height;

    /* === CHART: FFT Spectrum (Bar Chart) === */
    ex7_fft_chart = lv_chart_create(panel);
    lv_obj_set_size(ex7_fft_chart, chart_w, chart_h);
    lv_obj_align(ex7_fft_chart, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_chart_set_type(ex7_fft_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_point_count(ex7_fft_chart, FFT_CHART_BINS);
    lv_chart_set_range(ex7_fft_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Cyan theme for FFT */
    lv_obj_set_style_bg_color(ex7_fft_chart, lv_color_hex(0x001a1a), 0);
    lv_obj_set_style_line_color(ex7_fft_chart, lv_color_hex(0x003333), LV_PART_MAIN);
    lv_chart_set_div_line_count(ex7_fft_chart, 4, 5);
    lv_obj_set_style_border_width(ex7_fft_chart, 1, 0);
    lv_obj_set_style_border_color(ex7_fft_chart, lv_color_hex(0x006666), 0);
    lv_obj_set_style_pad_column(ex7_fft_chart, 2, 0);

    ex7_fft_series = lv_chart_add_series(ex7_fft_chart, lv_color_hex(0x00ffff), LV_CHART_AXIS_PRIMARY_Y);

    /* Initialize all bars to 0 */
    for (int i = 0; i < FFT_CHART_BINS; i++) {
        lv_chart_set_value_by_id(ex7_fft_chart, ex7_fft_series, i, 0);
    }
    lv_chart_refresh(ex7_fft_chart);

    /* === Controls at bottom (88px control area) === */
    /* ROW 1 (-55): Gain Slider (FFT analyzes current waveform) */
    lv_obj_t *gain_lbl = lv_label_create(panel);
    lv_label_set_text(gain_lbl, "Gain:");
    lv_obj_set_style_text_color(gain_lbl, lv_color_hex(0x00ffff), 0);
    lv_obj_set_style_text_font(gain_lbl, &lv_font_montserrat_14, 0);
    lv_obj_align(gain_lbl, LV_ALIGN_BOTTOM_LEFT, 10, -55);

    ex7_fft_gain_slider = lv_slider_create(panel);
    lv_obj_set_size(ex7_fft_gain_slider, SLIDER_WIDTH, SLIDER_HEIGHT);
    lv_slider_set_range(ex7_fft_gain_slider, 10, 100);
    lv_slider_set_value(ex7_fft_gain_slider, 50, LV_ANIM_OFF);
    lv_obj_align(ex7_fft_gain_slider, LV_ALIGN_BOTTOM_LEFT, 60, -55);
    lv_obj_set_ext_click_area(ex7_fft_gain_slider, SLIDER_EXT_CLICK);
    lv_obj_add_event_cb(ex7_fft_gain_slider, ex7_fft_gain_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* ROW 2 (-10): Peak frequency label */
    ex7_fft_dominant_label = lv_label_create(panel);
    lv_label_set_text(ex7_fft_dominant_label, "Peak: -- Hz");
    lv_obj_set_style_text_color(ex7_fft_dominant_label, lv_color_hex(0xffff00), 0);
    lv_obj_set_style_text_font(ex7_fft_dominant_label, &lv_font_montserrat_16, 0);
    lv_obj_align(ex7_fft_dominant_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}

/*******************************************************************************
 * Main Function: Ex7 Custom Panel Scope
 ******************************************************************************/
void part3_ex7_custom_panel_scope(void)
{
    /* Initialize scope subsystem */
    aic_scope_init();
    aic_fft_init(FFT_SIZE);

    /* Set initial values */
    ex7_wave_type = AIC_WAVE_SINE;
    ex7_frequency = DEFAULT_FREQUENCY;
    ex7_amplitude = DEFAULT_AMPLITUDE;
    ex7_duty = 50;
    ex7_is_running = true;
    ex7_active_panel = 0;

    /* Get actual screen dimensions at runtime */
    lv_obj_t *screen = lv_screen_active();
    int32_t screen_w = lv_obj_get_width(screen);
    int32_t screen_h = lv_obj_get_height(screen);

    /* Calculate dimensions based on actual screen size */
    int32_t nav_width = 70;
    int32_t footer_height = 22;
    int32_t content_width = screen_w - nav_width;
    int32_t content_height = screen_h - footer_height;

    /* Store in static variables for use by create_scope_panel */
    ex7_content_width = content_width;
    ex7_content_height = content_height;
    ex7_chart_width = content_width;
    ex7_chart_height = content_height - 88;  /* Leave 88px for controls */

    printf("[Week5] Ex7: Screen actual size: %dx%d\r\n", (int)screen_w, (int)screen_h);
    printf("[Week5] Ex7: Content area: %dx%d\r\n", (int)content_width, (int)content_height);
    printf("[Week5] Ex7: Chart dimensions: %dx%d\r\n", (int)ex7_chart_width, (int)ex7_chart_height);

    /*=========================================================================
     * CRITICAL: Set screen background to DARK
     *=========================================================================*/
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x0a0a1e), 0);

    /*=========================================================================
     * Create Navigation Bar (LEFT side, 70px wide)
     *=========================================================================*/
    lv_obj_t *nav_bar = lv_obj_create(screen);
    lv_obj_set_size(nav_bar, nav_width, content_height);
    lv_obj_set_pos(nav_bar, 0, 0);
    lv_obj_set_style_bg_color(nav_bar, lv_color_hex(0x1a1a2e), 0);
    lv_obj_set_style_border_width(nav_bar, 0, 0);
    lv_obj_set_style_radius(nav_bar, 0, 0);
    lv_obj_set_style_pad_all(nav_bar, 5, 0);
    lv_obj_remove_flag(nav_bar, LV_OBJ_FLAG_SCROLLABLE);

    /* Navigation buttons - FIXED SIZE: 60x80, FIXED POSITIONS */
    const char *nav_labels[] = {"Scope", "Gen", "FFT"};
    const int btn_y_pos[] = {50, 170, 290};  /* Fixed Y positions */

    for (int i = 0; i < 3; i++) {
        ex7_nav_btns[i] = lv_button_create(nav_bar);
        lv_obj_set_size(ex7_nav_btns[i], 60, 80);
        lv_obj_set_pos(ex7_nav_btns[i], 0, btn_y_pos[i]);
        lv_obj_set_style_radius(ex7_nav_btns[i], 5, 0);

        lv_obj_t *label = lv_label_create(ex7_nav_btns[i]);
        lv_label_set_text(label, nav_labels[i]);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
        lv_obj_center(label);

        /* Initial style - first button active */
        if (i == 0) {
            lv_obj_set_style_bg_color(ex7_nav_btns[i], lv_color_hex(0x00ff88), 0);
            lv_obj_set_style_text_color(ex7_nav_btns[i], lv_color_hex(0x000000), 0);
        } else {
            lv_obj_set_style_bg_color(ex7_nav_btns[i], lv_color_hex(0x1a1a2e), 0);
            lv_obj_set_style_text_color(ex7_nav_btns[i], lv_color_hex(0xAAAAAA), 0);
        }

        lv_obj_add_event_cb(ex7_nav_btns[i], ex7_nav_btn_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);
    }

    /*=========================================================================
     * Create Content Panels (RIGHT side, fills remaining space)
     *=========================================================================*/
    for (int i = 0; i < 3; i++) {
        ex7_panels[i] = lv_obj_create(screen);
        lv_obj_set_size(ex7_panels[i], content_width, content_height);
        lv_obj_set_pos(ex7_panels[i], nav_width, 0);
        lv_obj_set_style_bg_color(ex7_panels[i], lv_color_hex(0x0a0a1e), 0);
        lv_obj_set_style_border_width(ex7_panels[i], 0, 0);
        lv_obj_set_style_radius(ex7_panels[i], 0, 0);
        lv_obj_set_style_pad_all(ex7_panels[i], 0, 0);  /* NO PADDING for fullscreen */
        lv_obj_remove_flag(ex7_panels[i], LV_OBJ_FLAG_SCROLLABLE);

        /* Hide all except first panel */
        if (i != 0) {
            lv_obj_add_flag(ex7_panels[i], LV_OBJ_FLAG_HIDDEN);
        }
    }

    /* Populate panels */
    ex7_create_scope_panel(ex7_panels[0]);
    ex7_create_gen_panel(ex7_panels[1]);
    ex7_create_fft_panel(ex7_panels[2]);

    /*=========================================================================
     * Footer overlay (created AFTER panels so it appears on top)
     *=========================================================================*/
    aic_create_footer(screen);

    /* Create timer for updates */
    ex7_timer = lv_timer_create(ex7_timer_cb, SCOPE_UPDATE_MS, NULL);

    printf("[Week5] Ex7: Custom Panel Scope started\r\n");
    printf("  - No TabView constraints!\r\n");
    printf("  - Square wave now supports Duty Cycle\r\n");
}
