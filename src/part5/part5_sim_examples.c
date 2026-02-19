/*******************************************************************************
 * File: part5_sim_examples.c
 * Description: Part 5 WiFi Manager - PC Simulator with Mock Data
 *
 * This file provides a complete WiFi Manager UI with simulated network data
 * for testing on PC without actual hardware.
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#include "part5_examples.h"
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*******************************************************************************
 * WiFi Shared Definitions (Embedded for Simulator)
 ******************************************************************************/

#define WIFI_SSID_MAX_LEN       33
#define WIFI_SCAN_MAX_NETWORKS  16

typedef enum {
    WIFI_SECURITY_OPEN = 0,
    WIFI_SECURITY_WEP = 1,
    WIFI_SECURITY_WPA2 = 3,
    WIFI_SECURITY_WPA3 = 4,
} wifi_security_t;

typedef enum {
    WIFI_BAND_2_4GHZ = 0,
    WIFI_BAND_5GHZ = 1,
} wifi_band_t;

typedef enum {
    WIFI_STATE_DISCONNECTED = 0,
    WIFI_STATE_CONNECTING = 1,
    WIFI_STATE_CONNECTED = 2,
} wifi_state_t;

typedef struct {
    char ssid[WIFI_SSID_MAX_LEN];
    int8_t rssi;
    uint8_t security;
    uint8_t channel;
    uint8_t band;
    uint8_t flags;  /* bit0=connected */
} sim_wifi_network_t;

/*******************************************************************************
 * Color Palette (macOS Dark Theme)
 ******************************************************************************/

#define COLOR_BG            lv_color_hex(0x1E1E1E)
#define COLOR_SIDEBAR       lv_color_hex(0x2D2D2D)
#define COLOR_CARD          lv_color_hex(0x3A3A3A)
#define COLOR_HIGHLIGHT     lv_color_hex(0x0A84FF)
#define COLOR_TEXT          lv_color_hex(0xFFFFFF)
#define COLOR_TEXT_DIM      lv_color_hex(0x8E8E93)
#define COLOR_SUCCESS       lv_color_hex(0x30D158)
#define COLOR_ERROR         lv_color_hex(0xFF453A)
#define COLOR_WARNING       lv_color_hex(0xFFD60A)

/*******************************************************************************
 * Mock WiFi Data
 ******************************************************************************/

static sim_wifi_network_t mock_networks[] = {
    {"BiiL-Office-5G",      -45, WIFI_SECURITY_WPA2, 36, WIFI_BAND_5GHZ, 0x01},  /* Connected */
    {"HomeNetwork",         -52, WIFI_SECURITY_WPA2, 6,  WIFI_BAND_2_4GHZ, 0x00},
    {"iPhone (Somchai)",    -58, WIFI_SECURITY_WPA2, 1,  WIFI_BAND_2_4GHZ, 0x00},
    {"Starbucks_WiFi",      -65, WIFI_SECURITY_OPEN, 11, WIFI_BAND_2_4GHZ, 0x00},
    {"AIS_Fibre_5G",        -68, WIFI_SECURITY_WPA3, 44, WIFI_BAND_5GHZ, 0x00},
    {"TRUE_WIFI_FREE",      -72, WIFI_SECURITY_OPEN, 6,  WIFI_BAND_2_4GHZ, 0x00},
    {"Guest_Network",       -75, WIFI_SECURITY_WPA2, 1,  WIFI_BAND_2_4GHZ, 0x00},
    {"Hidden_5G",           -78, WIFI_SECURITY_WPA2, 149, WIFI_BAND_5GHZ, 0x00},
};
static int mock_network_count = 8;
static int connected_idx = 0;

/* Mock TCP/IP Info */
static uint8_t mock_ip[] = {192, 168, 1, 105};
static uint8_t mock_subnet[] = {255, 255, 255, 0};
static uint8_t mock_gateway[] = {192, 168, 1, 1};
static uint8_t mock_dns[] = {8, 8, 8, 8};
static uint8_t mock_mac[] = {0xA4, 0xCF, 0x12, 0x5A, 0x3B, 0x7C};

/*******************************************************************************
 * UI State
 ******************************************************************************/

static int g_selected_idx = -1;
static wifi_state_t g_wifi_state = WIFI_STATE_CONNECTED;
static lv_obj_t* g_network_list = NULL;
static lv_obj_t* g_details_ssid = NULL;
static lv_obj_t* g_details_status = NULL;
static lv_obj_t* g_connect_btn = NULL;
static lv_obj_t* g_status_label = NULL;
static lv_timer_t* g_update_timer = NULL;

/* Password Dialog State */
static lv_obj_t* g_pwd_dialog = NULL;
static lv_obj_t* g_pwd_textarea = NULL;
static lv_obj_t* g_pwd_keyboard = NULL;
static int g_pending_connect_idx = -1;

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

static const char* security_to_str(uint8_t sec)
{
    switch (sec) {
        case WIFI_SECURITY_OPEN: return "Open";
        case WIFI_SECURITY_WEP:  return "WEP";
        case WIFI_SECURITY_WPA2: return "WPA2";
        case WIFI_SECURITY_WPA3: return "WPA3";
        default: return "Unknown";
    }
}

static const char* band_to_str(uint8_t band)
{
    return (band == WIFI_BAND_5GHZ) ? "5 GHz" : "2.4 GHz";
}

static lv_color_t rssi_to_color(int8_t rssi)
{
    if (rssi >= -50) return lv_color_hex(0x30D158);  /* Excellent - Green */
    if (rssi >= -60) return lv_color_hex(0x63DA38);  /* Good - Light Green */
    if (rssi >= -70) return lv_color_hex(0xFFD60A);  /* Fair - Yellow */
    if (rssi >= -80) return lv_color_hex(0xFF9F0A);  /* Weak - Orange */
    return lv_color_hex(0xFF453A);                    /* Poor - Red */
}

static int rssi_to_bars(int8_t rssi)
{
    if (rssi >= -50) return 4;
    if (rssi >= -60) return 3;
    if (rssi >= -70) return 2;
    if (rssi >= -80) return 1;
    return 0;
}

