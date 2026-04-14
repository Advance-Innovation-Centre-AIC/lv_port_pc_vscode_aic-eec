#include "comp_tab_menu.h"

#include "../../layout/layout_tokens.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum {
  ICON_STYLE_HOME_CHIP = 0,
  ICON_STYLE_SETTING_CHIP = 1,
  ICON_STYLE_PLAIN = 2
} icon_style_t;

static lv_obj_t *s_health_title_label = NULL;
static lv_obj_t *s_home_title_label = NULL;
static char s_health_title_text[COMP_TAB_MENU_TITLE_BUF_SIZE] = "Zac's Family : 6";
static char s_home_title_text[COMP_TAB_MENU_TITLE_BUF_SIZE] = "Zac's Home";
static char s_user_detail_name_text[COMP_TAB_MENU_USER_NAME_BUF_SIZE] = "Member";
static char s_user_detail_age_text[COMP_TAB_MENU_USER_AGE_BUF_SIZE] = "Age: --";
static uint32_t s_user_detail_avatar_bg_color_hex = COMP_TAB_MENU_AVATAR_BG_COLOR_HEX;

#define COMP_TAB_MENU_USER_PROFILE_BINDING_MAX (16U)
typedef struct {
  lv_obj_t *avatar;
  lv_obj_t *name_label;
  lv_obj_t *age_label;
} user_profile_binding_t;

static user_profile_binding_t
    s_user_profile_bindings[COMP_TAB_MENU_USER_PROFILE_BINDING_MAX] = {0};
static uint32_t s_user_profile_binding_count = 0U;

static bool is_valid_obj(const lv_obj_t *obj) {
  return ((NULL != obj) && lv_obj_is_valid((lv_obj_t *)obj));
}

static void update_label_if_valid(lv_obj_t **label_ref, const char *text) {
  if ((NULL == label_ref) || (NULL == text)) {
    return;
  }

  if (is_valid_obj(*label_ref)) {
    lv_label_set_text(*label_ref, text);
  } else {
    *label_ref = NULL;
  }
}

static void update_bg_color_if_valid(lv_obj_t **obj_ref, uint32_t color_hex) {
  if (NULL == obj_ref) {
    return;
  }

  if (is_valid_obj(*obj_ref)) {
    lv_obj_set_style_bg_color(*obj_ref, lv_color_hex(color_hex), LV_PART_MAIN);
  } else {
    *obj_ref = NULL;
  }
}

static void compact_user_profile_bindings(void) {
  uint32_t read_idx;
  uint32_t write_idx = 0U;

  for (read_idx = 0U; read_idx < s_user_profile_binding_count; read_idx++) {
    user_profile_binding_t slot = s_user_profile_bindings[read_idx];
    bool alive = is_valid_obj(slot.avatar) || is_valid_obj(slot.name_label) ||
                 is_valid_obj(slot.age_label);

    if (!alive) {
      continue;
    }

    if (write_idx != read_idx) {
      s_user_profile_bindings[write_idx] = slot;
    }
    write_idx++;
  }

  while (write_idx < s_user_profile_binding_count) {
    s_user_profile_bindings[write_idx].avatar = NULL;
    s_user_profile_bindings[write_idx].name_label = NULL;
    s_user_profile_bindings[write_idx].age_label = NULL;
    write_idx++;
  }

  s_user_profile_binding_count = write_idx;
}

static void register_user_profile_binding(lv_obj_t *avatar, lv_obj_t *name_label,
                                          lv_obj_t *age_label) {
  user_profile_binding_t *slot;

  compact_user_profile_bindings();
  if (s_user_profile_binding_count >= COMP_TAB_MENU_USER_PROFILE_BINDING_MAX) {
    return;
  }

  slot = &s_user_profile_bindings[s_user_profile_binding_count++];
  slot->avatar = avatar;
  slot->name_label = name_label;
  slot->age_label = age_label;
}

static void add_event_bubble_recursive(lv_obj_t *obj) {
  uint32_t i;
  uint32_t child_count;

  if (NULL == obj) {
    return;
  }

  child_count = lv_obj_get_child_count(obj);
  for (i = 0U; i < child_count; i++) {
    lv_obj_t *child = lv_obj_get_child(obj, (int32_t)i);
    if (NULL == child) {
      continue;
    }
    lv_obj_add_flag(child, LV_OBJ_FLAG_EVENT_BUBBLE);
    add_event_bubble_recursive(child);
  }
}

