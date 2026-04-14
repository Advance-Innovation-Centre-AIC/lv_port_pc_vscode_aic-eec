#include "comp_user_card.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum {
  CHIP_STYLE_NORMAL = 0,
  CHIP_STYLE_WARNING = 1,
  CHIP_STYLE_CRITICAL = 2,
  CHIP_STYLE_MORE = 3
} chip_style_t;

static const char *get_condition_text(comp_user_condition_t condition) {
  switch (condition) {
  case COMP_USER_CONDITION_LOW_FEVER:
    return "Low fever";
  case COMP_USER_CONDITION_HIGH_FEVER:
    return "High fever";
  case COMP_USER_CONDITION_PREDIABETES:
    return "Prediabetes";
  case COMP_USER_CONDITION_NORMAL:
  default:
    return "Normal";
  }
}

static chip_style_t get_chip_style(comp_user_condition_t condition) {
  switch (condition) {
  case COMP_USER_CONDITION_LOW_FEVER:
  case COMP_USER_CONDITION_PREDIABETES:
    return CHIP_STYLE_WARNING;
  case COMP_USER_CONDITION_HIGH_FEVER:
    return CHIP_STYLE_CRITICAL;
  case COMP_USER_CONDITION_NORMAL:
  default:
    return CHIP_STYLE_NORMAL;
  }
}

static lv_color_t get_status_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(COMP_USER_CARD_METRIC_STATUS_WARNING_COLOR_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(COMP_USER_CARD_METRIC_STATUS_CRITICAL_COLOR_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(COMP_USER_CARD_METRIC_STATUS_NORMAL_COLOR_HEX);
  }
}

static const char *get_metric_title(comp_user_metric_type_t type) {
  switch (type) {
  case COMP_USER_METRIC_BODY_TEMP:
    return "Body Temp";
  case COMP_USER_METRIC_BLOOD_GLUCOSE:
    return "Blood Glucose";
  case COMP_USER_METRIC_BLOOD_PRESSURE:
  default:
    return "Blood Pressure";
  }
}

static const char *get_metric_icon_text(comp_user_metric_type_t type) {
  switch (type) {
  case COMP_USER_METRIC_BODY_TEMP:
    return "T";
  case COMP_USER_METRIC_BLOOD_GLUCOSE:
    return "G";
  case COMP_USER_METRIC_BLOOD_PRESSURE:
  default:
    return "B";
  }
}

