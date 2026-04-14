#ifndef COMP_DEVICE_CARD_H
#define COMP_DEVICE_CARD_H

#include <stdint.h>

#include "lvgl.h"
#include "health_ui_display_policy.h"
#include "../../layout/layout_fonts.h"

#ifndef COMP_DEVICE_CARD_WIDTH_PCT
/* Fallback width when parent page does not apply runtime pixel-width override. */
#if (MY_DISP_HOR_RES >= 1024)
#define COMP_DEVICE_CARD_WIDTH_PCT (24)
#else
#define COMP_DEVICE_CARD_WIDTH_PCT (31)
#endif
#endif

#ifndef COMP_DEVICE_CARD_HEIGHT_PX
#define COMP_DEVICE_CARD_HEIGHT_PX HL_SCALE_V(60)
#endif

#ifndef COMP_DEVICE_CARD_RADIUS_PX
#define COMP_DEVICE_CARD_RADIUS_PX (10)
#endif

#ifndef COMP_DEVICE_CARD_BG_COLOR_HEX
#define COMP_DEVICE_CARD_BG_COLOR_HEX (0xF7F8FA)
#endif

#ifndef COMP_DEVICE_CARD_PAD_PX
#define COMP_DEVICE_CARD_PAD_PX (8)
#endif

#ifndef COMP_DEVICE_CARD_PAD_HOR_PX
#define COMP_DEVICE_CARD_PAD_HOR_PX (COMP_DEVICE_CARD_PAD_PX)
#endif

#ifndef COMP_DEVICE_CARD_PAD_VER_PX
#define COMP_DEVICE_CARD_PAD_VER_PX HL_SCALE_V(COMP_DEVICE_CARD_PAD_PX)
#endif

#ifndef COMP_DEVICE_CARD_COL_GAP_PX
/* Horizontal gap between avatar block and text column. */
#define COMP_DEVICE_CARD_COL_GAP_PX (8)
#endif

#ifndef COMP_DEVICE_CARD_AVATAR_SIZE_PX
#define COMP_DEVICE_CARD_AVATAR_SIZE_PX (42)
#endif

#ifndef COMP_DEVICE_CARD_AVATAR_RADIUS_PX
#define COMP_DEVICE_CARD_AVATAR_RADIUS_PX (21)
#endif

#ifndef COMP_DEVICE_CARD_AVATAR_BG_COLOR_HEX
#define COMP_DEVICE_CARD_AVATAR_BG_COLOR_HEX (0xE7EAF0)
#endif

#ifndef COMP_DEVICE_CARD_AVATAR_TEXT_COLOR_HEX
#define COMP_DEVICE_CARD_AVATAR_TEXT_COLOR_HEX (0x667085)
#endif

#ifndef COMP_DEVICE_CARD_AVATAR_FONT
#define COMP_DEVICE_CARD_AVATAR_FONT HL_FONT_CAPTION
#endif

#ifndef COMP_DEVICE_CARD_TEXT_ROW_GAP_PX
#define COMP_DEVICE_CARD_TEXT_ROW_GAP_PX HL_SCALE_V(2)
#endif

#ifndef COMP_DEVICE_CARD_NAME_COLOR_HEX
#define COMP_DEVICE_CARD_NAME_COLOR_HEX (0x101828)
#endif

#ifndef COMP_DEVICE_CARD_NAME_FONT
#define COMP_DEVICE_CARD_NAME_FONT HL_FONT_BODY
#endif

#ifndef COMP_DEVICE_CARD_TYPE_COLOR_HEX
#define COMP_DEVICE_CARD_TYPE_COLOR_HEX (0x8A94A2)
#endif

#ifndef COMP_DEVICE_CARD_TYPE_FONT
#define COMP_DEVICE_CARD_TYPE_FONT HL_FONT_SMALL
#endif

typedef struct {
  const char *name;
  const char *type_text;
  uint32_t avatar_bg_color_hex;
  /* Placeholder text inside avatar while image assets are not bound yet. */
  const char *avatar_text;
} comp_device_card_data_t;

lv_obj_t *comp_device_card_create(lv_obj_t *parent,
                                  const comp_device_card_data_t *data);

#endif
