#ifndef HEALTH_LAYOUT_FONTS_H
#define HEALTH_LAYOUT_FONTS_H

#include "../health_ui_display_policy.h"
#include "lvgl.h"

/*******************************************************************************
 * Font breakpoint map — step-down by display height
 *
 * Fonts cannot scale linearly; pick from available Montserrat sizes.
 * Minimum readable on 4.3" (214 PPI) = montserrat_14 (1.7mm cap height).
 *
 *   Role          7" (≥600)        4.3" (<600)
 *   ─────────     ──────────       ───────────
 *   Hero          montserrat_36    montserrat_28
 *   Title         montserrat_24    montserrat_20
 *   Heading       montserrat_20    montserrat_16
 *   Body          montserrat_16    montserrat_14
 *   Caption       montserrat_14    montserrat_14  (clamped)
 *   Icon LG       montserrat_24    montserrat_20
 *   Icon SM       montserrat_20    montserrat_16
 ******************************************************************************/

#if (MY_DISP_VER_RES >= 600)
/* 7" display (1024×600) */
#define HL_FONT_HERO     (&lv_font_montserrat_36)
#define HL_FONT_TITLE    (&lv_font_montserrat_24)
#define HL_FONT_HEADING  (&lv_font_montserrat_20)
#define HL_FONT_BODY     (&lv_font_montserrat_16)
#define HL_FONT_CAPTION  (&lv_font_montserrat_14)
#define HL_FONT_SMALL    (&lv_font_montserrat_12)
#define HL_FONT_ICON_LG  (&lv_font_montserrat_24)
#define HL_FONT_ICON_SM  (&lv_font_montserrat_20)
#else
/* 4.3" display (832×480) */
#define HL_FONT_HERO     (&lv_font_montserrat_28)
#define HL_FONT_TITLE    (&lv_font_montserrat_20)
#define HL_FONT_HEADING  (&lv_font_montserrat_16)
#define HL_FONT_BODY     (&lv_font_montserrat_14)
#define HL_FONT_CAPTION  (&lv_font_montserrat_14)
#define HL_FONT_SMALL    (&lv_font_montserrat_12)
#define HL_FONT_ICON_LG  (&lv_font_montserrat_20)
#define HL_FONT_ICON_SM  (&lv_font_montserrat_16)
#endif

#endif