static void style_chip(lv_obj_t *chip, chip_style_t style) {
  lv_color_t bg_color;
  lv_color_t fg_color;
  lv_color_t border_color = lv_color_hex(0x000000);
  uint32_t border_width = 0U;

  switch (style) {
  case CHIP_STYLE_WARNING:
    bg_color = lv_color_hex(COMP_USER_CARD_CHIP_BG_COLOR_WARNING_HEX);
    fg_color = lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_WARNING_HEX);
    border_color = lv_color_hex(COMP_USER_CARD_CHIP_BORDER_COLOR_WARNING_HEX);
    border_width = 1U;
    break;
  case CHIP_STYLE_CRITICAL:
    bg_color = lv_color_hex(COMP_USER_CARD_CHIP_BG_COLOR_CRITICAL_HEX);
    fg_color = lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_CRITICAL_HEX);
    border_color = lv_color_hex(COMP_USER_CARD_CHIP_BORDER_COLOR_CRITICAL_HEX);
    border_width = 1U;
    break;
  case CHIP_STYLE_MORE:
    bg_color = lv_color_hex(COMP_USER_CARD_CHIP_BG_COLOR_MORE_HEX);
    fg_color = lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_MORE_HEX);
    break;
  case CHIP_STYLE_NORMAL:
  default:
    bg_color = lv_color_hex(COMP_USER_CARD_CHIP_BG_COLOR_NORMAL_HEX);
    fg_color = lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_NORMAL_HEX);
    break;
  }

  lv_obj_set_height(chip, COMP_USER_CARD_CHIP_HEIGHT_PX);
  lv_obj_set_style_radius(chip, COMP_USER_CARD_CHIP_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(chip, bg_color, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(chip, border_width, LV_PART_MAIN);
  lv_obj_set_style_border_color(chip, border_color, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(chip, COMP_USER_CARD_CHIP_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(chip, COMP_USER_CARD_CHIP_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(chip, COMP_USER_CARD_CHIP_PAD_VER_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(chip, COMP_USER_CARD_CHIP_PAD_VER_PX, LV_PART_MAIN);
  lv_obj_set_style_text_color(chip, fg_color, LV_PART_MAIN);
  lv_obj_set_style_text_font(chip, COMP_USER_CARD_CHIP_FONT, LV_PART_MAIN);
}

static lv_color_t get_chip_text_color(chip_style_t style) {
  switch (style) {
  case CHIP_STYLE_WARNING:
    return lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_WARNING_HEX);
  case CHIP_STYLE_CRITICAL:
    return lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_CRITICAL_HEX);
  case CHIP_STYLE_MORE:
    return lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_MORE_HEX);
  case CHIP_STYLE_NORMAL:
  default:
    return lv_color_hex(COMP_USER_CARD_CHIP_TEXT_COLOR_NORMAL_HEX);
  }
}

static lv_obj_t *create_condition_chip(lv_obj_t *parent, const char *text,
                                       chip_style_t style) {
  lv_obj_t *chip;
  lv_obj_t *label;

  chip = lv_btn_create(parent);
  style_chip(chip, style);
  lv_obj_clear_flag(chip, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(chip);
  lv_label_set_text(label, (NULL != text) ? text : "");
  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_color(label, get_chip_text_color(style), LV_PART_MAIN);
  lv_obj_set_style_text_font(label, COMP_USER_CARD_CHIP_FONT, LV_PART_MAIN);
  lv_obj_center(label);

  return chip;
}

static void populate_conditions(lv_obj_t *container,
                                const comp_user_condition_t *conditions,
                                uint32_t condition_count) {
  uint32_t i;
  uint32_t visible_actual;

  if (NULL == container) {
    return;
  }

  if ((NULL == conditions) || (0U == condition_count)) {
    (void)create_condition_chip(container, get_condition_text(COMP_USER_CONDITION_NORMAL),
                                CHIP_STYLE_NORMAL);
    return;
  }

  visible_actual = condition_count;
  if ((COMP_USER_CARD_MAX_VISIBLE_CHIPS > 0U) &&
      (visible_actual > COMP_USER_CARD_MAX_VISIBLE_CHIPS)) {
    visible_actual = COMP_USER_CARD_MAX_VISIBLE_CHIPS;
  }

  /* Keep visual width stable: reserve one rendered slot for "+N more". */
  if ((condition_count > visible_actual) && (visible_actual > 0U) &&
      (COMP_USER_CARD_ENABLE_MORE_CHIP != 0U) &&
      (COMP_USER_CARD_MORE_CHIP_USES_VISIBLE_SLOT != 0U)) {
    visible_actual -= 1U;
  }

  for (i = 0U; i < visible_actual; i++) {
    const char *text = get_condition_text(conditions[i]);
    (void)create_condition_chip(container, text, get_chip_style(conditions[i]));
  }

  if ((COMP_USER_CARD_ENABLE_MORE_CHIP != 0U) &&
      (condition_count > visible_actual)) {
    char more_text[24];
    uint32_t remain_count = condition_count - visible_actual;
    lv_obj_t *more_chip;

    (void)snprintf(more_text, sizeof(more_text), "+%lu more",
                   (unsigned long)remain_count);
    more_chip = create_condition_chip(container, more_text, CHIP_STYLE_MORE);
    lv_obj_clear_flag(more_chip, LV_OBJ_FLAG_CLICKABLE);
  }
}

static void create_metric_panel(lv_obj_t *card,
                                const comp_user_metric_preview_t *metric) {
  lv_obj_t *panel;
  lv_obj_t *top_row;
  lv_obj_t *left_group;
  lv_obj_t *icon_bg;
  lv_obj_t *icon_label;
  lv_obj_t *title;
  lv_obj_t *time_label;
  lv_obj_t *bottom_row;
  lv_obj_t *value_group;
  lv_obj_t *value_label;
  lv_obj_t *unit_label;
  lv_obj_t *status_group;
  lv_obj_t *dot_label;
  lv_obj_t *status_label;

  panel = lv_obj_create(card);
  lv_obj_set_size(panel, lv_pct(100), COMP_USER_CARD_METRIC_PANEL_HEIGHT_PX);
  lv_obj_set_style_radius(panel, COMP_USER_CARD_METRIC_PANEL_RADIUS_PX,
                          LV_PART_MAIN);
  lv_obj_set_style_bg_color(
      panel, lv_color_hex(COMP_USER_CARD_METRIC_PANEL_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(panel, COMP_USER_CARD_METRIC_PANEL_PAD_HOR_PX,
                            LV_PART_MAIN);
  lv_obj_set_style_pad_right(panel, COMP_USER_CARD_METRIC_PANEL_PAD_HOR_PX,
                             LV_PART_MAIN);
  lv_obj_set_style_pad_top(panel, COMP_USER_CARD_METRIC_PANEL_PAD_TOP_PX,
                           LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(panel, COMP_USER_CARD_METRIC_PANEL_PAD_BOTTOM_PX,
                              LV_PART_MAIN);
  lv_obj_set_style_pad_row(panel, COMP_USER_CARD_METRIC_PANEL_ROW_GAP_PX,
                           LV_PART_MAIN);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
  /* Center top/bottom metric rows along Y-axis inside the fixed-height panel. */
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  top_row = lv_obj_create(panel);
  lv_obj_set_size(top_row, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(top_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(top_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(top_row, COMP_USER_CARD_METRIC_TOP_ROW_GAP_PX,
                              LV_PART_MAIN);
  lv_obj_set_layout(top_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(top_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(top_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(top_row, LV_OBJ_FLAG_SCROLLABLE);

  left_group = lv_obj_create(top_row);
  lv_obj_set_size(left_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(left_group, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(left_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(left_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(left_group, COMP_USER_CARD_METRIC_LEFT_GROUP_GAP_PX,
                              LV_PART_MAIN);
  lv_obj_set_layout(left_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(left_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(left_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_flex_grow(left_group, 1);
  lv_obj_clear_flag(left_group, LV_OBJ_FLAG_SCROLLABLE);

  icon_bg = lv_obj_create(left_group);
  lv_obj_set_size(icon_bg, COMP_USER_CARD_METRIC_ICON_SIZE_PX,
                  COMP_USER_CARD_METRIC_ICON_SIZE_PX);
  lv_obj_set_style_radius(icon_bg, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(icon_bg, lv_color_hex(COMP_USER_CARD_METRIC_ICON_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(icon_bg, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(icon_bg, 0, LV_PART_MAIN);
  lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);

  icon_label = lv_label_create(icon_bg);
  lv_label_set_text(icon_label, get_metric_icon_text(metric->type));
  lv_obj_set_style_text_color(
      icon_label, lv_color_hex(COMP_USER_CARD_METRIC_ICON_TEXT_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_text_font(icon_label, COMP_USER_CARD_METRIC_ICON_FONT,
                             LV_PART_MAIN);
  lv_obj_center(icon_label);

  title = lv_label_create(left_group);
  lv_label_set_text(title, get_metric_title(metric->type));
  lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
  lv_obj_set_width(title, COMP_USER_CARD_METRIC_TITLE_MAX_WIDTH_PX);
  lv_obj_set_style_text_color(title, lv_color_hex(COMP_USER_CARD_METRIC_TITLE_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title, COMP_USER_CARD_METRIC_TITLE_FONT,
                             LV_PART_MAIN);

  time_label = lv_label_create(top_row);
  lv_label_set_text(time_label, (NULL != metric->time_text) ? metric->time_text : "--:--");
  lv_label_set_long_mode(time_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(time_label, COMP_USER_CARD_METRIC_TIME_MIN_WIDTH_PX);
  lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_set_style_text_color(time_label,
                              lv_color_hex(COMP_USER_CARD_METRIC_MUTED_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(time_label, COMP_USER_CARD_METRIC_TIME_FONT,
                             LV_PART_MAIN);

  bottom_row = lv_obj_create(panel);
  lv_obj_set_size(bottom_row, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(bottom_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(bottom_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(bottom_row, 0, LV_PART_MAIN);
  lv_obj_set_layout(bottom_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(bottom_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(bottom_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_END,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(bottom_row, LV_OBJ_FLAG_SCROLLABLE);

  value_group = lv_obj_create(bottom_row);
  lv_obj_set_size(value_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(value_group, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(value_group, COMP_USER_CARD_METRIC_VALUE_GROUP_GAP_PX,
                              LV_PART_MAIN);
  lv_obj_set_layout(value_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(value_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(value_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(value_group, LV_OBJ_FLAG_SCROLLABLE);

  value_label = lv_label_create(value_group);
  lv_label_set_text(value_label, (NULL != metric->value_text) ? metric->value_text : "--");
  lv_obj_set_style_text_color(value_label,
                              lv_color_hex(COMP_USER_CARD_METRIC_VALUE_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(value_label, COMP_USER_CARD_METRIC_VALUE_FONT,
                             LV_PART_MAIN);

  unit_label = lv_label_create(value_group);
  lv_label_set_text(unit_label, (NULL != metric->unit_text) ? metric->unit_text : "");
  lv_obj_set_style_text_color(unit_label,
                              lv_color_hex(COMP_USER_CARD_METRIC_MUTED_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(unit_label, COMP_USER_CARD_METRIC_UNIT_FONT,
                             LV_PART_MAIN);

  status_group = lv_obj_create(bottom_row);
  /* Keep a fixed right-side slot so bottom-row status aligns like top-row time. */
  lv_obj_set_size(status_group, COMP_USER_CARD_METRIC_STATUS_GROUP_WIDTH_PX,
                  LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(status_group, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(status_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(status_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(status_group, COMP_USER_CARD_METRIC_STATUS_GROUP_LEFT_PAD_PX,
                            LV_PART_MAIN);
  lv_obj_set_style_pad_column(status_group, COMP_USER_CARD_METRIC_STATUS_GROUP_GAP_PX,
                              LV_PART_MAIN);
  lv_obj_set_layout(status_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(status_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(status_group, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(status_group, LV_OBJ_FLAG_SCROLLABLE);

  dot_label = lv_label_create(status_group);
  lv_label_set_text(dot_label, LV_SYMBOL_BULLET);
  lv_obj_set_style_text_color(dot_label, get_status_color(metric->status_level),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(dot_label, COMP_USER_CARD_METRIC_STATUS_FONT,
                             LV_PART_MAIN);

  status_label = lv_label_create(status_group);
  lv_label_set_text(status_label,
                    (NULL != metric->status_text) ? metric->status_text : "Normal");
  /* Keep label content-sized so bullet can sit tight to the status text. */
  lv_label_set_long_mode(status_label, LV_LABEL_LONG_CLIP);
  lv_obj_set_style_text_color(status_label, get_status_color(metric->status_level),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(status_label, COMP_USER_CARD_METRIC_STATUS_FONT,
                             LV_PART_MAIN);
}

lv_obj_t *comp_user_card_create(lv_obj_t *parent,
                                const comp_user_card_data_t *data) {
  lv_obj_t *card;
  lv_obj_t *avatar;
  lv_obj_t *name_label;
  lv_obj_t *chip_row;
  const char *name_text = "Member";
  comp_user_metric_preview_t metric = {
      .type = COMP_USER_METRIC_BLOOD_PRESSURE,
      .value_text = "--",
      .unit_text = "",
      .status_text = "Normal",
      .time_text = "--:--",
      .status_level = COMP_USER_STATUS_NORMAL,
  };
  uint32_t avatar_bg = COMP_USER_CARD_AVATAR_BG_COLOR_HEX;
  const comp_user_condition_t *conditions = NULL;
  uint32_t condition_count = 0U;

  if (NULL == parent) {
    return NULL;
  }

  if (NULL != data) {
    name_text = (NULL != data->name) ? data->name : name_text;
    /* Card background is intentionally fixed by UX spec in this phase. */
    (void)data->card_bg_color_hex;
    avatar_bg = (0U != data->avatar_bg_color_hex) ? data->avatar_bg_color_hex
                                                   : avatar_bg;
    conditions = data->conditions;
    condition_count = data->condition_count;
    metric = data->metric;
  }

  card = lv_obj_create(parent);
  lv_obj_set_width(card, lv_pct(COMP_USER_CARD_WIDTH_PCT));
  lv_obj_set_height(card, lv_pct(100));
  lv_obj_set_style_radius(card, COMP_USER_CARD_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, lv_color_hex(COMP_USER_CARD_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(card, COMP_USER_CARD_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(card, COMP_USER_CARD_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(card, COMP_USER_CARD_PAD_TOP_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(card, COMP_USER_CARD_PAD_BOTTOM_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(card, COMP_USER_CARD_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_style_clip_corner(card, true, LV_PART_MAIN);
  lv_obj_set_layout(card, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(card, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  avatar = lv_obj_create(card);
  lv_obj_set_size(avatar, COMP_USER_CARD_AVATAR_SIZE_PX,
                  COMP_USER_CARD_AVATAR_SIZE_PX);
  lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(avatar, lv_color_hex(avatar_bg), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(avatar, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_margin_top(avatar, COMP_USER_CARD_AVATAR_TOP_OFFSET_PX,
                              LV_PART_MAIN);
  lv_obj_clear_flag(avatar, LV_OBJ_FLAG_SCROLLABLE);

  name_label = lv_label_create(card);
  lv_label_set_text(name_label, name_text);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(name_label, lv_pct(100));
  lv_obj_set_style_text_align(name_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_color(name_label, lv_color_hex(COMP_USER_CARD_NAME_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(name_label, COMP_USER_CARD_NAME_FONT, LV_PART_MAIN);

  chip_row = lv_obj_create(card);
  lv_obj_set_width(chip_row, lv_pct(100));
  lv_obj_set_height(chip_row, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(chip_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(chip_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chip_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(chip_row, COMP_USER_CARD_CHIP_ROW_GAP_PX,
                              LV_PART_MAIN);
  lv_obj_set_layout(chip_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(chip_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(chip_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(chip_row, LV_OBJ_FLAG_SCROLLABLE);

  populate_conditions(chip_row, conditions, condition_count);
  create_metric_panel(card, &metric);

  return card;
}
