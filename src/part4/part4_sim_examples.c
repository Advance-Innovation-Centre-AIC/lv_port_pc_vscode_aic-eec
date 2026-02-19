/*******************************************************************************
 * File: part4_sim_examples.c
 * Description: Part 4 Examples - IPC, Logging, Event Bus (PC Simulator)
 *
 * This file provides simulated versions of IPC, logging, and event bus
 * for testing on PC without actual hardware.
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#include "part4_examples.h"
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*******************************************************************************
 * Color Palette (Dark Theme)
 ******************************************************************************/

#define COLOR_BG            lv_color_hex(0x1E1E1E)
#define COLOR_CARD          lv_color_hex(0x2D2D2D)
#define COLOR_TEXT          lv_color_hex(0xFFFFFF)
#define COLOR_TEXT_DIM      lv_color_hex(0x8E8E93)
#define COLOR_PRIMARY       lv_color_hex(0x0A84FF)
#define COLOR_SUCCESS       lv_color_hex(0x30D158)
#define COLOR_ERROR         lv_color_hex(0xFF453A)
#define COLOR_WARNING       lv_color_hex(0xFFD60A)

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

static lv_obj_t* create_card(lv_obj_t* parent, const char* title, int width, int height)
{
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, width, height);
    lv_obj_set_style_bg_color(card, COLOR_CARD, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_radius(card, 12, 0);
    lv_obj_set_style_pad_all(card, 15, 0);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);

    if (title) {
        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, title);
        lv_obj_set_style_text_color(lbl, COLOR_TEXT_DIM, 0);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 0);
    }

    return card;
}

static void apply_dark_theme(lv_obj_t* scr)
{
    lv_obj_set_style_bg_color(scr, COLOR_BG, 0);
}

/*******************************************************************************
 * Example 1: IPC Ping Test (Simulated)
 ******************************************************************************/

static lv_obj_t* ex1_status_label;
static lv_obj_t* ex1_count_label;
static lv_obj_t* ex1_time_label;
static lv_timer_t* ex1_timer;
static int ex1_ping_count = 0;
static int ex1_pong_count = 0;

static void ex1_ping_cb(lv_event_t* e)
{
    (void)e;
    ex1_ping_count++;
    lv_label_set_text(ex1_status_label, "Sending PING to CM33...");
    lv_obj_set_style_text_color(ex1_status_label, COLOR_WARNING, 0);
}

static void ex1_timer_cb(lv_timer_t* timer)
{
    (void)timer;

    /* Simulate PONG response */
    if (ex1_ping_count > ex1_pong_count) {
        ex1_pong_count++;
        lv_label_set_text(ex1_status_label, "PONG received from CM33!");
        lv_obj_set_style_text_color(ex1_status_label, COLOR_SUCCESS, 0);
        lv_label_set_text_fmt(ex1_count_label, "Ping: %d / Pong: %d", ex1_ping_count, ex1_pong_count);

        /* Simulate round-trip time */
        int rtt = 3 + (rand() % 10);
        lv_label_set_text_fmt(ex1_time_label, "Round-trip time: %d ms", rtt);
    }
}

