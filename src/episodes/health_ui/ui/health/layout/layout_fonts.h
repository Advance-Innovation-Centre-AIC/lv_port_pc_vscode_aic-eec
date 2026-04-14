#ifndef HEALTH_LAYOUT_FONTS_H
#define HEALTH_LAYOUT_FONTS_H

#include "../health_ui_display_policy.h"
#include "lvgl.h"

/*******************************************************************************
 * Font breakpoint map — step-down by display height
 *
 * Uses runtime font pointers (hl_font_XX) that have Thai fallback set up
 * by health_ui_i18n_init(). This allows Montserrat to render Thai characters
 * through the LVGL font fallback chain.
 *
 *   Role          7" (>=600)       4.3" (<600)
 *   Hero          size 36          size 28
 *   Title         size 24          size 20
 *   Heading       size 20          size 16
 *   Body          size 16          size 14
 *   Caption       size 14          size 14  (clamped)
 *   Icon LG       size 24          size 20
 *   Icon SM       size 20          size 16
 ******************************************************************************/

/* Declared in layout_i18n.c — Montserrat copies with NotoSansThai fallback */
extern const lv_font_t *hl_font_12;
extern const lv_font_t *hl_font_14;
extern const lv_font_t *hl_font_16;
extern const lv_font_t *hl_font_20;
extern const lv_font_t *hl_font_24;
extern const lv_font_t *hl_font_28;
extern const lv_font_t *hl_font_36;

#if (MY_DISP_VER_RES >= 600)
/* 7" display (1024x600) */
#define HL_FONT_HERO     (hl_font_36)
#define HL_FONT_TITLE    (hl_font_24)
#define HL_FONT_HEADING  (hl_font_20)
#define HL_FONT_BODY     (hl_font_16)
#define HL_FONT_CAPTION  (hl_font_14)
#define HL_FONT_SMALL    (hl_font_12)
#define HL_FONT_ICON_LG  (hl_font_24)
#define HL_FONT_ICON_SM  (hl_font_20)
#else
/* 4.3" display (832x480) */
#define HL_FONT_HERO     (hl_font_28)
#define HL_FONT_TITLE    (hl_font_20)
#define HL_FONT_HEADING  (hl_font_16)
#define HL_FONT_BODY     (hl_font_14)
#define HL_FONT_CAPTION  (hl_font_14)
#define HL_FONT_SMALL    (hl_font_12)
#define HL_FONT_ICON_LG  (hl_font_20)
#define HL_FONT_ICON_SM  (hl_font_16)
#endif

#endif
