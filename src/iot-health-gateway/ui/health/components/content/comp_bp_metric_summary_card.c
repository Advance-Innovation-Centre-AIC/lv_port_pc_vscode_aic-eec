#include "comp_bp_metric_summary_card.h"

static lv_color_t status_bg_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_WARNING_BG_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_CRITICAL_BG_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_NORMAL_BG_HEX);
  }
}

static lv_color_t card_bg_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BG_WARNING_COLOR_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BG_CRITICAL_COLOR_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BG_COLOR_HEX);
  }
}

static lv_color_t card_border_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BORDER_WARNING_COLOR_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BORDER_CRITICAL_COLOR_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_BG_COLOR_HEX);
  }
}

static lv_coord_t card_border_width(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
  case COMP_USER_STATUS_CRITICAL:
    return COMP_BP_SUMMARY_CARD_BORDER_WIDTH_PX;
  case COMP_USER_STATUS_NORMAL:
  default:
    return 0;
  }
}

static lv_color_t status_text_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_WARNING_TEXT_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_CRITICAL_TEXT_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(COMP_BP_SUMMARY_CARD_STATUS_NORMAL_TEXT_HEX);
  }
}

static lv_obj_t *create_transparent_row(lv_obj_t *parent, lv_coord_t col_gap,
                                        lv_flex_align_t main_align) {
  lv_obj_t *row = lv_obj_create(parent);
  lv_obj_set_size(row, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(row, col_gap, LV_PART_MAIN);
  lv_obj_set_layout(row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, main_align, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
  return row;
}

lv_obj_t *comp_bp_metric_summary_card_create(
    lv_obj_t *parent, const comp_bp_metric_summary_card_data_t *data) {
  lv_obj_t *card;
  lv_obj_t *top_row;
  lv_obj_t *left_col;
  lv_obj_t *label_row;
  lv_obj_t *dot;
  lv_obj_t *label;
  lv_obj_t *unit;
  lv_obj_t *value;
  lv_obj_t *status_chip;
  lv_obj_t *status_label;
  const char *label_text = "Metric";
  const char *unit_text = "--";
  const char *value_text = "--";
  const char *status_text = "Normal";
  uint32_t accent_color_hex = 0x3F7FD1;
  comp_user_status_level_t status_level = COMP_USER_STATUS_NORMAL;

  if (NULL == parent) {
    return NULL;
  }

  if (NULL != data) {
    if ((NULL != data->label_text) && (data->label_text[0] != '\0')) {
      label_text = data->label_text;
    }
    if ((NULL != data->unit_text) && (data->unit_text[0] != '\0')) {
      unit_text = data->unit_text;
    }
    if ((NULL != data->value_text) && (data->value_text[0] != '\0')) {
      value_text = data->value_text;
    }
    if ((NULL != data->status_text) && (data->status_text[0] != '\0')) {
      status_text = data->status_text;
    }
    if (0U != data->accent_color_hex) {
      accent_color_hex = data->accent_color_hex;
    }
    status_level = data->status_level;
  }

  card = lv_obj_create(parent);
  lv_obj_set_size(card, 0, lv_pct(100));
  lv_obj_set_flex_grow(card, 1);
  lv_obj_set_style_bg_color(card, card_bg_color(status_level), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(card, COMP_BP_SUMMARY_CARD_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, card_border_color(status_level), LV_PART_MAIN);
  lv_obj_set_style_border_width(card, card_border_width(status_level), LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(card, COMP_BP_SUMMARY_CARD_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(card, COMP_BP_SUMMARY_CARD_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(card, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  top_row = create_transparent_row(card, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);

  left_col = lv_obj_create(top_row);
  lv_obj_set_size(left_col, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(left_col, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(left_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(left_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(left_col, 2, LV_PART_MAIN);
  lv_obj_set_layout(left_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(left_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(left_col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(left_col, LV_OBJ_FLAG_SCROLLABLE);

  label_row = create_transparent_row(left_col, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_width(label_row, LV_SIZE_CONTENT);

  dot = lv_obj_create(label_row);
  lv_obj_set_size(dot, COMP_BP_SUMMARY_CARD_DOT_SIZE_PX, COMP_BP_SUMMARY_CARD_DOT_SIZE_PX);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(dot, lv_color_hex(accent_color_hex), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN);
  lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(label_row);
  lv_label_set_text(label, label_text);
  lv_obj_set_style_text_font(label, COMP_BP_SUMMARY_CARD_METRIC_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(label, lv_color_hex(COMP_BP_SUMMARY_CARD_LABEL_COLOR_HEX),
                              LV_PART_MAIN);

  unit = lv_label_create(left_col);
  lv_label_set_text(unit, unit_text);
  lv_obj_set_style_text_font(unit, COMP_BP_SUMMARY_CARD_UNIT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(unit, lv_color_hex(COMP_BP_SUMMARY_CARD_UNIT_COLOR_HEX),
                              LV_PART_MAIN);

  value = lv_label_create(top_row);
  lv_label_set_text(value, value_text);
  lv_obj_set_style_text_font(value, COMP_BP_SUMMARY_CARD_VALUE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(value, lv_color_hex(COMP_BP_SUMMARY_CARD_VALUE_COLOR_HEX),
                              LV_PART_MAIN);

  status_chip = lv_obj_create(card);
  lv_obj_set_size(status_chip, lv_pct(100), COMP_BP_SUMMARY_CARD_STATUS_HEIGHT_PX);
  lv_obj_set_style_bg_color(status_chip, status_bg_color(status_level), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(status_chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(status_chip, COMP_BP_SUMMARY_CARD_STATUS_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(status_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(status_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(status_chip, 0, LV_PART_MAIN);
  lv_obj_clear_flag(status_chip, LV_OBJ_FLAG_SCROLLABLE);

  status_label = lv_label_create(status_chip);
  lv_label_set_text(status_label, status_text);
  lv_obj_set_style_text_font(status_label, COMP_BP_SUMMARY_CARD_STATUS_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(status_label, status_text_color(status_level), LV_PART_MAIN);
  lv_obj_center(status_label);

  return card;
}
