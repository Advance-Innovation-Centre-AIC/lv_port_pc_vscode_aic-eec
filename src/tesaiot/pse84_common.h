/**
 * pse84_common.h — PC simulator stub for Developer Hub examples
 *
 * Provides the same defines, color palette, and helper macros as the
 * firmware pse84_common.h so that practice examples compile unmodified.
 */
#ifndef PSE84_COMMON_H
#define PSE84_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"
#include "app_interface.h"
#include "tesaiot_thai.h"
#include "sensor_bus.h"
#include "app_logo.h"

/* ── Display ─────────────────────────────────────────────── */
#define DISPLAY_WIDTH   800
#define DISPLAY_HEIGHT  480

/* ── BSP sensor feature flags (all present on KIT_PSE84_AI) */
#define BSP_HAS_BMI270        1
#define BSP_HAS_BMM350        1
#define BSP_HAS_DPS368        1
#define BSP_HAS_SHT40         1
#define BSP_HAS_CAPSENSE      0
#define BSP_HAS_POTENTIOMETER 0

/* ── UI_COLOR aliases (used by some Developer Hub examples) ─ */
#define UI_COLOR_PRIMARY    lv_color_hex(0x00D4FF)
#define UI_COLOR_SUCCESS    lv_color_hex(0x4ADE80)
#define UI_COLOR_WARNING    lv_color_hex(0xFBBF24)
#define UI_COLOR_ERROR      lv_color_hex(0xEF4444)
#define UI_COLOR_INFO       lv_color_hex(0x3B82F6)
#define UI_COLOR_TEXT       lv_color_hex(0xFFFFFF)
#define UI_COLOR_TEXT_DIM   lv_color_hex(0x94A3B8)
#define UI_COLOR_BG_DARK    lv_color_hex(0x16213E)
#define UI_COLOR_BG_CARD    lv_color_hex(0x1F4068)

/* ── Color palette ───────────────────────────────────────── */
#define COLOR_PRIMARY       lv_color_hex(0x00D4FF)
#define COLOR_BG_DARK       lv_color_hex(0x16213E)
#define COLOR_BG_CARD       lv_color_hex(0x1F4068)
#define COLOR_SUCCESS       lv_color_hex(0x4ADE80)
#define COLOR_WARNING       lv_color_hex(0xFBBF24)
#define COLOR_ERROR         lv_color_hex(0xEF4444)
#define COLOR_INFO          lv_color_hex(0x3B82F6)
#define COLOR_TEXT          lv_color_hex(0xFFFFFF)
#define COLOR_TEXT_DIM      lv_color_hex(0x94A3B8)
#define COLOR_ACCENT_CYAN   lv_color_hex(0x00D4FF)
#define COLOR_ACCENT_GREEN  lv_color_hex(0x4ADE80)
#define COLOR_ACCENT_ORANGE lv_color_hex(0xFBBF24)
#define COLOR_ACCENT_RED    lv_color_hex(0xEF4444)
#define COLOR_ACCENT_BLUE   lv_color_hex(0x3B82F6)

/* ── Golden ratio layout ─────────────────────────────────── */
#define PHI_MAJOR  0.618f
#define PHI_MINOR  0.382f

/* ── GPIO stubs (no-op on PC) ────────────────────────────── */
#define CYBSP_USER_LED1  0
#define CYBSP_USER_LED2  0
#define CYBSP_USER_BTN1  0
#define CYBSP_USER_BTN2  0

/* ── FreeRTOS stubs (PC simulator has no RTOS) ───────────── */
#ifndef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE 256
#endif
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ 1000
#endif
#ifndef portTICK_PERIOD_MS
#define portTICK_PERIOD_MS (1000 / configTICK_RATE_HZ)
#endif
static inline uint32_t xTaskGetTickCount(void) { return lv_tick_get(); }

/* ── Per-sensor color aliases (used by production dashboard) */
#define UI_COLOR_BMI270     lv_color_hex(0x00D4FF)
#define UI_COLOR_DPS368     lv_color_hex(0xFBBF24)
#define UI_COLOR_SHT40      lv_color_hex(0x4ADE80)
#define UI_COLOR_BMM350     lv_color_hex(0xA78BFA)
#define UI_COLOR_CARD_BG    lv_color_hex(0x1F4068)

/* ── Additional UI defines used by agent-generated code ──── */
#define UI_CARD_RADIUS      8
#ifndef tskKERNEL_VERSION_NUMBER
#define tskKERNEL_VERSION_NUMBER "sim"
#endif

/* ── Helper inline: create a styled card (variadic for compat) */
static inline lv_obj_t *example_card_create(lv_obj_t *parent, ...)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_style_bg_color(card, COLOR_BG_CARD, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_radius(card, UI_CARD_RADIUS, 0);
    lv_obj_set_style_pad_all(card, 12, 0);
    return card;
}

/* ── Helper inline: create a styled label (2 or 4 args) ──── */
static inline lv_obj_t *example_label_create(lv_obj_t *parent, const char *text, ...)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, COLOR_TEXT, 0);
    return label;
}

/* ── Thai font size → font pointer mapping ───────────────── */
LV_FONT_DECLARE(lv_font_noto_thai_14);
LV_FONT_DECLARE(lv_font_noto_thai_16);
LV_FONT_DECLARE(lv_font_noto_thai_20);
LV_FONT_DECLARE(lv_font_noto_thai_28);

static inline const lv_font_t *_thai_font_by_size(int sz)
{
    if (sz >= 28) return &lv_font_noto_thai_28;
    if (sz >= 20) return &lv_font_noto_thai_20;
    if (sz >= 16) return &lv_font_noto_thai_16;
    return &lv_font_noto_thai_14;
}

/* ── thai_label(parent, text, font_size_int, color) ──────── */
static inline lv_obj_t *thai_label(lv_obj_t *parent, const char *text, int font_size, lv_color_t color)
{
    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, text);
    lv_obj_set_style_text_font(lbl, _thai_font_by_size(font_size), 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    return lbl;
}

#endif /* PSE84_COMMON_H */