/*******************************************************************************
 * Footer Label Helper
 ******************************************************************************/

static void sim_create_footer(lv_obj_t* parent, const char* text)
{
    lv_obj_t* footer = lv_label_create(parent);
    lv_label_set_text(footer, text);
    lv_obj_set_style_text_font(footer, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(footer, lv_color_hex(0x555555), 0);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_LEFT, 8, -4);
    lv_obj_remove_flag(footer, LV_OBJ_FLAG_CLICKABLE);
}

/*******************************************************************************
 * NTP Clock Mock (system time)
 ******************************************************************************/

static lv_obj_t* g_clock_label = NULL;

static void clock_timer_cb(lv_timer_t* timer)
{
    (void)timer;
    if (!g_clock_label) return;
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%a %d %b %H:%M", t);
    lv_label_set_text(g_clock_label, buf);
}

static void sim_create_clock(lv_obj_t* parent)
{
    g_clock_label = lv_label_create(parent);
    lv_obj_set_style_text_color(g_clock_label, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_font(g_clock_label, &lv_font_montserrat_12, 0);
    lv_obj_align(g_clock_label, LV_ALIGN_TOP_RIGHT, -10, 8);
    lv_obj_remove_flag(g_clock_label, LV_OBJ_FLAG_CLICKABLE);
    clock_timer_cb(NULL);  /* Set initial time */
    lv_timer_create(clock_timer_cb, 60000, NULL);  /* Update every 60s */
}

/*******************************************************************************
 * Signal Bars Widget
 ******************************************************************************/

static lv_obj_t* create_signal_bars(lv_obj_t* parent, int8_t rssi)
{
    int bars = rssi_to_bars(rssi);
    lv_color_t color = rssi_to_color(rssi);

    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, 32, 24);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_pad_column(container, 2, 0);  /* Gap between bars */
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < 4; i++) {
        lv_obj_t* bar = lv_obj_create(container);
        int h = 6 + i * 5;  /* Heights: 6, 11, 16, 21 */
        lv_obj_set_size(bar, 5, h);
        lv_obj_set_style_radius(bar, 1, 0);
        lv_obj_set_style_border_width(bar, 0, 0);
        lv_obj_set_style_pad_all(bar, 0, 0);
        lv_obj_set_scrollbar_mode(bar, LV_SCROLLBAR_MODE_OFF);
        lv_obj_remove_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_color(bar, (i < bars) ? color : lv_color_hex(0x555555), 0);
    }

    return container;
}

/*******************************************************************************
 * WiFi Icon Widget (iOS Style)
 ******************************************************************************/