/* Shared lightweight container used by all menu modes to reduce style duplication. */
static lv_obj_t *create_transparent_flex_row(lv_obj_t *parent, bool grow,
                                              lv_coord_t col_gap) {
  lv_obj_t *obj;

  obj = lv_obj_create(parent);
  lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(obj, col_gap, LV_PART_MAIN);
  lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
  if (grow) {
    lv_obj_set_style_flex_grow(obj, 1, LV_PART_MAIN);
  }
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

  return obj;
}

static lv_obj_t *create_icon_chip(lv_obj_t *parent, const char *symbol,
                                  icon_style_t style) {
  lv_obj_t *chip;
  lv_obj_t *icon;
  lv_color_t icon_color;

  if (style == ICON_STYLE_PLAIN) {
    chip = lv_btn_create(parent);
    lv_obj_set_size(chip, COMP_TAB_MENU_ICON_CHIP_SIZE_PX,
                    COMP_TAB_MENU_ICON_CHIP_SIZE_PX);
    lv_obj_set_style_radius(chip, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chip, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(chip, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(chip, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(chip, 0, LV_PART_MAIN);
  } else {
    chip = lv_btn_create(parent);
    lv_obj_set_size(chip, COMP_TAB_MENU_ICON_CHIP_SIZE_PX,
                    COMP_TAB_MENU_ICON_CHIP_SIZE_PX);
    lv_obj_set_style_radius(chip,
                            (style == ICON_STYLE_HOME_CHIP)
                                ? COMP_TAB_MENU_ICON_DARK_RADIUS_PX
                                : COMP_TAB_MENU_ICON_LIGHT_RADIUS_PX,
                            LV_PART_MAIN);
    lv_obj_set_style_bg_color(chip,
                              (style == ICON_STYLE_HOME_CHIP)
                                  ? lv_color_hex(COMP_TAB_MENU_ICON_DARK_BG_COLOR_HEX)
                                  : lv_color_hex(COMP_TAB_MENU_ICON_LIGHT_BG_COLOR_HEX),
                              LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chip, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(
        chip, (style == ICON_STYLE_HOME_CHIP) ? 0
                                                : COMP_TAB_MENU_ICON_LIGHT_BORDER_WIDTH_PX,
        LV_PART_MAIN);
    lv_obj_set_style_border_color(
        chip, lv_color_hex(COMP_TAB_MENU_ICON_LIGHT_BORDER_COLOR_HEX),
        LV_PART_MAIN);
    lv_obj_set_style_shadow_width(chip, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(chip, 0, LV_PART_MAIN);
  }
  lv_obj_clear_flag(chip, LV_OBJ_FLAG_SCROLLABLE);

  if (style == ICON_STYLE_HOME_CHIP) {
    icon_color = lv_color_hex(COMP_TAB_MENU_ICON_DARK_COLOR_HEX);
  } else if (style == ICON_STYLE_SETTING_CHIP) {
    icon_color = lv_color_hex(COMP_TAB_MENU_ICON_LIGHT_COLOR_HEX);
  } else {
    icon_color = lv_color_hex(COMP_TAB_MENU_ICON_COLOR_HEX);
  }

  icon = lv_label_create(chip);
  lv_label_set_text(icon, symbol);
  lv_obj_set_style_text_color(icon, icon_color, LV_PART_MAIN);
  lv_obj_set_style_text_font(icon, COMP_TAB_MENU_ICON_FONT, LV_PART_MAIN);
  lv_obj_center(icon);

  return chip;
}

static lv_obj_t *create_segment_btn(lv_obj_t *parent, const char *text, bool selected) {
  lv_obj_t *btn;
  lv_obj_t *label;

  btn = lv_btn_create(parent);
  lv_obj_set_size(btn, COMP_TAB_MENU_SEGMENT_BTN_WIDTH_PX,
                  COMP_TAB_MENU_SEGMENT_BTN_HEIGHT_PX);
  lv_obj_set_style_radius(btn, COMP_TAB_MENU_SEGMENT_BTN_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn,
                            selected ? lv_color_hex(COMP_TAB_MENU_SEGMENT_SELECTED_BG_COLOR_HEX)
                                     : lv_color_hex(COMP_TAB_MENU_SEGMENT_UNSELECTED_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(btn);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_color(label,
                              selected
                                  ? lv_color_hex(COMP_TAB_MENU_SEGMENT_SELECTED_TEXT_COLOR_HEX)
                                  : lv_color_hex(
                                        COMP_TAB_MENU_SEGMENT_UNSELECTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(label, COMP_TAB_MENU_SEGMENT_FONT, LV_PART_MAIN);
  lv_obj_center(label);

  return btn;
}

static void create_main_menu(lv_obj_t *menu, comp_tab_menu_mode_t mode,
                             comp_tab_menu_refs_t *out_refs) {
  lv_obj_t *left;
  lv_obj_t *title;
  lv_obj_t *spacer;
  lv_obj_t *right;
  lv_obj_t *segment;

  /* Keep back hit-area limited to the visual left cluster only. */
  left = create_transparent_flex_row(menu, false, HL_SPACE_SM);

  /* Left home icon and right setting icon are intentionally styled by separate knobs. */
  (void)create_icon_chip(left, LV_SYMBOL_HOME, ICON_STYLE_HOME_CHIP);

  title = lv_label_create(left);
  if (mode == COMP_TAB_MENU_MODE_MAIN_HOME) {
    s_home_title_label = title;
    lv_label_set_text(title, s_home_title_text);
  } else {
    s_health_title_label = title;
    lv_label_set_text(title, s_health_title_text);
  }
  lv_obj_set_style_text_color(title, lv_color_hex(COMP_TAB_MENU_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title, COMP_TAB_MENU_TEXT_FONT, LV_PART_MAIN);

  /* Keep main-page segmented tabs anchored to the far right. */
  spacer = create_transparent_flex_row(menu, true, 0);
  lv_obj_set_size(spacer, 0, 0);

  right = create_transparent_flex_row(menu, false, HL_SPACE_SM);

  segment = lv_obj_create(right);
  lv_obj_set_size(segment, LV_SIZE_CONTENT, COMP_TAB_MENU_SEGMENT_HEIGHT_PX);
  lv_obj_set_style_bg_color(segment, lv_color_hex(COMP_TAB_MENU_SEGMENT_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(segment, COMP_TAB_MENU_SEGMENT_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(segment, COMP_TAB_MENU_SEGMENT_BORDER_WIDTH_PX,
                                LV_PART_MAIN);
  lv_obj_set_style_border_color(segment,
                                lv_color_hex(COMP_TAB_MENU_SEGMENT_BORDER_COLOR_HEX),
                                LV_PART_MAIN);
  lv_obj_set_style_pad_all(segment, HL_SPACE_XS, LV_PART_MAIN);
  lv_obj_set_style_pad_column(segment, HL_SPACE_XS, LV_PART_MAIN);
  lv_obj_set_layout(segment, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(segment, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(segment, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(segment, LV_OBJ_FLAG_SCROLLABLE);

  out_refs->btn_health = create_segment_btn(segment, "Health",
                                            mode == COMP_TAB_MENU_MODE_MAIN_HEALTH);
  out_refs->btn_home = create_segment_btn(segment, "Home",
                                          mode == COMP_TAB_MENU_MODE_MAIN_HOME);
  out_refs->btn_setting =
      create_icon_chip(right, LV_SYMBOL_SETTINGS, ICON_STYLE_SETTING_CHIP);
}

static void create_setting_menu(lv_obj_t *menu, comp_tab_menu_refs_t *out_refs) {
  lv_obj_t *left;
  lv_obj_t *title;

  left = create_transparent_flex_row(menu, false, HL_SPACE_SM);

  out_refs->btn_back = create_icon_chip(left, LV_SYMBOL_LEFT, ICON_STYLE_PLAIN);

  title = lv_label_create(left);
  lv_label_set_text(title, "Setting");
  lv_obj_set_style_text_color(title, lv_color_hex(COMP_TAB_MENU_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title, COMP_TAB_MENU_TEXT_FONT, LV_PART_MAIN);
}

static lv_obj_t *create_action_btn(lv_obj_t *parent) {
  lv_obj_t *btn;
  lv_obj_t *label;

  if (NULL == parent) {
    return NULL;
  }

  btn = lv_button_create(parent);
  lv_obj_set_size(btn, COMP_TAB_MENU_ACTION_BTN_WIDTH_PX,
                  COMP_TAB_MENU_ACTION_BTN_HEIGHT_PX);
  lv_obj_set_style_radius(btn, COMP_TAB_MENU_ACTION_BTN_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, lv_color_hex(COMP_TAB_MENU_ACTION_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(btn);
  lv_label_set_text(label, COMP_TAB_MENU_ACTION_BTN_LABEL_TEXT);
  lv_obj_set_style_text_font(label, COMP_TAB_MENU_ACTION_BTN_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(label,
                              lv_color_hex(COMP_TAB_MENU_ACTION_BTN_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(label);
  return btn;
}

static void create_user_menu(lv_obj_t *menu, comp_tab_menu_refs_t *out_refs,
                             bool with_action_btn) {
  lv_obj_t *left;
  lv_obj_t *spacer;
  lv_obj_t *right;
  lv_obj_t *avatar;
  lv_obj_t *name_col;
  lv_obj_t *name;
  lv_obj_t *age;

  left = create_transparent_flex_row(menu, false, HL_SPACE_SM);

  /* User-detail mode: keep back hit-area limited to left profile cluster only. */
  lv_obj_add_flag(left, LV_OBJ_FLAG_CLICKABLE);
  out_refs->btn_back = left;
#if COMP_TAB_MENU_DEBUG_HITAREA_ENABLE
  lv_obj_set_style_bg_color(
      left, lv_color_hex(COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_bg_opa(left, COMP_TAB_MENU_DEBUG_HITAREA_LEFT_BG_OPA,
                          LV_PART_MAIN);
  lv_obj_set_style_radius(left, COMP_TAB_MENU_DEBUG_HITAREA_LEFT_RADIUS_PX,
                          LV_PART_MAIN);
#endif

  (void)create_icon_chip(left, LV_SYMBOL_LEFT, ICON_STYLE_PLAIN);

  avatar = lv_obj_create(left);
  lv_obj_set_size(avatar, COMP_TAB_MENU_AVATAR_SIZE_PX,
                  COMP_TAB_MENU_AVATAR_SIZE_PX);
  lv_obj_set_style_radius(avatar, COMP_TAB_MENU_AVATAR_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(avatar, lv_color_hex(s_user_detail_avatar_bg_color_hex),
                            LV_PART_MAIN);
  lv_obj_set_style_border_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(avatar, 0, LV_PART_MAIN);
  lv_obj_clear_flag(avatar, LV_OBJ_FLAG_SCROLLABLE);
  {
    lv_obj_t *avatar_icon = lv_label_create(avatar);
    lv_label_set_text(avatar_icon, LV_SYMBOL_IMAGE);
    lv_obj_set_style_text_color(avatar_icon,
                                lv_color_hex(COMP_TAB_MENU_AVATAR_ICON_COLOR_HEX),
                                LV_PART_MAIN);
    lv_obj_center(avatar_icon);
  }

  name_col = lv_obj_create(left);
  lv_obj_set_size(name_col, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(name_col, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(name_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(name_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(name_col, COMP_TAB_MENU_NAME_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(name_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(name_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_clear_flag(name_col, LV_OBJ_FLAG_SCROLLABLE);

  name = lv_label_create(name_col);
  lv_label_set_text(name, s_user_detail_name_text);
  lv_obj_set_style_text_color(name, lv_color_hex(COMP_TAB_MENU_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(name, COMP_TAB_MENU_TEXT_FONT, LV_PART_MAIN);

  age = lv_label_create(name_col);
  lv_label_set_text(age, s_user_detail_age_text);
  lv_obj_set_style_text_color(age, lv_color_hex(COMP_TAB_MENU_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(age, COMP_TAB_MENU_TEXT_FONT, LV_PART_MAIN);
  register_user_profile_binding(avatar, name, age);

  add_event_bubble_recursive(left);

  if (with_action_btn) {
    /* Keep back hit-area only on left cluster while pushing action button to far right. */
    spacer = create_transparent_flex_row(menu, true, 0);
    lv_obj_set_size(spacer, 0, 0);

    right = create_transparent_flex_row(menu, false, 0);
    out_refs->btn_action = create_action_btn(right);
  }
}

lv_obj_t *comp_tab_menu_create(lv_obj_t *parent, comp_tab_menu_mode_t mode,
                               comp_tab_menu_refs_t *out_refs) {
  lv_obj_t *menu;
  comp_tab_menu_refs_t local_refs;
  comp_tab_menu_refs_t *refs;

  if (NULL == parent) {
    return NULL;
  }

  refs = (NULL != out_refs) ? out_refs : &local_refs;
  refs->btn_health = NULL;
  refs->btn_home = NULL;
  refs->btn_setting = NULL;
  refs->btn_back = NULL;
  refs->btn_action = NULL;

  /* One fixed-height menu shell is reused across all tab-menu modes. */
  menu = lv_obj_create(parent);
  lv_obj_set_size(menu, lv_pct(100), COMP_TAB_MENU_HEIGHT_PX);
  lv_obj_set_style_bg_color(menu, lv_color_hex(COMP_TAB_MENU_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(menu, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(menu, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(menu, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_style_pad_right(menu, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_style_pad_top(menu, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(menu, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(menu, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_layout(menu, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(menu, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(menu, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(menu, LV_OBJ_FLAG_SCROLLABLE);

  if ((mode == COMP_TAB_MENU_MODE_MAIN_HEALTH) ||
      (mode == COMP_TAB_MENU_MODE_MAIN_HOME)) {
    create_main_menu(menu, mode, refs);
  } else if (mode == COMP_TAB_MENU_MODE_SETTING) {
    create_setting_menu(menu, refs);
  } else if (mode == COMP_TAB_MENU_MODE_METRIC_DETAIL) {
    create_user_menu(menu, refs, true);
  } else {
    create_user_menu(menu, refs, false);
  }

  return menu;
}

void comp_tab_menu_set_family_summary(const char *family_name,
                                      uint32_t member_count) {
  const char *name;

  name = ((NULL != family_name) && (family_name[0] != '\0')) ? family_name
                                                              : "Family";
  /* Keep both labels synchronized so mode switches never show stale family text. */
  (void)snprintf(s_health_title_text, sizeof(s_health_title_text), "%s : %lu",
                 name, (unsigned long)member_count);
  (void)snprintf(s_home_title_text, sizeof(s_home_title_text), "%s Home", name);

  /* Menu instances are recreated during page switches; validate before writing. */
  update_label_if_valid(&s_health_title_label, s_health_title_text);
  update_label_if_valid(&s_home_title_label, s_home_title_text);
}

void comp_tab_menu_set_user_detail_profile(const char *user_name,
                                           uint32_t age_years,
                                           uint32_t avatar_bg_color_hex) {
  uint32_t i;
  const char *name;

  name = ((NULL != user_name) && (user_name[0] != '\0')) ? user_name : "Member";

  (void)snprintf(s_user_detail_name_text, sizeof(s_user_detail_name_text), "%s",
                 name);
  if (age_years > 0U) {
    (void)snprintf(s_user_detail_age_text, sizeof(s_user_detail_age_text),
                   "Age: %lu", (unsigned long)age_years);
  } else {
    (void)snprintf(s_user_detail_age_text, sizeof(s_user_detail_age_text),
                   "Age: --");
  }

  s_user_detail_avatar_bg_color_hex =
      (0U != avatar_bg_color_hex) ? avatar_bg_color_hex
                                  : COMP_TAB_MENU_AVATAR_BG_COLOR_HEX;

  compact_user_profile_bindings();
  for (i = 0U; i < s_user_profile_binding_count; i++) {
    update_label_if_valid(&s_user_profile_bindings[i].name_label,
                          s_user_detail_name_text);
    update_label_if_valid(&s_user_profile_bindings[i].age_label,
                          s_user_detail_age_text);
    update_bg_color_if_valid(&s_user_profile_bindings[i].avatar,
                             s_user_detail_avatar_bg_color_hex);
  }
}
