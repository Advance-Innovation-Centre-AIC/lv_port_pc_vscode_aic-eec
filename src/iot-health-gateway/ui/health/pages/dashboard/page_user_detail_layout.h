#ifndef PAGE_USER_DETAIL_LAYOUT_H
#define PAGE_USER_DETAIL_LAYOUT_H

#include <stdbool.h>
#include <stdint.h>

#include "lvgl.h"
#include "../../health_ui_display_policy.h"
#include "../../layout/layout_fonts.h"

#ifndef USER_DETAIL_SIDE_INSET_PX
#define USER_DETAIL_SIDE_INSET_PX (8)
#endif

#ifndef USER_DETAIL_SECTION_ROW_GAP_PX
#define USER_DETAIL_SECTION_ROW_GAP_PX HL_SCALE_V(8)
#endif

#ifndef USER_DETAIL_TOP_ROW_H_PX
#define USER_DETAIL_TOP_ROW_H_PX HL_SCALE_V(190)
#endif

#ifndef USER_DETAIL_BOTTOM_ROW_H_PX
#define USER_DETAIL_BOTTOM_ROW_H_PX HL_SCALE_V(180)
#endif

#ifndef USER_DETAIL_CARD_GAP_PX
#define USER_DETAIL_CARD_GAP_PX (12)
#endif

#ifndef USER_DETAIL_CARD_RADIUS_PX
#define USER_DETAIL_CARD_RADIUS_PX (20)
#endif

#ifndef USER_DETAIL_CARD_BG_COLOR_HEX
#define USER_DETAIL_CARD_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef USER_DETAIL_WARNING_BG_COLOR_HEX
#define USER_DETAIL_WARNING_BG_COLOR_HEX (0xFDF8E5)
#endif

#ifndef USER_DETAIL_CRITICAL_BG_COLOR_HEX
#define USER_DETAIL_CRITICAL_BG_COLOR_HEX (0xFFF1F1)
#endif

#ifndef USER_DETAIL_LOADING_ICON_DEFAULT_VISIBLE
#define USER_DETAIL_LOADING_ICON_DEFAULT_VISIBLE (0)
#endif

#ifndef USER_DETAIL_FONT_SECTION_TITLE
#define USER_DETAIL_FONT_SECTION_TITLE HL_FONT_HEADING
#endif

#ifndef USER_DETAIL_FONT_CARD_TITLE
#define USER_DETAIL_FONT_CARD_TITLE HL_FONT_BODY
#endif

#ifndef USER_DETAIL_FONT_ICON
#define USER_DETAIL_FONT_ICON HL_FONT_ICON_SM
#endif

#ifndef USER_DETAIL_FONT_TIME
#define USER_DETAIL_FONT_TIME HL_FONT_BODY
#endif

#ifndef USER_DETAIL_TIME_MIN_WIDTH_PX
#define USER_DETAIL_TIME_MIN_WIDTH_PX (44)
#endif

#ifndef USER_DETAIL_TITLE_MAX_WIDTH_PX
#define USER_DETAIL_TITLE_MAX_WIDTH_PX (150)
#endif

#ifndef USER_DETAIL_FONT_VALUE
#define USER_DETAIL_FONT_VALUE HL_FONT_HERO
#endif

#ifndef USER_DETAIL_FONT_TREND
#define USER_DETAIL_FONT_TREND HL_FONT_HEADING
#endif

#ifndef USER_DETAIL_FONT_VALUE_META
#define USER_DETAIL_FONT_VALUE_META HL_FONT_BODY
#endif

#ifndef USER_DETAIL_FONT_UNIT
#define USER_DETAIL_FONT_UNIT HL_FONT_BODY
#endif

#ifndef USER_DETAIL_FONT_STATUS
#define USER_DETAIL_FONT_STATUS HL_FONT_BODY
#endif

#ifndef USER_DETAIL_FONT_PREV_TIME
#define USER_DETAIL_FONT_PREV_TIME HL_FONT_BODY
#endif

#ifndef USER_DETAIL_FONT_PREV_VALUE
#define USER_DETAIL_FONT_PREV_VALUE HL_FONT_BODY
#endif

void page_user_detail_layout_build(lv_obj_t *tab);
void page_user_detail_layout_set_user_name(const char *user_name);
void page_user_detail_layout_set_member_index(uint32_t member_index);
void page_user_detail_layout_set_health_loading_icon(bool visible);

#endif
