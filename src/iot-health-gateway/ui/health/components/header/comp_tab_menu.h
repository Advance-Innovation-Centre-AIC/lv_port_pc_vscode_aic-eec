#ifndef COMP_TAB_MENU_H
#define COMP_TAB_MENU_H

#include <stdint.h>

#include "lvgl.h"

#include "health_ui_display_policy.h"
#include "layout/layout_fonts.h"

/* Layout tuning knobs for tab menu component. */
#ifndef COMP_TAB_MENU_HEIGHT_PX
#define COMP_TAB_MENU_HEIGHT_PX HL_SCALE_V(64)
#endif

#ifndef COMP_TAB_MENU_ICON_CHIP_SIZE_PX
#define COMP_TAB_MENU_ICON_CHIP_SIZE_PX HL_SCALE_V(34)
#endif

#ifndef COMP_TAB_MENU_ICON_DARK_RADIUS_PX
#define COMP_TAB_MENU_ICON_DARK_RADIUS_PX (10)
#endif

#ifndef COMP_TAB_MENU_ICON_LIGHT_RADIUS_PX
#define COMP_TAB_MENU_ICON_LIGHT_RADIUS_PX (18)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_HEIGHT_PX
#define COMP_TAB_MENU_SEGMENT_HEIGHT_PX HL_SCALE_V(42)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_RADIUS_PX
#define COMP_TAB_MENU_SEGMENT_RADIUS_PX (24)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BORDER_WIDTH_PX
#define COMP_TAB_MENU_SEGMENT_BORDER_WIDTH_PX (0)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BTN_WIDTH_PX
#define COMP_TAB_MENU_SEGMENT_BTN_WIDTH_PX (102)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BTN_HEIGHT_PX
#define COMP_TAB_MENU_SEGMENT_BTN_HEIGHT_PX HL_SCALE_V(34)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BTN_RADIUS_PX
#define COMP_TAB_MENU_SEGMENT_BTN_RADIUS_PX (20)
#endif

#ifndef COMP_TAB_MENU_AVATAR_SIZE_PX
#define COMP_TAB_MENU_AVATAR_SIZE_PX HL_SCALE_V(52)
#endif

#ifndef COMP_TAB_MENU_AVATAR_RADIUS_PX
#define COMP_TAB_MENU_AVATAR_RADIUS_PX (26)
#endif

#ifndef COMP_TAB_MENU_NAME_ROW_GAP_PX
#define COMP_TAB_MENU_NAME_ROW_GAP_PX (2)
#endif

/* Metric-detail right action button (Add Data). */
#ifndef COMP_TAB_MENU_ACTION_BTN_WIDTH_PX
#define COMP_TAB_MENU_ACTION_BTN_WIDTH_PX (156)
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_HEIGHT_PX
#define COMP_TAB_MENU_ACTION_BTN_HEIGHT_PX HL_SCALE_V(56)
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_RADIUS_PX
#define COMP_TAB_MENU_ACTION_BTN_RADIUS_PX (28)
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_BG_COLOR_HEX
#define COMP_TAB_MENU_ACTION_BTN_BG_COLOR_HEX (0x3F7FD1)
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_TEXT_COLOR_HEX
#define COMP_TAB_MENU_ACTION_BTN_TEXT_COLOR_HEX (0xFFFFFF)
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_FONT
#define COMP_TAB_MENU_ACTION_BTN_FONT HL_FONT_HEADING
#endif

#ifndef COMP_TAB_MENU_ACTION_BTN_LABEL_TEXT
#define COMP_TAB_MENU_ACTION_BTN_LABEL_TEXT (LV_SYMBOL_PLUS " Add Data")
#endif

#ifndef COMP_TAB_MENU_TITLE_BUF_SIZE
/* Runtime-updated title text cache (Family/Home labels). */
#define COMP_TAB_MENU_TITLE_BUF_SIZE (64U)
#endif

#ifndef COMP_TAB_MENU_USER_NAME_BUF_SIZE
#define COMP_TAB_MENU_USER_NAME_BUF_SIZE (64U)
#endif

#ifndef COMP_TAB_MENU_USER_AGE_BUF_SIZE
#define COMP_TAB_MENU_USER_AGE_BUF_SIZE (24U)
#endif

/* Typography knobs (keep in this component to tune tab/header appearance fast). */
#ifndef COMP_TAB_MENU_TEXT_FONT
#define COMP_TAB_MENU_TEXT_FONT HL_FONT_HEADING
#endif

#ifndef COMP_TAB_MENU_ICON_FONT
#define COMP_TAB_MENU_ICON_FONT HL_FONT_ICON_SM
#endif

#ifndef COMP_TAB_MENU_SEGMENT_FONT
#define COMP_TAB_MENU_SEGMENT_FONT HL_FONT_HEADING
#endif

