#ifndef HEALTH_LAYOUT_TOKENS_H
#define HEALTH_LAYOUT_TOKENS_H

#include "../health_ui_display_policy.h"

/*******************************************************************************
 * Spacing — scaled proportionally to viewport height
 *
 *   7" (600px):  XS=4  SM=8  MD=12  LG=16  XL=24
 *   4.3" (480px): XS=3  SM=6  MD=10  LG=13  XL=19
 ******************************************************************************/
#define HL_SPACE_XS HL_SCALE_V(4)
#define HL_SPACE_SM HL_SCALE_V(8)
#define HL_SPACE_MD HL_SCALE_V(12)
#define HL_SPACE_LG HL_SCALE_V(16)
#define HL_SPACE_XL HL_SCALE_V(24)

/*******************************************************************************
 * Border radius — scaled proportionally
 ******************************************************************************/
#define HL_RADIUS_SM HL_SCALE_V(8)
#define HL_RADIUS_MD HL_SCALE_V(12)
#define HL_RADIUS_LG HL_SCALE_V(16)
#define HL_RADIUS_XL HL_SCALE_V(20)

/*******************************************************************************
 * Touch target minimum — clamped to physical 7mm
 *
 *   4.3" (214 PPI): 7mm ≈ 59px → clamp at 48px minimum
 *   7"   (170 PPI): 7mm ≈ 47px → 48px minimum
 ******************************************************************************/
#define HL_TOUCH_MIN    (48)
#define HL_TOUCH_TARGET ((HL_SCALE_V(60) > HL_TOUCH_MIN) ? HL_SCALE_V(60) : HL_TOUCH_MIN)
#define HL_TOUCH_COMFY  ((HL_SCALE_V(76) > 60) ? HL_SCALE_V(76) : 60)

/*******************************************************************************
 * Colors — resolution independent
 ******************************************************************************/
#define HL_COLOR_BG (0xFFFFFF)
#define HL_COLOR_SURFACE (0xFFFFFF)
#define HL_COLOR_BORDER (0xD9E2EC)

#define HL_COLOR_TEXT_PRIMARY (0x102A43)
#define HL_COLOR_TEXT_SECONDARY (0x486581)
#define HL_COLOR_TEXT_MUTED (0x7B8794)

#define HL_COLOR_STATUS_NORMAL (0x1E8E3E)
#define HL_COLOR_STATUS_WARNING (0xF59F00)
#define HL_COLOR_STATUS_CRITICAL (0xD93025)

#endif