void part4_ex1_ipc_ping(void)
{
    printf("Part 4 Example 1: IPC Ping Test (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    /* Main container */
    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex1: IPC Ping Test");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* subtitle = lv_label_create(main);
    lv_label_set_text(subtitle, "CM55 <-> CM33 Inter-Processor Communication (Simulated)");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 35);

    /* IPC Diagram */
    lv_obj_t* diagram = create_card(main, NULL, 500, 120);
    lv_obj_align(diagram, LV_ALIGN_TOP_MID, 0, 80);

    lv_obj_t* cm55_box = lv_obj_create(diagram);
    lv_obj_set_size(cm55_box, 100, 60);
    lv_obj_set_style_bg_color(cm55_box, COLOR_PRIMARY, 0);
    lv_obj_set_style_radius(cm55_box, 8, 0);
    lv_obj_align(cm55_box, LV_ALIGN_LEFT_MID, 30, 0);
    lv_obj_t* cm55_lbl = lv_label_create(cm55_box);
    lv_label_set_text(cm55_lbl, "CM55\n(LVGL)");
    lv_obj_set_style_text_align(cm55_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(cm55_lbl);

    lv_obj_t* arrow = lv_label_create(diagram);
    lv_label_set_text(arrow, LV_SYMBOL_RIGHT " PING " LV_SYMBOL_RIGHT "\n" LV_SYMBOL_LEFT " PONG " LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(arrow, COLOR_SUCCESS, 0);
    lv_obj_align(arrow, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* cm33_box = lv_obj_create(diagram);
    lv_obj_set_size(cm33_box, 100, 60);
    lv_obj_set_style_bg_color(cm33_box, COLOR_SUCCESS, 0);
    lv_obj_set_style_radius(cm33_box, 8, 0);
    lv_obj_align(cm33_box, LV_ALIGN_RIGHT_MID, -30, 0);
    lv_obj_t* cm33_lbl = lv_label_create(cm33_box);
    lv_label_set_text(cm33_lbl, "CM33\n(Driver)");
    lv_obj_set_style_text_align(cm33_lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(cm33_lbl);

    /* Status card */
    lv_obj_t* status_card = create_card(main, "IPC Status", 400, 140);
    lv_obj_align(status_card, LV_ALIGN_CENTER, 0, 50);

    ex1_status_label = lv_label_create(status_card);
    lv_label_set_text(ex1_status_label, "Ready to send PING");
    lv_obj_set_style_text_color(ex1_status_label, COLOR_PRIMARY, 0);
    lv_obj_align(ex1_status_label, LV_ALIGN_TOP_LEFT, 0, 30);

    ex1_count_label = lv_label_create(status_card);
    lv_label_set_text(ex1_count_label, "Ping: 0 / Pong: 0");
    lv_obj_set_style_text_color(ex1_count_label, COLOR_TEXT, 0);
    lv_obj_align(ex1_count_label, LV_ALIGN_TOP_LEFT, 0, 60);

    ex1_time_label = lv_label_create(status_card);
    lv_label_set_text(ex1_time_label, "Round-trip time: --- ms");
    lv_obj_set_style_text_color(ex1_time_label, COLOR_TEXT_DIM, 0);
    lv_obj_align(ex1_time_label, LV_ALIGN_TOP_LEFT, 0, 90);

    /* Ping button */
    lv_obj_t* btn = lv_button_create(main);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_set_style_bg_color(btn, COLOR_PRIMARY, 0);
    lv_obj_set_style_radius(btn, 10, 0);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_add_event_cb(btn, ex1_ping_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* btn_lbl = lv_label_create(btn);
    lv_label_set_text(btn_lbl, LV_SYMBOL_REFRESH " Send PING");
    lv_obj_center(btn_lbl);

    ex1_timer = lv_timer_create(ex1_timer_cb, 500, NULL);
}

/*******************************************************************************
 * Example 2: IPC Remote Logging
 ******************************************************************************/

static lv_obj_t* ex2_log_textarea;
static uint32_t ex2_log_count = 0;

static void ex2_log_button_cb(lv_event_t* e)
{
    int level = (int)(intptr_t)lv_event_get_user_data(e);
    ex2_log_count++;

    const char* level_names[] = {"ERROR", "WARN ", "INFO ", "DEBUG"};
    const char* level_colors[] = {"#FF453A", "#FFD60A", "#30D158", "#0A84FF"};
    const char* messages[] = {
        "Something went wrong!",
        "This is a warning message",
        "Information logged",
        "Debug details here"
    };

    char log_entry[128];
    snprintf(log_entry, sizeof(log_entry), "[%s] %s (#%u)\n",
             level_names[level], messages[level], (unsigned)ex2_log_count);

    lv_textarea_add_text(ex2_log_textarea, log_entry);

    printf("[LOG-%s] %s\n", level_names[level], messages[level]);
}

void part4_ex2_ipc_log(void)
{
    printf("Part 4 Example 2: Logging System (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex2: Logging System");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* subtitle = lv_label_create(main);
    lv_label_set_text(subtitle, "Thread-safe queued logging with levels");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 35);

    /* Log buttons */
    lv_obj_t* btn_row = lv_obj_create(main);
    lv_obj_set_size(btn_row, 500, 60);
    lv_obj_set_style_bg_opa(btn_row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(btn_row, 0, 0);
    lv_obj_set_flex_flow(btn_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(btn_row, LV_ALIGN_TOP_MID, 0, 70);

    const char* btn_labels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    lv_color_t btn_colors[] = {COLOR_ERROR, COLOR_WARNING, COLOR_SUCCESS, COLOR_PRIMARY};

    for (int i = 0; i < 4; i++) {
        lv_obj_t* btn = lv_button_create(btn_row);
        lv_obj_set_size(btn, 100, 45);
        lv_obj_set_style_bg_color(btn, btn_colors[i], 0);
        lv_obj_set_style_radius(btn, 8, 0);
        lv_obj_add_event_cb(btn, ex2_log_button_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);

        lv_obj_t* lbl = lv_label_create(btn);
        lv_label_set_text(lbl, btn_labels[i]);
        lv_obj_set_style_text_color(lbl, COLOR_TEXT, 0);
        lv_obj_center(lbl);
    }

    /* Log output area */
    lv_obj_t* log_card = create_card(main, "Log Output (Console)", 760, 280);
    lv_obj_align(log_card, LV_ALIGN_BOTTOM_MID, 0, -20);

    ex2_log_textarea = lv_textarea_create(log_card);
    lv_obj_set_size(ex2_log_textarea, 730, 220);
    lv_obj_align(ex2_log_textarea, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ex2_log_textarea, lv_color_hex(0x1A1A1A), 0);
    lv_obj_set_style_text_color(ex2_log_textarea, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(ex2_log_textarea, &lv_font_montserrat_12, 0);
    lv_textarea_set_text(ex2_log_textarea, "");
    lv_textarea_set_placeholder_text(ex2_log_textarea, "Click buttons above to generate logs...");
}

/*******************************************************************************
 * Example 3: IPC Sensor Data (Simulated)
 ******************************************************************************/

static lv_obj_t* ex3_accel_labels[3];
static lv_obj_t* ex3_gyro_labels[3];
static lv_obj_t* ex3_adc_bar;
static lv_obj_t* ex3_adc_label;
static lv_timer_t* ex3_timer;
static float ex3_sim_angle = 0;

static void ex3_timer_cb(lv_timer_t* timer)
{
    (void)timer;
    ex3_sim_angle += 0.05f;

    /* Simulate IMU data */
    float ax = sinf(ex3_sim_angle) * 9.8f;
    float ay = cosf(ex3_sim_angle * 0.7f) * 9.8f;
    float az = 9.8f + sinf(ex3_sim_angle * 0.3f) * 2.0f;

    float gx = sinf(ex3_sim_angle * 2.0f) * 250.0f;
    float gy = cosf(ex3_sim_angle * 1.5f) * 250.0f;
    float gz = sinf(ex3_sim_angle * 0.5f) * 250.0f;

    lv_label_set_text_fmt(ex3_accel_labels[0], "X: %+6.2f", (double)ax);
    lv_label_set_text_fmt(ex3_accel_labels[1], "Y: %+6.2f", (double)ay);
    lv_label_set_text_fmt(ex3_accel_labels[2], "Z: %+6.2f", (double)az);

    lv_label_set_text_fmt(ex3_gyro_labels[0], "X: %+6.1f", (double)gx);
    lv_label_set_text_fmt(ex3_gyro_labels[1], "Y: %+6.1f", (double)gy);
    lv_label_set_text_fmt(ex3_gyro_labels[2], "Z: %+6.1f", (double)gz);

    /* Simulate ADC data */
    int adc = (int)(2048 + sinf(ex3_sim_angle * 0.5f) * 2000);
    int pct = adc * 100 / 4095;
    lv_bar_set_value(ex3_adc_bar, pct, LV_ANIM_ON);
    lv_label_set_text_fmt(ex3_adc_label, "ADC: %d (%.2fV)", adc, (double)(adc * 3.3f / 4095.0f));
}

void part4_ex3_ipc_sensor(void)
{
    printf("Part 4 Example 3: IPC Sensor Data (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex3: Sensor Data via IPC");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* subtitle = lv_label_create(main);
    lv_label_set_text(subtitle, "CM33 reads sensors -> sends data to CM55 via IPC (Simulated)");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 35);

    /* Accelerometer card */
    lv_obj_t* accel_card = create_card(main, "Accelerometer (m/s²)", 240, 150);
    lv_obj_align(accel_card, LV_ALIGN_TOP_LEFT, 20, 80);

    for (int i = 0; i < 3; i++) {
        ex3_accel_labels[i] = lv_label_create(accel_card);
        lv_label_set_text(ex3_accel_labels[i], "X: +0.00");
        lv_obj_set_style_text_color(ex3_accel_labels[i], COLOR_TEXT, 0);
        lv_obj_set_style_text_font(ex3_accel_labels[i], &lv_font_montserrat_18, 0);
        lv_obj_align(ex3_accel_labels[i], LV_ALIGN_TOP_LEFT, 10, 35 + i * 30);
    }

    /* Gyroscope card */
    lv_obj_t* gyro_card = create_card(main, "Gyroscope (deg/s)", 240, 150);
    lv_obj_align(gyro_card, LV_ALIGN_TOP_MID, 0, 80);

    for (int i = 0; i < 3; i++) {
        ex3_gyro_labels[i] = lv_label_create(gyro_card);
        lv_label_set_text(ex3_gyro_labels[i], "X: +0.0");
        lv_obj_set_style_text_color(ex3_gyro_labels[i], COLOR_TEXT, 0);
        lv_obj_set_style_text_font(ex3_gyro_labels[i], &lv_font_montserrat_18, 0);
        lv_obj_align(ex3_gyro_labels[i], LV_ALIGN_TOP_LEFT, 10, 35 + i * 30);
    }

    /* ADC card */
    lv_obj_t* adc_card = create_card(main, "ADC Reading", 240, 150);
    lv_obj_align(adc_card, LV_ALIGN_TOP_RIGHT, -20, 80);

    ex3_adc_bar = lv_bar_create(adc_card);
    lv_obj_set_size(ex3_adc_bar, 200, 30);
    lv_bar_set_range(ex3_adc_bar, 0, 100);
    lv_obj_set_style_bg_color(ex3_adc_bar, COLOR_TEXT_DIM, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ex3_adc_bar, COLOR_PRIMARY, LV_PART_INDICATOR);
    lv_obj_align(ex3_adc_bar, LV_ALIGN_CENTER, 0, 10);

    ex3_adc_label = lv_label_create(adc_card);
    lv_label_set_text(ex3_adc_label, "ADC: --- (-.--V)");
    lv_obj_set_style_text_color(ex3_adc_label, COLOR_TEXT, 0);
    lv_obj_align(ex3_adc_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    /* Info */
    lv_obj_t* info = lv_label_create(main);
    lv_label_set_text(info, "In real hardware:\n"
                           "- CM33 reads BMI270 IMU and ADC\n"
                           "- Data is sent via Cy_IPC_Pipe to CM55\n"
                           "- CM55 updates the UI with received data");
    lv_obj_set_style_text_color(info, COLOR_TEXT_DIM, 0);
    lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -40);

    ex3_timer = lv_timer_create(ex3_timer_cb, 50, NULL);
}

/*******************************************************************************
 * Example 4: Event Bus Demo
 ******************************************************************************/

static lv_obj_t* ex4_event_count_label;
static lv_obj_t* ex4_event_list;
static lv_timer_t* ex4_timer;
static uint32_t ex4_event_count = 0;

static void ex4_add_event(const char* event_name, const char* data)
{
    ex4_event_count++;

    char buf[64];
    snprintf(buf, sizeof(buf), "#%u %s: %s", (unsigned)ex4_event_count, event_name, data);
    lv_textarea_add_text(ex4_event_list, buf);
    lv_textarea_add_text(ex4_event_list, "\n");

    lv_label_set_text_fmt(ex4_event_count_label, "Events processed: %u", (unsigned)ex4_event_count);
}

static void ex4_timer_cb(lv_timer_t* timer)
{
    (void)timer;
    static int counter = 0;
    counter++;

    /* Publish random events */
    int event_type = counter % 4;
    switch (event_type) {
        case 0:
            ex4_add_event("IMU_UPDATE", "ax=1.2, ay=-0.5, az=9.8");
            break;
        case 1:
            ex4_add_event("ADC_UPDATE", "ch0=2048 (1.65V)");
            break;
        case 2:
            ex4_add_event("BUTTON", "USER_BTN pressed");
            break;
        case 3:
            ex4_add_event("TIMER", "100ms tick");
            break;
    }
}

void part4_ex4_event_bus(void)
{
    printf("Part 4 Example 4: Event Bus Demo\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex4: Event Bus");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* subtitle = lv_label_create(main);
    lv_label_set_text(subtitle, "Publish-Subscribe pattern for decoupled communication");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 35);

    /* Event count */
    ex4_event_count_label = lv_label_create(main);
    lv_label_set_text(ex4_event_count_label, "Events processed: 0");
    lv_obj_set_style_text_color(ex4_event_count_label, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(ex4_event_count_label, &lv_font_montserrat_18, 0);
    lv_obj_align(ex4_event_count_label, LV_ALIGN_TOP_MID, 0, 70);

    /* Event list */
    lv_obj_t* card = create_card(main, "Event Stream", 760, 320);
    lv_obj_align(card, LV_ALIGN_BOTTOM_MID, 0, -20);

    ex4_event_list = lv_textarea_create(card);
    lv_obj_set_size(ex4_event_list, 730, 260);
    lv_obj_align(ex4_event_list, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(ex4_event_list, lv_color_hex(0x1A1A1A), 0);
    lv_obj_set_style_text_color(ex4_event_list, COLOR_SUCCESS, 0);
    lv_obj_set_style_text_font(ex4_event_list, &lv_font_montserrat_12, 0);
    lv_textarea_set_text(ex4_event_list, "");

    ex4_timer = lv_timer_create(ex4_timer_cb, 500, NULL);
}

/*******************************************************************************
 * Example 5: HW IPC LED Control (Simulated)
 ******************************************************************************/

static lv_obj_t* ex5_led;
static lv_obj_t* ex5_brightness_slider;
static lv_obj_t* ex5_status_label;
static bool ex5_led_on = false;

static void ex5_toggle_cb(lv_event_t* e)
{
    (void)e;
    ex5_led_on = !ex5_led_on;

    if (ex5_led_on) {
        lv_led_on(ex5_led);
        lv_label_set_text(ex5_status_label, "LED ON (IPC: LED_ON sent to CM33)");
        lv_obj_set_style_text_color(ex5_status_label, COLOR_SUCCESS, 0);
    } else {
        lv_led_off(ex5_led);
        lv_label_set_text(ex5_status_label, "LED OFF (IPC: LED_OFF sent to CM33)");
        lv_obj_set_style_text_color(ex5_status_label, COLOR_ERROR, 0);
    }
}

static void ex5_brightness_cb(lv_event_t* e)
{
    lv_obj_t* slider = lv_event_get_target(e);
    int val = lv_slider_get_value(slider);
    lv_led_set_brightness(ex5_led, val * 255 / 100);

    char buf[64];
    snprintf(buf, sizeof(buf), "Brightness: %d%% (IPC: PWM=%d)", val, val * 255 / 100);
    lv_label_set_text(ex5_status_label, buf);
}

void part4_ex5_hw_ipc_led(void)
{
    printf("Part 4 Example 5: HW IPC LED Control (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex5: LED Control via IPC");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* LED display */
    ex5_led = lv_led_create(main);
    lv_obj_set_size(ex5_led, 100, 100);
    lv_led_set_color(ex5_led, COLOR_PRIMARY);
    lv_led_off(ex5_led);
    lv_obj_align(ex5_led, LV_ALIGN_CENTER, 0, -50);

    /* Toggle button */
    lv_obj_t* btn = lv_button_create(main);
    lv_obj_set_size(btn, 150, 50);
    lv_obj_set_style_bg_color(btn, COLOR_PRIMARY, 0);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(btn, ex5_toggle_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* btn_lbl = lv_label_create(btn);
    lv_label_set_text(btn_lbl, "Toggle LED");
    lv_obj_center(btn_lbl);

    /* Brightness slider */
    lv_obj_t* slider_lbl = lv_label_create(main);
    lv_label_set_text(slider_lbl, "Brightness:");
    lv_obj_set_style_text_color(slider_lbl, COLOR_TEXT, 0);
    lv_obj_align(slider_lbl, LV_ALIGN_CENTER, -150, 110);

    ex5_brightness_slider = lv_slider_create(main);
    lv_obj_set_width(ex5_brightness_slider, 300);
    lv_slider_set_range(ex5_brightness_slider, 0, 100);
    lv_slider_set_value(ex5_brightness_slider, 100, LV_ANIM_OFF);
    lv_obj_align(ex5_brightness_slider, LV_ALIGN_CENTER, 50, 110);
    lv_obj_add_event_cb(ex5_brightness_slider, ex5_brightness_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Status */
    ex5_status_label = lv_label_create(main);
    lv_label_set_text(ex5_status_label, "LED OFF");
    lv_obj_set_style_text_color(ex5_status_label, COLOR_TEXT_DIM, 0);
    lv_obj_align(ex5_status_label, LV_ALIGN_BOTTOM_MID, 0, -60);
}

/*******************************************************************************
 * Example 6: HW IPC Button Events (Simulated)
 ******************************************************************************/

static lv_obj_t* ex6_btn_indicators[3];
static lv_obj_t* ex6_event_label;
static lv_timer_t* ex6_timer;

static void ex6_timer_cb(lv_timer_t* timer)
{
    (void)timer;
    static int sim_counter = 0;
    sim_counter++;

    /* Simulate random button presses */
    if (sim_counter % 20 == 0) {
        int btn_idx = rand() % 3;
        const char* btn_names[] = {"USER_BTN", "BTN_A", "BTN_B"};

        /* Flash the indicator */
        lv_led_on(ex6_btn_indicators[btn_idx]);

        char buf[64];
        snprintf(buf, sizeof(buf), "IPC Event: %s pressed!", btn_names[btn_idx]);
        lv_label_set_text(ex6_event_label, buf);
    } else if (sim_counter % 20 == 5) {
        /* Turn off all indicators */
        for (int i = 0; i < 3; i++) {
            lv_led_off(ex6_btn_indicators[i]);
        }
    }
}

void part4_ex6_hw_ipc_button(void)
{
    printf("Part 4 Example 6: HW IPC Button Events (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex6: Button Events via IPC");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* subtitle = lv_label_create(main);
    lv_label_set_text(subtitle, "CM33 detects button -> sends event to CM55 via IPC");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 35);

    /* Button indicators */
    const char* btn_names[] = {"USER_BTN", "BTN_A", "BTN_B"};
    lv_color_t btn_colors[] = {COLOR_PRIMARY, COLOR_SUCCESS, COLOR_WARNING};

    for (int i = 0; i < 3; i++) {
        lv_obj_t* card = create_card(main, btn_names[i], 200, 150);
        lv_obj_align(card, LV_ALIGN_CENTER, (i - 1) * 220, -30);

        ex6_btn_indicators[i] = lv_led_create(card);
        lv_obj_set_size(ex6_btn_indicators[i], 60, 60);
        lv_led_set_color(ex6_btn_indicators[i], btn_colors[i]);
        lv_led_off(ex6_btn_indicators[i]);
        lv_obj_align(ex6_btn_indicators[i], LV_ALIGN_CENTER, 0, 15);
    }

    /* Event label */
    ex6_event_label = lv_label_create(main);
    lv_label_set_text(ex6_event_label, "Waiting for button events...");
    lv_obj_set_style_text_color(ex6_event_label, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(ex6_event_label, &lv_font_montserrat_18, 0);
    lv_obj_align(ex6_event_label, LV_ALIGN_BOTTOM_MID, 0, -80);

    ex6_timer = lv_timer_create(ex6_timer_cb, 100, NULL);
}

/*******************************************************************************
 * Example 7: HW IPC Dashboard (Simulated)
 ******************************************************************************/

void part4_ex7_hw_ipc_dashboard(void)
{
    printf("Part 4 Example 7: HW IPC Dashboard (Simulated)\n");
    /* Reuse Example 3 which already shows a comprehensive dashboard */
    part4_ex3_ipc_sensor();
}

/*******************************************************************************
 * Example 8: Advanced Features
 ******************************************************************************/

void part4_ex8_advanced(void)
{
    printf("Part 4 Example 8: Advanced Features (Simulated)\n");

    lv_obj_t* scr = lv_screen_active();
    apply_dark_theme(scr);

    lv_obj_t* main = lv_obj_create(scr);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_set_scrollbar_mode(main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "Part 4 Ex8: Advanced Features");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* Feature cards */
    const char* features[] = {
        "IPC\nCommunication",
        "Event\nBus",
        "Logging\nSystem",
        "Layout\nHelpers"
    };

    const char* descriptions[] = {
        "Cy_IPC_Pipe\nCM55 <-> CM33",
        "Pub-Sub\nDecoupled",
        "Thread-safe\nQueued",
        "Flexbox\nDark Theme"
    };

    lv_color_t colors[] = {COLOR_PRIMARY, COLOR_SUCCESS, COLOR_WARNING, COLOR_ERROR};

    for (int i = 0; i < 4; i++) {
        lv_obj_t* card = create_card(main, NULL, 170, 180);
        lv_obj_align(card, LV_ALIGN_CENTER, (i - 1.5) * 185, 30);

        lv_obj_t* icon = lv_obj_create(card);
        lv_obj_set_size(icon, 60, 60);
        lv_obj_set_style_bg_color(icon, colors[i], 0);
        lv_obj_set_style_radius(icon, LV_RADIUS_CIRCLE, 0);
        lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 10);

        lv_obj_t* feat = lv_label_create(card);
        lv_label_set_text(feat, features[i]);
        lv_obj_set_style_text_color(feat, COLOR_TEXT, 0);
        lv_obj_set_style_text_align(feat, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(feat, LV_ALIGN_CENTER, 0, 20);

        lv_obj_t* desc = lv_label_create(card);
        lv_label_set_text(desc, descriptions[i]);
        lv_obj_set_style_text_color(desc, COLOR_TEXT_DIM, 0);
        lv_obj_set_style_text_font(desc, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(desc, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(desc, LV_ALIGN_BOTTOM_MID, 0, -10);
    }

    /* Footer */
    lv_obj_t* footer = lv_label_create(main);
    lv_label_set_text(footer, "Part 4 covers the infrastructure for real-time embedded UI applications");
    lv_obj_set_style_text_color(footer, COLOR_TEXT_DIM, 0);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/*******************************************************************************
 * Example 9: CAPSENSE via IPC (PC Simulation)
 *
 * Simulates CAPSENSE interaction:
 * - Slider (CSS1) is DRAGGABLE by mouse (PC interactive)
 * - Button 0 (CSB1) is CLICKABLE -> Red LED
 * - Button 1 (CSB2) is CLICKABLE -> Green LED
 * - LED brightness follows slider position
 * - IPC event counter increments on state changes
 ******************************************************************************/

#define EX9_NUM_BUTTONS 2

/* Ex9 UI objects */
static lv_obj_t* ex9_slider;
static lv_obj_t* ex9_slider_value;
static lv_obj_t* ex9_output_led;
static lv_obj_t* ex9_btn_panels[EX9_NUM_BUTTONS];
static lv_obj_t* ex9_btn_leds[EX9_NUM_BUTTONS];
static lv_obj_t* ex9_btn_status[EX9_NUM_BUTTONS];
static lv_obj_t* ex9_ipc_count_label;

/* Ex9 state */
static uint8_t ex9_btn_state[EX9_NUM_BUTTONS];
static uint32_t ex9_ipc_event_count;

/* Slider change callback (user drags slider) */
static void ex9_slider_cb(lv_event_t* e)
{
    lv_obj_t* slider = lv_event_get_target(e);
    int32_t val = lv_slider_get_value(slider);

    lv_label_set_text_fmt(ex9_slider_value, "%d%%", (int)val);

    if (val > 0) {
        uint8_t brightness = (uint8_t)(val * 255 / 100);
        lv_led_on(ex9_output_led);
        lv_led_set_brightness(ex9_output_led, brightness);
    } else {
        lv_led_off(ex9_output_led);
    }

    ex9_ipc_event_count++;
    lv_label_set_text_fmt(ex9_ipc_count_label,
        "Ex9: CAPSENSE (IPC)    IPC events: %u",
        (unsigned)ex9_ipc_event_count);
}

/* Button click callback (user clicks button panel) */
static void ex9_btn_click_cb(lv_event_t* e)
{
    int idx = (int)(intptr_t)lv_event_get_user_data(e);
    if (idx < 0 || idx >= EX9_NUM_BUTTONS) return;

    /* Toggle button state */
    ex9_btn_state[idx] = !ex9_btn_state[idx];

    if (ex9_btn_state[idx]) {
        lv_obj_set_style_bg_color(ex9_btn_panels[idx], lv_color_hex(0x00AA00), 0);
        lv_led_on(ex9_btn_leds[idx]);
        lv_label_set_text(ex9_btn_status[idx], "TOUCHED");
        lv_obj_set_style_text_color(ex9_btn_status[idx], COLOR_SUCCESS, 0);
    } else {
        lv_obj_set_style_bg_color(ex9_btn_panels[idx], lv_color_hex(0x333355), 0);
        lv_led_off(ex9_btn_leds[idx]);
        lv_label_set_text(ex9_btn_status[idx], "Ready");
        lv_obj_set_style_text_color(ex9_btn_status[idx], COLOR_TEXT_DIM, 0);
    }

    ex9_ipc_event_count++;
    lv_label_set_text_fmt(ex9_ipc_count_label,
        "Ex9: CAPSENSE (IPC)    IPC events: %u",
        (unsigned)ex9_ipc_event_count);
}

void part4_ex9_capsense_ipc(void)
{
    ex9_ipc_event_count = 0;
    memset(ex9_btn_state, 0, sizeof(ex9_btn_state));

    lv_obj_t* scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x1a1a2e), 0);

    /* ===== TITLE ===== */
    lv_obj_t* title = lv_label_create(scr);
    lv_label_set_text(title, "Part 4 Ex9: CAPSENSE (IPC)");
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    /* ===== MODE LABEL ===== */
    lv_obj_t* mode_label = lv_label_create(scr);
    lv_label_set_text(mode_label, "Mode: Simulated (PC)");
    lv_obj_set_style_text_color(mode_label, COLOR_PRIMARY, 0);
    lv_obj_align(mode_label, LV_ALIGN_TOP_MID, 0, 32);

    /* ===== CAPSENSE SLIDER (TOP) - Same layout as hardware ===== */
    lv_obj_t* slider_panel = lv_obj_create(scr);
    lv_obj_set_size(slider_panel, 420, 80);
    lv_obj_align(slider_panel, LV_ALIGN_TOP_MID, 0, 55);
    lv_obj_set_style_bg_color(slider_panel, lv_color_hex(0x0f0f23), 0);
    lv_obj_set_style_pad_all(slider_panel, 8, 0);
    lv_obj_set_style_border_width(slider_panel, 0, 0);
    lv_obj_clear_flag(slider_panel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* slider_title = lv_label_create(slider_panel);
    lv_label_set_text(slider_title, "SLIDER (CSS1)");
    lv_obj_set_style_text_color(slider_title, COLOR_TEXT, 0);
    lv_obj_align(slider_title, LV_ALIGN_TOP_LEFT, 10, 0);

    ex9_slider_value = lv_label_create(slider_panel);
    lv_label_set_text(ex9_slider_value, "0%");
    lv_obj_set_style_text_color(ex9_slider_value, COLOR_PRIMARY, 0);
    lv_obj_set_style_text_font(ex9_slider_value, &lv_font_montserrat_16, 0);
    lv_obj_align(ex9_slider_value, LV_ALIGN_TOP_RIGHT, -10, 0);

    /* Slider - INTERACTIVE on PC (draggable by mouse) */
    ex9_slider = lv_slider_create(slider_panel);
    lv_obj_set_width(ex9_slider, 340);
    lv_obj_set_height(ex9_slider, 25);
    lv_obj_align(ex9_slider, LV_ALIGN_BOTTOM_MID, 0, -8);
    lv_slider_set_range(ex9_slider, 0, 100);
    lv_slider_set_value(ex9_slider, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ex9_slider, lv_color_hex(0x333355), LV_PART_MAIN);
    lv_obj_set_style_bg_color(ex9_slider, COLOR_PRIMARY, LV_PART_INDICATOR);
    lv_obj_add_event_cb(ex9_slider, ex9_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Output LED next to slider */
    ex9_output_led = lv_led_create(slider_panel);
    lv_obj_set_size(ex9_output_led, 25, 25);
    lv_obj_align(ex9_output_led, LV_ALIGN_BOTTOM_RIGHT, -5, -8);
    lv_led_set_color(ex9_output_led, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
    lv_led_off(ex9_output_led);

    /* ===== CAPSENSE BUTTONS (BOTTOM) - BTN0 left, BTN1 right ===== */
    const char* btn_names[] = {"BTN0", "BTN1"};
    const char* btn_ids[] = {"(CSB1)", "(CSB2)"};
    uint32_t led_colors[] = {0xFF0000, 0x00FF00};
    int btn_x_pos[] = {-110, 110};

    for (int i = 0; i < EX9_NUM_BUTTONS; i++) {
        ex9_btn_panels[i] = lv_obj_create(scr);
        lv_obj_set_size(ex9_btn_panels[i], 140, 150);
        lv_obj_align(ex9_btn_panels[i], LV_ALIGN_BOTTOM_MID, btn_x_pos[i], -55);
        lv_obj_set_style_bg_color(ex9_btn_panels[i], lv_color_hex(0x333355), 0);
        lv_obj_set_style_border_width(ex9_btn_panels[i], 3, 0);
        lv_obj_set_style_border_color(ex9_btn_panels[i], lv_color_hex(0x666699), 0);
        lv_obj_set_style_radius(ex9_btn_panels[i], 10, 0);
        lv_obj_set_style_pad_all(ex9_btn_panels[i], 5, 0);
        lv_obj_clear_flag(ex9_btn_panels[i], LV_OBJ_FLAG_SCROLLABLE);

        /* Make button panel clickable */
        lv_obj_add_flag(ex9_btn_panels[i], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(ex9_btn_panels[i], ex9_btn_click_cb,
                            LV_EVENT_CLICKED, (void*)(intptr_t)i);

        lv_obj_t* name = lv_label_create(ex9_btn_panels[i]);
        lv_label_set_text(name, btn_names[i]);
        lv_obj_set_style_text_color(name, COLOR_TEXT, 0);
        lv_obj_set_style_text_font(name, &lv_font_montserrat_16, 0);
        lv_obj_align(name, LV_ALIGN_TOP_MID, 0, 2);

        lv_obj_t* id_lbl = lv_label_create(ex9_btn_panels[i]);
        lv_label_set_text(id_lbl, btn_ids[i]);
        lv_obj_set_style_text_color(id_lbl, COLOR_TEXT_DIM, 0);
        lv_obj_align(id_lbl, LV_ALIGN_TOP_MID, 0, 22);

        ex9_btn_leds[i] = lv_led_create(ex9_btn_panels[i]);
        lv_obj_set_size(ex9_btn_leds[i], 50, 50);
        lv_obj_align(ex9_btn_leds[i], LV_ALIGN_CENTER, 0, 8);
        lv_led_set_color(ex9_btn_leds[i], lv_color_hex(led_colors[i]));
        lv_led_off(ex9_btn_leds[i]);

        ex9_btn_status[i] = lv_label_create(ex9_btn_panels[i]);
        lv_label_set_text(ex9_btn_status[i], "Ready");
        lv_obj_set_style_text_color(ex9_btn_status[i], COLOR_TEXT_DIM, 0);
        lv_obj_align(ex9_btn_status[i], LV_ALIGN_BOTTOM_MID, 0, -2);
    }

    /* ===== FOOTER STATUS ===== */
    ex9_ipc_count_label = lv_label_create(scr);
    lv_label_set_text(ex9_ipc_count_label, "Ex9: CAPSENSE (IPC)    IPC events: 0");
    lv_obj_set_style_text_color(ex9_ipc_count_label, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_align(ex9_ipc_count_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(ex9_ipc_count_label, LV_ALIGN_BOTTOM_MID, 0, -25);

    /* Copyright footer */
    lv_obj_t* copy = lv_label_create(scr);
    lv_label_set_text(copy, "(C) 2025 BiiL - Embedded C for IoT");
    lv_obj_set_style_text_color(copy, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(copy, &lv_font_montserrat_12, 0);
    lv_obj_align(copy, LV_ALIGN_BOTTOM_MID, 0, -6);

    printf("[Week6-SIM] Ex9: CAPSENSE via IPC (Simulated)\n");
    printf("  - Drag slider to simulate CSS1\n");
    printf("  - Click buttons to simulate CSB1/CSB2\n");
}