/* Color knobs for tab menu component. */
#ifndef COMP_TAB_MENU_BG_COLOR_HEX
#define COMP_TAB_MENU_BG_COLOR_HEX (0xFFFFFF)
#endif

#ifndef COMP_TAB_MENU_TEXT_COLOR_HEX
#define COMP_TAB_MENU_TEXT_COLOR_HEX (0x002329)
#endif

/* Generic icon color used by plain icons (for example back button). */
#ifndef COMP_TAB_MENU_ICON_COLOR_HEX
#define COMP_TAB_MENU_ICON_COLOR_HEX (0x002329)
#endif

/* Home-left icon color (main pages). */
#ifndef COMP_TAB_MENU_ICON_DARK_COLOR_HEX
#define COMP_TAB_MENU_ICON_DARK_COLOR_HEX (0x000000)
#endif

/* Setting icon color (right side icon button). */
#ifndef COMP_TAB_MENU_ICON_LIGHT_COLOR_HEX
#define COMP_TAB_MENU_ICON_LIGHT_COLOR_HEX (0x667085)
#endif

/* Home-left icon background (main pages). */
#ifndef COMP_TAB_MENU_ICON_DARK_BG_COLOR_HEX
#define COMP_TAB_MENU_ICON_DARK_BG_COLOR_HEX (0xFFFFFF)
#endif

/* Setting icon button background (main pages). */
#ifndef COMP_TAB_MENU_ICON_LIGHT_BG_COLOR_HEX
#define COMP_TAB_MENU_ICON_LIGHT_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef COMP_TAB_MENU_ICON_LIGHT_BORDER_COLOR_HEX
#define COMP_TAB_MENU_ICON_LIGHT_BORDER_COLOR_HEX (0xF2F3F5)
#endif

#ifndef COMP_TAB_MENU_ICON_LIGHT_BORDER_WIDTH_PX
#define COMP_TAB_MENU_ICON_LIGHT_BORDER_WIDTH_PX (0)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BG_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_BORDER_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_BORDER_COLOR_HEX (0xF2F3F5)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_SELECTED_BG_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_SELECTED_BG_COLOR_HEX (0x3F7FD1)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_UNSELECTED_BG_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_UNSELECTED_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_SELECTED_TEXT_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_SELECTED_TEXT_COLOR_HEX (0xFFFFFF)
#endif

#ifndef COMP_TAB_MENU_SEGMENT_UNSELECTED_TEXT_COLOR_HEX
#define COMP_TAB_MENU_SEGMENT_UNSELECTED_TEXT_COLOR_HEX (0x475467)
#endif

#ifndef COMP_TAB_MENU_AVATAR_BG_COLOR_HEX
#define COMP_TAB_MENU_AVATAR_BG_COLOR_HEX (0xC3D9F4)
#endif

#ifndef COMP_TAB_MENU_AVATAR_ICON_COLOR_HEX
#define COMP_TAB_MENU_AVATAR_ICON_COLOR_HEX (0x002329)
#endif

/* Debug knobs: visualize user-detail back hit-area (left cluster only). */
#ifndef COMP_TAB_MENU_DEBUG_HITAREA_ENABLE
#define COMP_TAB_MENU_DEBUG_HITAREA_ENABLE (0)
#endif

#ifndef COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_COLOR_HEX
#define COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_COLOR_HEX (0xD1FAE5)
#endif

#ifndef COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_OPA
#define COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_OPA (LV_OPA_70)
#endif

#ifndef COMP_TAB_MENU_DEBUG_HITAREA_LEFT_RADIUS_PX
#define COMP_TAB_MENU_DEBUG_HITAREA_LEFT_RADIUS_PX (8)
#endif

typedef enum {
  COMP_TAB_MENU_MODE_MAIN_HEALTH = 0,
  COMP_TAB_MENU_MODE_MAIN_HOME = 1,
  COMP_TAB_MENU_MODE_SETTING = 2,
  COMP_TAB_MENU_MODE_USER_DETAIL = 3,
  COMP_TAB_MENU_MODE_METRIC_DETAIL = 4
} comp_tab_menu_mode_t;

typedef struct {
  lv_obj_t *btn_health;
  lv_obj_t *btn_home;
  lv_obj_t *btn_setting;
  lv_obj_t *btn_back;
  lv_obj_t *btn_action;
} comp_tab_menu_refs_t;

lv_obj_t *comp_tab_menu_create(lv_obj_t *parent, comp_tab_menu_mode_t mode,
                               comp_tab_menu_refs_t *out_refs);
/* Updates runtime title text shown in Health/Home menu modes. */
void comp_tab_menu_set_family_summary(const char *family_name,
                                      uint32_t member_count);
/* Updates user-detail left cluster (avatar color + name + age) at runtime. */
void comp_tab_menu_set_user_detail_profile(const char *user_name,
                                           uint32_t age_years,
                                           uint32_t avatar_bg_color_hex);

#endif