static lv_obj_t* create_wifi_icon_large(lv_obj_t* parent)
{
    /* Create container for custom WiFi icon */
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, 100, 80);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_color_t wifi_blue = lv_color_hex(0x007AFF);

    /* Draw WiFi arcs (3 arcs + dot) */
    /* Arc 3 (largest) */
    lv_obj_t* arc3 = lv_arc_create(container);
    lv_obj_set_size(arc3, 90, 90);
    lv_arc_set_angles(arc3, 225, 315);
    lv_arc_set_bg_angles(arc3, 225, 315);
    lv_obj_set_style_arc_width(arc3, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc3, wifi_blue, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc3, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_remove_style(arc3, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(arc3, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_value(arc3, 100);
    lv_obj_align(arc3, LV_ALIGN_CENTER, 0, 15);

    /* Arc 2 (medium) */
    lv_obj_t* arc2 = lv_arc_create(container);
    lv_obj_set_size(arc2, 60, 60);
    lv_arc_set_angles(arc2, 225, 315);
    lv_arc_set_bg_angles(arc2, 225, 315);
    lv_obj_set_style_arc_width(arc2, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc2, wifi_blue, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc2, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_remove_style(arc2, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(arc2, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_value(arc2, 100);
    lv_obj_align(arc2, LV_ALIGN_CENTER, 0, 30);

    /* Arc 1 (smallest) */
    lv_obj_t* arc1 = lv_arc_create(container);
    lv_obj_set_size(arc1, 30, 30);
    lv_arc_set_angles(arc1, 225, 315);
    lv_arc_set_bg_angles(arc1, 225, 315);
    lv_obj_set_style_arc_width(arc1, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc1, wifi_blue, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_remove_style(arc1, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(arc1, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_value(arc1, 100);
    lv_obj_align(arc1, LV_ALIGN_CENTER, 0, 45);

    /* Dot at bottom */
    lv_obj_t* dot = lv_obj_create(container);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, wifi_blue, 0);
    lv_obj_set_style_border_width(dot, 0, 0);
    lv_obj_set_scrollbar_mode(dot, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(dot, LV_ALIGN_CENTER, 0, 60);

    return container;
}

/*******************************************************************************
 * Password Dialog Functions (iOS Style)
 ******************************************************************************/

static void pwd_dialog_close(void);
static void do_wifi_connect(int idx);

static void pwd_cancel_cb(lv_event_t* e)
{
    (void)e;
    pwd_dialog_close();
}

static void pwd_confirm_cb(lv_event_t* e)
{
    (void)e;
    /* Get password (in real implementation, use this) */
    const char* pwd = lv_textarea_get_text(g_pwd_textarea);
    printf("WiFi Password entered: %s\n", pwd);

    /* Close dialog and connect */
    int idx = g_pending_connect_idx;
    pwd_dialog_close();

    if (idx >= 0) {
        do_wifi_connect(idx);
    }
}

static void pwd_textarea_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_FOCUSED) {
        if (g_pwd_keyboard) {
            lv_keyboard_set_textarea(g_pwd_keyboard, g_pwd_textarea);
            lv_obj_remove_flag(g_pwd_keyboard, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void pwd_keyboard_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_READY) {
        /* Enter pressed - same as confirm */
        pwd_confirm_cb(NULL);
    } else if (code == LV_EVENT_CANCEL) {
        /* ESC pressed - same as cancel */
        pwd_cancel_cb(NULL);
    }
}

static void pwd_dialog_close(void)
{
    if (g_pwd_dialog) {
        lv_obj_delete(g_pwd_dialog);
        g_pwd_dialog = NULL;
        g_pwd_textarea = NULL;
        g_pwd_keyboard = NULL;
    }
    g_pending_connect_idx = -1;
}

static void show_password_dialog(int network_idx)
{
    if (network_idx < 0 || network_idx >= mock_network_count) return;

    sim_wifi_network_t* net = &mock_networks[network_idx];
    g_pending_connect_idx = network_idx;

    /* Create modal overlay */
    g_pwd_dialog = lv_obj_create(lv_screen_active());
    lv_obj_set_size(g_pwd_dialog, 800, 480);
    lv_obj_set_style_bg_color(g_pwd_dialog, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(g_pwd_dialog, LV_OPA_90, 0);
    lv_obj_set_style_border_width(g_pwd_dialog, 0, 0);
    lv_obj_set_style_radius(g_pwd_dialog, 0, 0);
    lv_obj_set_scrollbar_mode(g_pwd_dialog, LV_SCROLLBAR_MODE_OFF);
    lv_obj_center(g_pwd_dialog);

    /* Cancel button (X) - top left */
    lv_obj_t* cancel_btn = lv_button_create(g_pwd_dialog);
    lv_obj_set_size(cancel_btn, 50, 50);
    lv_obj_set_style_bg_color(cancel_btn, lv_color_hex(0x3A3A3C), 0);
    lv_obj_set_style_radius(cancel_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(cancel_btn, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_add_event_cb(cancel_btn, pwd_cancel_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* cancel_lbl = lv_label_create(cancel_btn);
    lv_label_set_text(cancel_lbl, LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_font(cancel_lbl, &lv_font_montserrat_18, 0);
    lv_obj_center(cancel_lbl);

    /* Confirm button (checkmark) - top right */
    lv_obj_t* confirm_btn = lv_button_create(g_pwd_dialog);
    lv_obj_set_size(confirm_btn, 50, 50);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x3A3A3C), 0);
    lv_obj_set_style_radius(confirm_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_RIGHT, -20, 20);
    lv_obj_add_event_cb(confirm_btn, pwd_confirm_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* confirm_lbl = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_lbl, LV_SYMBOL_OK);
    lv_obj_set_style_text_font(confirm_lbl, &lv_font_montserrat_18, 0);
    lv_obj_center(confirm_lbl);

    /* WiFi Icon (iOS style) */
    lv_obj_t* wifi_icon = create_wifi_icon_large(g_pwd_dialog);
    lv_obj_align(wifi_icon, LV_ALIGN_TOP_MID, 0, 60);

    /* Title: "Joining..." */
    lv_obj_t* title = lv_label_create(g_pwd_dialog);
    char title_buf[64];
    snprintf(title_buf, sizeof(title_buf), "Joining \"%s\"...", net->ssid);
    lv_label_set_text(title, title_buf);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 160);

    /* Subtitle */
    lv_obj_t* subtitle = lv_label_create(g_pwd_dialog);
    lv_label_set_text(subtitle, "Enter the password to join this\nWi-Fi network.");
    lv_obj_set_style_text_color(subtitle, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_text_align(subtitle, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(subtitle, LV_ALIGN_TOP_MID, 0, 195);

    /* Password input area */
    g_pwd_textarea = lv_textarea_create(g_pwd_dialog);
    lv_obj_set_size(g_pwd_textarea, 350, 50);
    lv_textarea_set_placeholder_text(g_pwd_textarea, "Password");
    lv_textarea_set_password_mode(g_pwd_textarea, true);
    lv_textarea_set_one_line(g_pwd_textarea, true);
    lv_obj_set_style_bg_color(g_pwd_textarea, lv_color_hex(0x3A3A3C), 0);
    lv_obj_set_style_border_width(g_pwd_textarea, 0, 0);
    lv_obj_set_style_radius(g_pwd_textarea, 12, 0);
    lv_obj_set_style_text_color(g_pwd_textarea, COLOR_TEXT, 0);
    lv_obj_set_style_pad_all(g_pwd_textarea, 12, 0);
    lv_obj_align(g_pwd_textarea, LV_ALIGN_TOP_MID, 0, 260);
    lv_obj_add_event_cb(g_pwd_textarea, pwd_textarea_cb, LV_EVENT_FOCUSED, NULL);

    /* Note text */
    lv_obj_t* note = lv_label_create(g_pwd_dialog);
    lv_label_set_text(note, "You can also connect by selecting the network\nfrom the list on the left.");
    lv_obj_set_style_text_color(note, lv_color_hex(0x8E8E93), 0);
    lv_obj_set_style_text_font(note, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(note, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(note, LV_ALIGN_TOP_MID, 0, 320);

    /* Keyboard */
    g_pwd_keyboard = lv_keyboard_create(g_pwd_dialog);
    lv_obj_set_size(g_pwd_keyboard, 780, 180);
    lv_keyboard_set_textarea(g_pwd_keyboard, g_pwd_textarea);
    lv_obj_set_style_bg_color(g_pwd_keyboard, lv_color_hex(0x2C2C2E), 0);
    lv_obj_align(g_pwd_keyboard, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(g_pwd_keyboard, pwd_keyboard_cb, LV_EVENT_ALL, NULL);

    /* Auto-focus on textarea */
    lv_obj_add_state(g_pwd_textarea, LV_STATE_FOCUSED);
}

/*******************************************************************************
 * WiFi Connect Helper
 ******************************************************************************/

static void do_wifi_connect(int idx)
{
    if (idx < 0 || idx >= mock_network_count) return;

    sim_wifi_network_t* net = &mock_networks[idx];

    /* Disconnect previous */
    if (connected_idx >= 0) {
        mock_networks[connected_idx].flags &= ~0x01;
    }

    /* Connect to new */
    net->flags |= 0x01;
    connected_idx = idx;
    g_wifi_state = WIFI_STATE_CONNECTED;
    g_selected_idx = idx;

    /* Update UI */
    if (g_details_ssid) {
        lv_label_set_text(g_details_ssid, net->ssid);
    }

    if (g_details_status) {
        lv_label_set_text(g_details_status, "Connected");
        lv_obj_set_style_text_color(g_details_status, COLOR_SUCCESS, 0);
    }

    if (g_connect_btn) {
        lv_obj_t* btn_lbl = lv_obj_get_child(g_connect_btn, 0);
        lv_label_set_text(btn_lbl, "Disconnect");
        lv_obj_set_style_bg_color(g_connect_btn, COLOR_ERROR, 0);
        lv_obj_remove_flag(g_connect_btn, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_status_label) {
        char buf[64];
        snprintf(buf, sizeof(buf), "Connected to %s", net->ssid);
        lv_label_set_text(g_status_label, buf);
    }

    /* Refresh list highlight */
    if (g_network_list) {
        int i = 0;
        lv_obj_t* child = lv_obj_get_child(g_network_list, 0);
        while (child) {
            lv_obj_set_style_bg_color(child, (i == idx) ? COLOR_HIGHLIGHT : COLOR_CARD, 0);
            child = lv_obj_get_child(g_network_list, ++i);
        }
    }
}

/*******************************************************************************
 * Network Item Click Handler
 ******************************************************************************/

static void network_item_cb(lv_event_t* e)
{
    lv_obj_t* item = lv_event_get_target(e);
    int idx = (int)(intptr_t)lv_obj_get_user_data(item);

    if (idx < 0 || idx >= mock_network_count) return;

    g_selected_idx = idx;
    sim_wifi_network_t* net = &mock_networks[idx];

    /* Update details */
    if (g_details_ssid) {
        lv_label_set_text(g_details_ssid, net->ssid);
    }

    if (g_details_status) {
        if (net->flags & 0x01) {
            lv_label_set_text(g_details_status, "Connected");
            lv_obj_set_style_text_color(g_details_status, COLOR_SUCCESS, 0);
        } else {
            lv_label_set_text(g_details_status, "Not connected");
            lv_obj_set_style_text_color(g_details_status, COLOR_TEXT_DIM, 0);
        }
    }

    /* Update connect button */
    if (g_connect_btn) {
        lv_obj_t* btn_lbl = lv_obj_get_child(g_connect_btn, 0);
        if (net->flags & 0x01) {
            lv_label_set_text(btn_lbl, "Disconnect");
            lv_obj_set_style_bg_color(g_connect_btn, COLOR_ERROR, 0);
        } else {
            lv_label_set_text(btn_lbl, "Connect");
            lv_obj_set_style_bg_color(g_connect_btn, COLOR_HIGHLIGHT, 0);
        }
        lv_obj_remove_flag(g_connect_btn, LV_OBJ_FLAG_HIDDEN);
    }

    /* Update highlight */
    int i = 0;
    lv_obj_t* child = lv_obj_get_child(g_network_list, 0);
    while (child) {
        lv_obj_set_style_bg_color(child, (i == idx) ? COLOR_HIGHLIGHT : COLOR_CARD, 0);
        child = lv_obj_get_child(g_network_list, ++i);
    }
}

/*******************************************************************************
 * Connect Button Handler
 ******************************************************************************/

static void connect_btn_cb(lv_event_t* e)
{
    (void)e;
    if (g_selected_idx < 0) return;

    sim_wifi_network_t* net = &mock_networks[g_selected_idx];

    if (net->flags & 0x01) {
        /* Disconnect */
        net->flags &= ~0x01;
        connected_idx = -1;
        g_wifi_state = WIFI_STATE_DISCONNECTED;

        if (g_details_status) {
            lv_label_set_text(g_details_status, "Not connected");
            lv_obj_set_style_text_color(g_details_status, COLOR_TEXT_DIM, 0);
        }

        /* Update button */
        if (g_connect_btn) {
            lv_obj_t* btn_lbl = lv_obj_get_child(g_connect_btn, 0);
            lv_label_set_text(btn_lbl, "Connect");
            lv_obj_set_style_bg_color(g_connect_btn, COLOR_HIGHLIGHT, 0);
        }

        /* Update status bar */
        if (g_status_label) {
            lv_label_set_text(g_status_label, "Not connected");
        }
    } else {
        /* Connect to new network */
        if (net->security == WIFI_SECURITY_OPEN) {
            /* Open network - connect directly */
            do_wifi_connect(g_selected_idx);
        } else {
            /* Secured network - show password dialog */
            show_password_dialog(g_selected_idx);
        }
    }
}

/*******************************************************************************
 * Scan Button Handler
 ******************************************************************************/

static void scan_btn_cb(lv_event_t* e)
{
    /* Simulate RSSI fluctuation */
    for (int i = 0; i < mock_network_count; i++) {
        mock_networks[i].rssi += (rand() % 11) - 5;  /* -5 to +5 */
        if (mock_networks[i].rssi > -30) mock_networks[i].rssi = -30;
        if (mock_networks[i].rssi < -90) mock_networks[i].rssi = -90;
    }

    /* Refresh list */
    if (g_network_list) {
        lv_obj_clean(g_network_list);

        for (int i = 0; i < mock_network_count; i++) {
            sim_wifi_network_t* net = &mock_networks[i];

            lv_obj_t* item = lv_obj_create(g_network_list);
            lv_obj_set_size(item, 248, 50);
            lv_obj_set_style_bg_color(item, (i == g_selected_idx) ? COLOR_HIGHLIGHT : COLOR_CARD, 0);
            lv_obj_set_style_border_width(item, 0, 0);
            lv_obj_set_style_radius(item, 6, 0);
            lv_obj_set_style_pad_all(item, 8, 0);
            lv_obj_set_scrollbar_mode(item, LV_SCROLLBAR_MODE_OFF);
            lv_obj_remove_flag(item, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_flex_flow(item, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(item, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_user_data(item, (void*)(intptr_t)i);
            lv_obj_add_event_cb(item, network_item_cb, LV_EVENT_CLICKED, NULL);

            /* Left: SSID + status */
            lv_obj_t* left = lv_obj_create(item);
            lv_obj_set_size(left, 180, LV_SIZE_CONTENT);
            lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
            lv_obj_set_style_border_width(left, 0, 0);
            lv_obj_set_style_pad_all(left, 0, 0);
            lv_obj_set_scrollbar_mode(left, LV_SCROLLBAR_MODE_OFF);
            lv_obj_remove_flag(left, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);

            lv_obj_t* ssid = lv_label_create(left);
            lv_label_set_text(ssid, net->ssid);
            lv_obj_set_style_text_color(ssid, COLOR_TEXT, 0);
            lv_label_set_long_mode(ssid, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_width(ssid, 180);

            lv_obj_t* status = lv_label_create(left);
            if (net->flags & 0x01) {
                char buf[48];
                snprintf(buf, sizeof(buf), LV_SYMBOL_OK " %s", security_to_str(net->security));
                lv_label_set_text(status, buf);
                lv_obj_set_style_text_color(status, COLOR_SUCCESS, 0);
            } else {
                char buf[48];
                if (net->security != WIFI_SECURITY_OPEN) {
                    snprintf(buf, sizeof(buf), LV_SYMBOL_EYE_CLOSE " %s", security_to_str(net->security));
                } else {
                    snprintf(buf, sizeof(buf), "Open");
                }
                lv_label_set_text(status, buf);
                lv_obj_set_style_text_color(status, COLOR_TEXT_DIM, 0);
            }
            lv_obj_set_style_text_font(status, &lv_font_montserrat_12, 0);

            /* Right: Signal bars */
            create_signal_bars(item, net->rssi);
        }
    }

    if (g_status_label) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d networks found", mock_network_count);
        lv_label_set_text(g_status_label, buf);
    }
}

/*******************************************************************************
 * Example 5: Full WiFi Manager (macOS Style)
 ******************************************************************************/

void part5_ex5_wifi_manager(void)
{
    printf("Part 5 Example 5: WiFi Manager (Simulated)\n");

    lv_obj_t* screen = lv_screen_active();

    /* Main container */
    lv_obj_t* main = lv_obj_create(screen);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 0, 0);
    lv_obj_set_style_radius(main, 0, 0);
    lv_obj_center(main);
    lv_obj_set_flex_flow(main, LV_FLEX_FLOW_ROW);

    /* ===== LEFT SIDEBAR ===== */
    lv_obj_t* sidebar = lv_obj_create(main);
    lv_obj_set_size(sidebar, 280, 480);
    lv_obj_set_style_bg_color(sidebar, COLOR_SIDEBAR, 0);
    lv_obj_set_style_border_width(sidebar, 0, 0);
    lv_obj_set_style_radius(sidebar, 0, 0);
    lv_obj_set_style_pad_all(sidebar, 12, 0);
    lv_obj_set_flex_flow(sidebar, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(sidebar, 0);

    /* Title */
    lv_obj_t* title = lv_label_create(sidebar);
    lv_label_set_text(title, LV_SYMBOL_WIFI " Wi-Fi");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_set_style_pad_bottom(title, 8, 0);

    /* Status */
    g_status_label = lv_label_create(sidebar);
    lv_label_set_text(g_status_label, "8 networks found");
    lv_obj_set_style_text_color(g_status_label, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_pad_bottom(g_status_label, 10, 0);

    /* Network list */
    g_network_list = lv_obj_create(sidebar);
    lv_obj_set_size(g_network_list, 256, 340);
    lv_obj_set_style_bg_color(g_network_list, COLOR_CARD, 0);
    lv_obj_set_style_border_width(g_network_list, 0, 0);
    lv_obj_set_style_radius(g_network_list, 8, 0);
    lv_obj_set_style_pad_all(g_network_list, 5, 0);
    lv_obj_set_flex_flow(g_network_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(g_network_list, LV_DIR_VER);
    lv_obj_set_flex_grow(g_network_list, 1);

    /* Scan button */
    lv_obj_t* scan_btn = lv_button_create(sidebar);
    lv_obj_set_size(scan_btn, 256, 42);
    lv_obj_set_style_bg_color(scan_btn, COLOR_HIGHLIGHT, 0);
    lv_obj_set_style_radius(scan_btn, 8, 0);
    lv_obj_add_event_cb(scan_btn, scan_btn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* scan_lbl = lv_label_create(scan_btn);
    lv_label_set_text(scan_lbl, LV_SYMBOL_REFRESH " Scan");
    lv_obj_center(scan_lbl);

    /* ===== RIGHT DETAILS PANEL ===== */
    lv_obj_t* details = lv_obj_create(main);
    lv_obj_set_size(details, 520, 480);
    lv_obj_set_style_bg_color(details, COLOR_BG, 0);
    lv_obj_set_style_border_width(details, 0, 0);
    lv_obj_set_style_radius(details, 0, 0);
    lv_obj_set_style_pad_all(details, 20, 0);
    lv_obj_set_flex_flow(details, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(details, 1);

    /* SSID Header */
    g_details_ssid = lv_label_create(details);
    lv_label_set_text(g_details_ssid, "Select a network");
    lv_obj_set_style_text_font(g_details_ssid, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(g_details_ssid, COLOR_TEXT, 0);

    /* Status */
    g_details_status = lv_label_create(details);
    lv_label_set_text(g_details_status, "Not connected");
    lv_obj_set_style_text_color(g_details_status, COLOR_TEXT_DIM, 0);
    lv_obj_set_style_pad_bottom(g_details_status, 15, 0);

    /* TabView for TCP/IP and Hardware */
    lv_obj_t* tabview = lv_tabview_create(details);
    lv_obj_set_size(tabview, 480, 300);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
    lv_tabview_set_tab_bar_size(tabview, 40);
    lv_obj_set_style_bg_color(tabview, COLOR_CARD, 0);
    lv_obj_set_style_radius(tabview, 8, 0);
    lv_obj_set_flex_grow(tabview, 1);

    /* TCP/IP Tab */
    lv_obj_t* tab_tcpip = lv_tabview_add_tab(tabview, "TCP/IP");
    lv_obj_set_style_pad_all(tab_tcpip, 15, 0);
    lv_obj_set_flex_flow(tab_tcpip, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(tab_tcpip, LV_SCROLLBAR_MODE_OFF);

    const char* tcpip_labels[] = {"Configure IPv4:", "IP Address:", "Subnet Mask:", "Router:", "DNS:"};
    char tcpip_values[5][20];
    snprintf(tcpip_values[0], 20, "Using DHCP");
    snprintf(tcpip_values[1], 20, "%d.%d.%d.%d", mock_ip[0], mock_ip[1], mock_ip[2], mock_ip[3]);
    snprintf(tcpip_values[2], 20, "%d.%d.%d.%d", mock_subnet[0], mock_subnet[1], mock_subnet[2], mock_subnet[3]);
    snprintf(tcpip_values[3], 20, "%d.%d.%d.%d", mock_gateway[0], mock_gateway[1], mock_gateway[2], mock_gateway[3]);
    snprintf(tcpip_values[4], 20, "%d.%d.%d.%d", mock_dns[0], mock_dns[1], mock_dns[2], mock_dns[3]);

    for (int i = 0; i < 5; i++) {
        lv_obj_t* row = lv_obj_create(tab_tcpip);
        lv_obj_set_size(row, LV_PCT(100), 40);
        lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);
        lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* lbl = lv_label_create(row);
        lv_label_set_text(lbl, tcpip_labels[i]);
        lv_obj_set_style_text_color(lbl, COLOR_TEXT_DIM, 0);

        lv_obj_t* val = lv_label_create(row);
        lv_label_set_text(val, tcpip_values[i]);
        lv_obj_set_style_text_color(val, COLOR_TEXT, 0);
    }

    /* Hardware Tab */
    lv_obj_t* tab_hw = lv_tabview_add_tab(tabview, "Hardware");
    lv_obj_set_style_pad_all(tab_hw, 15, 0);
    lv_obj_set_flex_flow(tab_hw, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(tab_hw, LV_SCROLLBAR_MODE_OFF);

    const char* hw_labels[] = {"MAC Address:", "Channel:", "Band:", "Security:", "Signal:"};
    char hw_values[5][24];
    snprintf(hw_values[0], 24, "%02X:%02X:%02X:%02X:%02X:%02X",
             mock_mac[0], mock_mac[1], mock_mac[2], mock_mac[3], mock_mac[4], mock_mac[5]);
    snprintf(hw_values[1], 24, "%d", mock_networks[0].channel);
    snprintf(hw_values[2], 24, "%s", band_to_str(mock_networks[0].band));
    snprintf(hw_values[3], 24, "%s", security_to_str(mock_networks[0].security));
    snprintf(hw_values[4], 24, "%d dBm", mock_networks[0].rssi);

    for (int i = 0; i < 5; i++) {
        lv_obj_t* row = lv_obj_create(tab_hw);
        lv_obj_set_size(row, LV_PCT(100), 40);
        lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);
        lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* lbl = lv_label_create(row);
        lv_label_set_text(lbl, hw_labels[i]);
        lv_obj_set_style_text_color(lbl, COLOR_TEXT_DIM, 0);

        lv_obj_t* val = lv_label_create(row);
        lv_label_set_text(val, hw_values[i]);
        lv_obj_set_style_text_color(val, COLOR_TEXT, 0);
    }

    /* Connect/Disconnect Button */
    g_connect_btn = lv_button_create(details);
    lv_obj_set_size(g_connect_btn, 150, 45);
    lv_obj_set_style_bg_color(g_connect_btn, COLOR_HIGHLIGHT, 0);
    lv_obj_set_style_radius(g_connect_btn, 8, 0);
    lv_obj_add_event_cb(g_connect_btn, connect_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(g_connect_btn, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t* btn_lbl = lv_label_create(g_connect_btn);
    lv_label_set_text(btn_lbl, "Connect");
    lv_obj_center(btn_lbl);

    /* Populate initial list */
    scan_btn_cb(NULL);

    /* Auto-select connected network */
    g_selected_idx = connected_idx;
    if (g_selected_idx >= 0) {
        sim_wifi_network_t* net = &mock_networks[g_selected_idx];
        lv_label_set_text(g_details_ssid, net->ssid);
        lv_label_set_text(g_details_status, "Connected");
        lv_obj_set_style_text_color(g_details_status, COLOR_SUCCESS, 0);

        lv_obj_t* btn_child = lv_obj_get_child(g_connect_btn, 0);
        lv_label_set_text(btn_child, "Disconnect");
        lv_obj_set_style_bg_color(g_connect_btn, COLOR_ERROR, 0);
        lv_obj_remove_flag(g_connect_btn, LV_OBJ_FLAG_HIDDEN);
    }

    /* Footer + Clock */
    sim_create_footer(screen, "Ex5: WiFi Manager");
    sim_create_clock(screen);
}

/*******************************************************************************
 * Example 1-4: Simpler versions
 ******************************************************************************/

void part5_ex1_wifi_list(void)
{
    printf("Part 5 Example 1: WiFi Network List (Simulated)\n");
    part5_ex5_wifi_manager();
    sim_create_footer(lv_screen_active(), "Ex1: WiFi Network List");
    sim_create_clock(lv_screen_active());
}

void part5_ex2_wifi_connect(void)
{
    printf("Part 5 Example 2: WiFi Connect/Disconnect (Simulated)\n");
    part5_ex5_wifi_manager();
    sim_create_footer(lv_screen_active(), "Ex2: WiFi Connect");
    sim_create_clock(lv_screen_active());
}

void part5_ex3_tcpip_info(void)
{
    printf("Part 5 Example 3: TCP/IP Information (Simulated)\n");
    part5_ex5_wifi_manager();
    sim_create_footer(lv_screen_active(), "Ex3: TCP/IP Info");
}

void part5_ex4_hardware_info(void)
{
    printf("Part 5 Example 4: Hardware Information (Simulated)\n");
    part5_ex5_wifi_manager();
    sim_create_footer(lv_screen_active(), "Ex4: Hardware Info");
}

/*******************************************************************************
 * Example 6: WiFi Status Dashboard
 ******************************************************************************/

static lv_obj_t* g_rssi_arc = NULL;
static lv_obj_t* g_rssi_label = NULL;
static lv_obj_t* g_uptime_label = NULL;
static uint32_t g_uptime = 0;

static void status_timer_cb(lv_timer_t* timer)
{
    g_uptime++;

    /* Simulate RSSI fluctuation */
    if (connected_idx >= 0) {
        mock_networks[connected_idx].rssi += (rand() % 5) - 2;
        if (mock_networks[connected_idx].rssi > -30) mock_networks[connected_idx].rssi = -30;
        if (mock_networks[connected_idx].rssi < -90) mock_networks[connected_idx].rssi = -90;

        int8_t rssi = mock_networks[connected_idx].rssi;

        if (g_rssi_arc) {
            int arc_val = (rssi + 100) * 100 / 70;
            if (arc_val < 0) arc_val = 0;
            if (arc_val > 100) arc_val = 100;
            lv_arc_set_value(g_rssi_arc, arc_val);
            lv_obj_set_style_arc_color(g_rssi_arc, rssi_to_color(rssi), LV_PART_INDICATOR);
        }

        if (g_rssi_label) {
            lv_label_set_text_fmt(g_rssi_label, "%d dBm", rssi);
        }
    }

    if (g_uptime_label) {
        uint32_t h = g_uptime / 3600;
        uint32_t m = (g_uptime % 3600) / 60;
        uint32_t s = g_uptime % 60;
        lv_label_set_text_fmt(g_uptime_label, "%02u:%02u:%02u", (unsigned)h, (unsigned)m, (unsigned)s);
    }
}

void part5_ex6_wifi_status(void)
{
    printf("Part 5 Example 6: WiFi Status Dashboard (Simulated)\n");

    lv_obj_t* screen = lv_screen_active();

    lv_obj_t* main = lv_obj_create(screen);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "WiFi Status Dashboard");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* Cards row */
    lv_obj_t* cards = lv_obj_create(main);
    lv_obj_set_size(cards, 760, 180);
    lv_obj_set_style_bg_opa(cards, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cards, 0, 0);
    lv_obj_align(cards, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_flex_flow(cards, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cards, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Connection Card */
    lv_obj_t* conn_card = lv_obj_create(cards);
    lv_obj_set_size(conn_card, 220, 160);
    lv_obj_set_style_bg_color(conn_card, COLOR_CARD, 0);
    lv_obj_set_style_radius(conn_card, 12, 0);

    lv_obj_t* conn_title = lv_label_create(conn_card);
    lv_label_set_text(conn_title, "Connection");
    lv_obj_set_style_text_color(conn_title, COLOR_TEXT_DIM, 0);
    lv_obj_align(conn_title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* wifi_icon = lv_label_create(conn_card);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(wifi_icon, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(wifi_icon, COLOR_SUCCESS, 0);
    lv_obj_align(wifi_icon, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t* ssid_lbl = lv_label_create(conn_card);
    lv_label_set_text(ssid_lbl, mock_networks[connected_idx].ssid);
    lv_obj_set_style_text_color(ssid_lbl, COLOR_TEXT, 0);
    lv_obj_align(ssid_lbl, LV_ALIGN_BOTTOM_MID, 0, -15);

    /* Signal Card */
    lv_obj_t* sig_card = lv_obj_create(cards);
    lv_obj_set_size(sig_card, 220, 160);
    lv_obj_set_style_bg_color(sig_card, COLOR_CARD, 0);
    lv_obj_set_style_radius(sig_card, 12, 0);

    lv_obj_t* sig_title = lv_label_create(sig_card);
    lv_label_set_text(sig_title, "Signal Strength");
    lv_obj_set_style_text_color(sig_title, COLOR_TEXT_DIM, 0);
    lv_obj_align(sig_title, LV_ALIGN_TOP_MID, 0, 10);

    g_rssi_arc = lv_arc_create(sig_card);
    lv_obj_set_size(g_rssi_arc, 100, 100);
    lv_arc_set_range(g_rssi_arc, 0, 100);
    lv_arc_set_value(g_rssi_arc, 70);
    lv_arc_set_bg_angles(g_rssi_arc, 135, 45);
    lv_obj_set_style_arc_color(g_rssi_arc, lv_color_hex(0x555555), LV_PART_MAIN);
    lv_obj_set_style_arc_color(g_rssi_arc, COLOR_SUCCESS, LV_PART_INDICATOR);
    lv_obj_remove_style(g_rssi_arc, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(g_rssi_arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(g_rssi_arc, LV_ALIGN_CENTER, 0, 5);

    g_rssi_label = lv_label_create(sig_card);
    lv_label_set_text_fmt(g_rssi_label, "%d dBm", mock_networks[connected_idx].rssi);
    lv_obj_set_style_text_color(g_rssi_label, COLOR_TEXT, 0);
    lv_obj_align(g_rssi_label, LV_ALIGN_BOTTOM_MID, 0, -10);

    /* Uptime Card */
    lv_obj_t* up_card = lv_obj_create(cards);
    lv_obj_set_size(up_card, 220, 160);
    lv_obj_set_style_bg_color(up_card, COLOR_CARD, 0);
    lv_obj_set_style_radius(up_card, 12, 0);

    lv_obj_t* up_title = lv_label_create(up_card);
    lv_label_set_text(up_title, "Connection Uptime");
    lv_obj_set_style_text_color(up_title, COLOR_TEXT_DIM, 0);
    lv_obj_align(up_title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* clock_icon = lv_label_create(up_card);
    lv_label_set_text(clock_icon, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(clock_icon, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(clock_icon, COLOR_HIGHLIGHT, 0);
    lv_obj_align(clock_icon, LV_ALIGN_CENTER, 0, -10);

    g_uptime_label = lv_label_create(up_card);
    lv_label_set_text(g_uptime_label, "00:00:00");
    lv_obj_set_style_text_font(g_uptime_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(g_uptime_label, COLOR_TEXT, 0);
    lv_obj_align(g_uptime_label, LV_ALIGN_BOTTOM_MID, 0, -15);

    /* Info panel */
    lv_obj_t* info = lv_obj_create(main);
    lv_obj_set_size(info, 760, 180);
    lv_obj_set_style_bg_color(info, COLOR_SIDEBAR, 0);
    lv_obj_set_style_radius(info, 12, 0);
    lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t* info_title = lv_label_create(info);
    lv_label_set_text(info_title, "Network Activity");
    lv_obj_set_style_text_color(info_title, COLOR_TEXT_DIM, 0);
    lv_obj_align(info_title, LV_ALIGN_TOP_LEFT, 20, 15);

    lv_obj_t* info_msg = lv_label_create(info);
    lv_label_set_text(info_msg, "Monitoring network connection...\nSignal strength updates every second.");
    lv_obj_set_style_text_color(info_msg, COLOR_TEXT, 0);
    lv_obj_align(info_msg, LV_ALIGN_CENTER, 0, 10);

    /* Footer */
    sim_create_footer(screen, "Ex6: WiFi Status");

    /* Timer */
    g_update_timer = lv_timer_create(status_timer_cb, 1000, NULL);
}

/*******************************************************************************
 * Example 7 & 8: IoT Dashboard and MQTT Preview
 ******************************************************************************/

void part5_ex7_iot_dashboard(void)
{
    printf("Part 5 Example 7: IoT Dashboard (Simulated)\n");
    part5_ex6_wifi_status();
    sim_create_footer(lv_screen_active(), "Ex7: IoT Dashboard");
}

void part5_ex8_mqtt_preview(void)
{
    printf("Part 5 Example 8: MQTT Preview (UI Only)\n");

    lv_obj_t* screen = lv_screen_active();

    lv_obj_t* main = lv_obj_create(screen);
    lv_obj_set_size(main, 800, 480);
    lv_obj_set_style_bg_color(main, COLOR_BG, 0);
    lv_obj_set_style_border_width(main, 0, 0);
    lv_obj_set_style_pad_all(main, 20, 0);
    lv_obj_center(main);

    /* Title */
    lv_obj_t* title = lv_label_create(main);
    lv_label_set_text(title, "MQTT IoT Connectivity (Preview)");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title, COLOR_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* Content */
    lv_obj_t* content = lv_obj_create(main);
    lv_obj_set_size(content, 760, 380);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    /* Left: Connection */
    lv_obj_t* conn = lv_obj_create(content);
    lv_obj_set_size(conn, 370, 370);
    lv_obj_set_style_bg_color(conn, COLOR_CARD, 0);
    lv_obj_set_style_radius(conn, 12, 0);
    lv_obj_set_style_pad_all(conn, 20, 0);

    lv_obj_t* conn_title = lv_label_create(conn);
    lv_label_set_text(conn_title, "Broker Connection");
    lv_obj_set_style_text_font(conn_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(conn_title, COLOR_TEXT, 0);
    lv_obj_align(conn_title, LV_ALIGN_TOP_LEFT, 0, 0);

    const char* info[] = {"Broker: mqtt.eclipse.org", "Port: 1883", "Client: psoc-edge-001", "Status: Disconnected"};
    for (int i = 0; i < 4; i++) {
        lv_obj_t* lbl = lv_label_create(conn);
        lv_label_set_text(lbl, info[i]);
        lv_obj_set_style_text_color(lbl, i == 3 ? COLOR_ERROR : COLOR_TEXT_DIM, 0);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 40 + i * 30);
    }

    lv_obj_t* mqtt_btn = lv_button_create(conn);
    lv_obj_set_size(mqtt_btn, 150, 45);
    lv_obj_set_style_bg_color(mqtt_btn, COLOR_HIGHLIGHT, 0);
    lv_obj_set_style_radius(mqtt_btn, 8, 0);
    lv_obj_align(mqtt_btn, LV_ALIGN_BOTTOM_LEFT, 0, -20);

    lv_obj_t* btn_lbl = lv_label_create(mqtt_btn);
    lv_label_set_text(btn_lbl, "Connect");
    lv_obj_center(btn_lbl);

    /* Subscriptions */
    lv_obj_t* sub_title = lv_label_create(conn);
    lv_label_set_text(sub_title, "Subscriptions:");
    lv_obj_set_style_text_color(sub_title, COLOR_TEXT, 0);
    lv_obj_align(sub_title, LV_ALIGN_TOP_LEFT, 0, 200);

    const char* topics[] = {"sensors/temperature", "sensors/humidity", "device/control"};
    for (int i = 0; i < 3; i++) {
        lv_obj_t* topic = lv_label_create(conn);
        char buf[48];
        snprintf(buf, sizeof(buf), "  " LV_SYMBOL_RIGHT " %s", topics[i]);
        lv_label_set_text(topic, buf);
        lv_obj_set_style_text_color(topic, COLOR_SUCCESS, 0);
        lv_obj_align(topic, LV_ALIGN_TOP_LEFT, 0, 230 + i * 25);
    }

    /* Right: Messages */
    lv_obj_t* msg_panel = lv_obj_create(content);
    lv_obj_set_size(msg_panel, 370, 370);
    lv_obj_set_style_bg_color(msg_panel, COLOR_CARD, 0);
    lv_obj_set_style_radius(msg_panel, 12, 0);
    lv_obj_set_style_pad_all(msg_panel, 20, 0);

    lv_obj_t* msg_title = lv_label_create(msg_panel);
    lv_label_set_text(msg_title, "Message Log");
    lv_obj_set_style_text_font(msg_title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(msg_title, COLOR_TEXT, 0);
    lv_obj_align(msg_title, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t* log_area = lv_obj_create(msg_panel);
    lv_obj_set_size(log_area, 330, 200);
    lv_obj_set_style_bg_color(log_area, COLOR_SIDEBAR, 0);
    lv_obj_set_style_radius(log_area, 8, 0);
    lv_obj_align(log_area, LV_ALIGN_TOP_LEFT, 0, 40);

    lv_obj_t* log_msg = lv_label_create(log_area);
    lv_label_set_text(log_msg, "Waiting for connection...\n\nMessages will appear here.");
    lv_obj_set_style_text_color(log_msg, COLOR_TEXT_DIM, 0);
    lv_obj_center(log_msg);

    /* Note */
    lv_obj_t* note = lv_label_create(main);
    lv_label_set_text(note, "Note: This is a UI preview. MQTT requires library integration.");
    lv_obj_set_style_text_color(note, COLOR_TEXT_DIM, 0);
    lv_obj_align(note, LV_ALIGN_BOTTOM_MID, 0, -10);

    /* Footer */
    sim_create_footer(screen, "Ex8: MQTT Preview");
}
