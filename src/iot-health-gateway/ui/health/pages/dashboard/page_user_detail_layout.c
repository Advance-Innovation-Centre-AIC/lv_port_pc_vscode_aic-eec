#include "page_user_detail_layout.h"

#include "../../components/header/comp_tab_menu.h"
#include "../../health_ui_root.h"
#include "../../layout/layout_scaffold.h"
#include "../../health_ui_data_adapter.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_DETAIL_TITLE_TEXT_COLOR_HEX (0x002329)
#define USER_DETAIL_MUTED_TEXT_COLOR_HEX (0x8A94A2)
#define USER_DETAIL_ICON_BG_COLOR_HEX (0xE5E7EB)
#define USER_DETAIL_STATUS_NORMAL_BG_HEX (0xFFFFFF)
#define USER_DETAIL_STATUS_NORMAL_TEXT_HEX (0x8A94A2)
#define USER_DETAIL_STATUS_WARNING_BG_HEX (0xF8E8AF)
#define USER_DETAIL_STATUS_WARNING_TEXT_HEX (0x8A5A00)
#define USER_DETAIL_STATUS_CRITICAL_BG_HEX (0xF3B8B4)
#define USER_DETAIL_STATUS_CRITICAL_TEXT_HEX (0xB42318)
#define USER_DETAIL_TREND_ACTIVE_COLOR_HEX (0x475467)
#define USER_DETAIL_TREND_MUTED_COLOR_HEX (0xD0D5DD)

#define USER_DETAIL_BP_CARD_GROW (3)
#define USER_DETAIL_GLUCOSE_CARD_GROW (1)

static lv_obj_t *s_content_root = NULL;
static lv_obj_t *s_loading_icon = NULL;
static uint32_t s_selected_member_index = 0U;
static bool s_loading_icon_visible =
    ((USER_DETAIL_LOADING_ICON_DEFAULT_VISIBLE) != 0);
static char s_user_name_text[96] = "Member";

typedef enum {
  USER_DETAIL_TREND_UNKNOWN = 0,
  USER_DETAIL_TREND_UP,
  USER_DETAIL_TREND_DOWN
} user_detail_trend_t;

typedef struct {
  health_ui_metric_detail_t metric;
} metric_nav_ctx_t;

static void on_back_clicked(lv_event_t *e) {
  (void)e;
  health_ui_root_set_active_page(HEALTH_UI_PAGE_HEALTH, false);
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

static void on_metric_card_deleted(lv_event_t *e) {
  metric_nav_ctx_t *ctx = (metric_nav_ctx_t *)lv_event_get_user_data(e);
  if (NULL != ctx) {
    lv_free(ctx);
  }
}

static void on_metric_card_clicked(lv_event_t *e) {
  metric_nav_ctx_t *ctx = (metric_nav_ctx_t *)lv_event_get_user_data(e);
  if (NULL == ctx) {
    return;
  }

  health_ui_root_open_metric_detail(ctx->metric);
}

static void attach_metric_navigation(lv_obj_t *card,
                                     health_ui_metric_detail_t metric) {
  metric_nav_ctx_t *ctx;

  if (NULL == card) {
    return;
  }

  ctx = (metric_nav_ctx_t *)lv_malloc(sizeof(*ctx));
  if (NULL == ctx) {
    return;
  }

  ctx->metric = metric;
  add_event_bubble_recursive(card);
  lv_obj_add_event_cb(card, on_metric_card_clicked, LV_EVENT_CLICKED, ctx);
  lv_obj_add_event_cb(card, on_metric_card_deleted, LV_EVENT_DELETE, ctx);
}

static lv_color_t get_status_text_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(USER_DETAIL_STATUS_WARNING_TEXT_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(USER_DETAIL_STATUS_CRITICAL_TEXT_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(USER_DETAIL_STATUS_NORMAL_TEXT_HEX);
  }
}

static lv_color_t get_status_chip_bg_color(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(USER_DETAIL_STATUS_WARNING_BG_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(USER_DETAIL_STATUS_CRITICAL_BG_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(USER_DETAIL_STATUS_NORMAL_BG_HEX);
  }
}

static lv_color_t get_card_bg_by_status(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(USER_DETAIL_WARNING_BG_COLOR_HEX);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(USER_DETAIL_CRITICAL_BG_COLOR_HEX);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(USER_DETAIL_CARD_BG_COLOR_HEX);
  }
}

static lv_color_t get_card_border_by_status(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
    return lv_color_hex(0xF2CF66);
  case COMP_USER_STATUS_CRITICAL:
    return lv_color_hex(0xF59AA0);
  case COMP_USER_STATUS_NORMAL:
  default:
    return lv_color_hex(USER_DETAIL_CARD_BG_COLOR_HEX);
  }
}

static lv_coord_t get_card_border_width_by_status(comp_user_status_level_t level) {
  switch (level) {
  case COMP_USER_STATUS_WARNING:
  case COMP_USER_STATUS_CRITICAL:
    return 1;
  case COMP_USER_STATUS_NORMAL:
  default:
    return 0;
  }
}

static void apply_loading_icon_visibility(void) {
  if ((NULL == s_loading_icon) || (!lv_obj_is_valid(s_loading_icon))) {
    s_loading_icon = NULL;
    return;
  }

  if (s_loading_icon_visible) {
    lv_obj_clear_flag(s_loading_icon, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_flag(s_loading_icon, LV_OBJ_FLAG_HIDDEN);
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
  lv_obj_set_flex_align(row, main_align, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
  return row;
}

static lv_obj_t *create_card_shell(lv_obj_t *parent, lv_color_t bg_color,
                                   lv_color_t border_color,
                                   lv_coord_t border_width) {
  lv_obj_t *card = lv_obj_create(parent);
  lv_obj_set_height(card, lv_pct(100));
  lv_obj_set_style_bg_color(card, bg_color, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(card, USER_DETAIL_CARD_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, border_width, LV_PART_MAIN);
  lv_obj_set_style_border_color(card, border_color, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(card, 14, LV_PART_MAIN);
  lv_obj_set_style_pad_right(card, 14, LV_PART_MAIN);
  lv_obj_set_style_pad_top(card, HL_SCALE_V(10), LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(card, HL_SCALE_V(10), LV_PART_MAIN);
  lv_obj_set_style_pad_row(card, HL_SCALE_V(6), LV_PART_MAIN);
  lv_obj_set_layout(card, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  return card;
}

static const char *metric_icon_from_title(const char *title_text) {
  if (NULL == title_text) {
    return "M";
  }

  if (0 == strcmp(title_text, "Body Temp")) {
    return "T";
  }
  if (0 == strcmp(title_text, "Blood Glucose")) {
    return "G";
  }
  if (0 == strcmp(title_text, "Sleep Duration")) {
    return "S";
  }
  if (0 == strcmp(title_text, "Weight")) {
    return "W";
  }
  if ((0 == strcmp(title_text, "SpO2")) || (0 == strcmp(title_text, "SpO₂"))) {
    return "O";
  }
  return "M";
}

static bool parse_first_float(const char *text, float *out_value) {
  char *end_ptr = NULL;
  float parsed = 0.0f;

  if ((NULL == text) || (NULL == out_value)) {
    return false;
  }

  parsed = strtof(text, &end_ptr);
  if (end_ptr == text) {
    return false;
  }

  *out_value = parsed;
  return true;
}

static user_detail_trend_t resolve_metric_trend(
    const health_ui_user_detail_metric_card_t *metric) {
  float current = 0.0f;
  float previous = 0.0f;
  float diff = 0.0f;

  if ((NULL == metric) || (!metric->previous_available)) {
    return USER_DETAIL_TREND_UNKNOWN;
  }

  if (!parse_first_float(metric->value_text, &current) ||
      !parse_first_float(metric->previous_value_text, &previous)) {
    return USER_DETAIL_TREND_UNKNOWN;
  }

  diff = current - previous;
  if (diff > 0.001f) {
    return USER_DETAIL_TREND_UP;
  }
  if (diff < -0.001f) {
    return USER_DETAIL_TREND_DOWN;
  }

  return USER_DETAIL_TREND_UNKNOWN;
}

static lv_obj_t *create_trend_indicator(lv_obj_t *parent,
                                        user_detail_trend_t trend) {
  lv_obj_t *trend_col;
  lv_obj_t *up_label;
  lv_obj_t *down_label;
  lv_color_t active_color = lv_color_hex(USER_DETAIL_TREND_ACTIVE_COLOR_HEX);
  lv_color_t muted_color = lv_color_hex(USER_DETAIL_TREND_MUTED_COLOR_HEX);
  lv_color_t up_color =
      (USER_DETAIL_TREND_UP == trend) ? active_color : muted_color;
  lv_color_t down_color =
      (USER_DETAIL_TREND_DOWN == trend) ? active_color : muted_color;

  trend_col = lv_obj_create(parent);
  lv_obj_set_size(trend_col, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(trend_col, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(trend_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(trend_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(trend_col, 0, LV_PART_MAIN);
  lv_obj_set_layout(trend_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(trend_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(trend_col, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(trend_col, LV_OBJ_FLAG_SCROLLABLE);

  up_label = lv_label_create(trend_col);
  lv_label_set_text(up_label, LV_SYMBOL_UP);
  lv_obj_set_style_text_color(up_label, up_color, LV_PART_MAIN);
  lv_obj_set_style_text_font(up_label, USER_DETAIL_FONT_TREND, LV_PART_MAIN);

  down_label = lv_label_create(trend_col);
  lv_label_set_text(down_label, LV_SYMBOL_DOWN);
  lv_obj_set_style_text_color(down_label, down_color, LV_PART_MAIN);
  lv_obj_set_style_text_font(down_label, USER_DETAIL_FONT_TREND, LV_PART_MAIN);

  return trend_col;
}

static void create_status_chip(lv_obj_t *parent, const char *status_text,
                               comp_user_status_level_t level) {
  lv_obj_t *chip = lv_obj_create(parent);
  lv_obj_t *label;

  lv_obj_set_size(chip, lv_pct(100), HL_SCALE_V(26));
  lv_obj_set_style_bg_color(chip, get_status_chip_bg_color(level), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(chip, 14, LV_PART_MAIN);
  lv_obj_set_style_border_width(chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chip, 0, LV_PART_MAIN);
  lv_obj_clear_flag(chip, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(chip);
  lv_label_set_text(label, (NULL != status_text) ? status_text : "Normal");
  lv_obj_set_style_text_color(label, get_status_text_color(level), LV_PART_MAIN);
  lv_obj_set_style_text_font(label, USER_DETAIL_FONT_STATUS, LV_PART_MAIN);
  lv_obj_center(label);
}

static void create_bp_value_column(lv_obj_t *parent, const char *value_text,
                                   const char *name_text, const char *unit_text,
                                   const char *status_text,
                                   comp_user_status_level_t status_level) {
  lv_obj_t *col;
  lv_obj_t *value_row;
  lv_obj_t *value_group;
  lv_obj_t *value_label;
  lv_obj_t *name_label;
  lv_obj_t *unit_label;
  lv_obj_t *prev1;
  lv_obj_t *prev2;

  col = lv_obj_create(parent);
  lv_obj_set_size(col, 0, LV_SIZE_CONTENT);
  lv_obj_set_flex_grow(col, 1);
  lv_obj_set_style_bg_opa(col, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(col, 6, LV_PART_MAIN);
  lv_obj_set_layout(col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(col, LV_OBJ_FLAG_SCROLLABLE);

  value_row = create_transparent_row(col, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);
  (void)create_trend_indicator(value_row, USER_DETAIL_TREND_UNKNOWN);

  value_group = lv_obj_create(value_row);
  lv_obj_set_size(value_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(value_group, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(value_group, 6, LV_PART_MAIN);
  lv_obj_set_layout(value_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(value_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(value_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(value_group, LV_OBJ_FLAG_SCROLLABLE);

  value_label = lv_label_create(value_group);
  lv_label_set_text(value_label, (NULL != value_text) ? value_text : "--");
  lv_obj_set_style_text_color(value_label, lv_color_hex(0x101828), LV_PART_MAIN);
  lv_obj_set_style_text_font(value_label, USER_DETAIL_FONT_VALUE, LV_PART_MAIN);

  name_label = lv_label_create(value_group);
  lv_label_set_text(name_label, (NULL != name_text) ? name_text : "");
  lv_obj_set_style_text_color(name_label, lv_color_hex(0x101828), LV_PART_MAIN);
  lv_obj_set_style_text_font(name_label, USER_DETAIL_FONT_VALUE_META, LV_PART_MAIN);

  unit_label = lv_label_create(col);
  lv_label_set_text(unit_label, (NULL != unit_text) ? unit_text : "");
  lv_obj_set_width(unit_label, lv_pct(100));
  lv_obj_set_style_text_align(unit_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_set_style_text_color(unit_label, lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(unit_label, USER_DETAIL_FONT_UNIT, LV_PART_MAIN);

  create_status_chip(col, status_text, status_level);

  prev1 = lv_label_create(col);
  lv_label_set_text(prev1, "No Previous");
  lv_obj_set_style_text_color(prev1, lv_color_hex(0x101828), LV_PART_MAIN);
  lv_obj_set_style_text_font(prev1, USER_DETAIL_FONT_PREV_VALUE, LV_PART_MAIN);

  prev2 = lv_label_create(col);
  lv_label_set_text(prev2, "Health Record");
  lv_obj_set_style_text_color(prev2, lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(prev2, USER_DETAIL_FONT_PREV_TIME, LV_PART_MAIN);
}

static void create_bp_summary_card(lv_obj_t *parent,
                                   const health_ui_user_detail_bp_summary_t *bp) {
  lv_obj_t *card;
  lv_obj_t *header_row;
  lv_obj_t *left_group;
  lv_obj_t *icon_bg;
  lv_obj_t *icon_label;
  lv_obj_t *title;
  lv_obj_t *time_label;
  lv_obj_t *value_row;

  card = create_card_shell(parent, get_card_bg_by_status(bp->status_level),
                           get_card_border_by_status(bp->status_level),
                           get_card_border_width_by_status(bp->status_level));
  lv_obj_set_size(card, 0, lv_pct(100));
  lv_obj_set_flex_grow(card, USER_DETAIL_BP_CARD_GROW);

  header_row = create_transparent_row(card, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);

  left_group = create_transparent_row(header_row, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_size(left_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  icon_bg = lv_obj_create(left_group);
  lv_obj_set_size(icon_bg, 32, 32);
  lv_obj_set_style_radius(icon_bg, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(icon_bg, lv_color_hex(USER_DETAIL_ICON_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(icon_bg, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(icon_bg, 0, LV_PART_MAIN);
  lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);

  icon_label = lv_label_create(icon_bg);
  lv_label_set_text(icon_label, "B");
  lv_obj_set_style_text_color(icon_label, lv_color_hex(0x667085), LV_PART_MAIN);
  lv_obj_set_style_text_font(icon_label, USER_DETAIL_FONT_ICON, LV_PART_MAIN);
  lv_obj_center(icon_label);

  title = lv_label_create(left_group);
  lv_label_set_text(title, "Blood Pressure");
  lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
  lv_obj_set_width(title, USER_DETAIL_TITLE_MAX_WIDTH_PX);
  lv_obj_set_style_text_color(title, lv_color_hex(USER_DETAIL_TITLE_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title, USER_DETAIL_FONT_CARD_TITLE, LV_PART_MAIN);

  time_label = lv_label_create(header_row);
  lv_label_set_text(time_label, (NULL != bp->time_text) ? bp->time_text : "--:--");
  lv_label_set_long_mode(time_label, LV_LABEL_LONG_CLIP);
  lv_obj_set_width(time_label, USER_DETAIL_TIME_MIN_WIDTH_PX);
  lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_set_style_text_color(time_label,
                              lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(time_label, USER_DETAIL_FONT_TIME, LV_PART_MAIN);

  value_row = create_transparent_row(card, 10, LV_FLEX_ALIGN_START);
  create_bp_value_column(value_row, bp->sys_value_text, "SYS", bp->sys_unit_text,
                         bp->status_text, bp->status_level);
  create_bp_value_column(value_row, bp->dia_value_text, "DIA", bp->dia_unit_text,
                         bp->status_text, bp->status_level);
  create_bp_value_column(value_row, bp->pulse_value_text, "PULSE",
                         bp->pulse_unit_text, bp->status_text, bp->status_level);

  attach_metric_navigation(card, HEALTH_UI_METRIC_DETAIL_BP);
}

static void create_metric_card(lv_obj_t *parent,
                               const health_ui_user_detail_metric_card_t *metric,
                               lv_coord_t grow_units,
                               health_ui_metric_detail_t metric_id) {
  lv_obj_t *card;
  lv_obj_t *header_row;
  lv_obj_t *left_group;
  lv_obj_t *icon_bg;
  lv_obj_t *icon_label;
  lv_obj_t *title;
  lv_obj_t *time_label;
  lv_obj_t *value_row;
  lv_obj_t *value_group;
  lv_obj_t *value_label;
  lv_obj_t *unit_label;
  lv_obj_t *prev_line_1;
  lv_obj_t *prev_line_2;
  user_detail_trend_t trend;

  card = create_card_shell(parent, get_card_bg_by_status(metric->status_level),
                           get_card_border_by_status(metric->status_level),
                           get_card_border_width_by_status(metric->status_level));
  lv_obj_set_size(card, 0, lv_pct(100));
  lv_obj_set_flex_grow(card, grow_units);

  header_row = create_transparent_row(card, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);

  left_group = create_transparent_row(header_row, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_size(left_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  icon_bg = lv_obj_create(left_group);
  lv_obj_set_size(icon_bg, 32, 32);
  lv_obj_set_style_radius(icon_bg, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(icon_bg, lv_color_hex(USER_DETAIL_ICON_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(icon_bg, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(icon_bg, 0, LV_PART_MAIN);
  lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);

  icon_label = lv_label_create(icon_bg);
  lv_label_set_text(icon_label, metric_icon_from_title(metric->title_text));
  lv_obj_set_style_text_color(icon_label, lv_color_hex(0x667085), LV_PART_MAIN);
  lv_obj_set_style_text_font(icon_label, USER_DETAIL_FONT_ICON, LV_PART_MAIN);
  lv_obj_center(icon_label);

  title = lv_label_create(left_group);
  lv_label_set_text(title, (NULL != metric->title_text) ? metric->title_text : "Metric");
  lv_label_set_long_mode(title, LV_LABEL_LONG_DOT);
  lv_obj_set_width(title, USER_DETAIL_TITLE_MAX_WIDTH_PX);
  lv_obj_set_style_text_color(title, lv_color_hex(USER_DETAIL_TITLE_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title, USER_DETAIL_FONT_CARD_TITLE, LV_PART_MAIN);

  time_label = lv_label_create(header_row);
  lv_label_set_text(time_label,
                    (NULL != metric->time_text) ? metric->time_text : "--:--");
  lv_label_set_long_mode(time_label, LV_LABEL_LONG_CLIP);
  lv_obj_set_width(time_label, USER_DETAIL_TIME_MIN_WIDTH_PX);
  lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
  lv_obj_set_style_text_color(time_label,
                              lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(time_label, USER_DETAIL_FONT_TIME, LV_PART_MAIN);

  trend = resolve_metric_trend(metric);
  value_row = create_transparent_row(card, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);
  (void)create_trend_indicator(value_row, trend);

  value_group = lv_obj_create(value_row);
  lv_obj_set_size(value_group, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(value_group, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(value_group, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(value_group, 8, LV_PART_MAIN);
  lv_obj_set_layout(value_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(value_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(value_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(value_group, LV_OBJ_FLAG_SCROLLABLE);

  value_label = lv_label_create(value_group);
  lv_label_set_text(value_label, (NULL != metric->value_text) ? metric->value_text : "--");
  lv_obj_set_style_text_color(value_label, lv_color_hex(0x101828), LV_PART_MAIN);
  lv_obj_set_style_text_font(value_label, USER_DETAIL_FONT_VALUE, LV_PART_MAIN);

  unit_label = lv_label_create(value_group);
  lv_label_set_text(unit_label, (NULL != metric->unit_text) ? metric->unit_text : "");
  lv_obj_set_style_text_color(unit_label,
                              lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(unit_label, USER_DETAIL_FONT_UNIT, LV_PART_MAIN);

  create_status_chip(card, metric->status_text, metric->status_level);

  if (metric->previous_available) {
    prev_line_1 = lv_label_create(card);
    lv_label_set_text(prev_line_1,
                      (NULL != metric->previous_time_text) ? metric->previous_time_text
                                                           : "--");
    lv_obj_set_style_text_color(prev_line_1,
                                lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                                LV_PART_MAIN);
    lv_obj_set_style_text_font(prev_line_1, USER_DETAIL_FONT_PREV_TIME, LV_PART_MAIN);

    prev_line_2 = lv_label_create(card);
    if ((NULL != metric->previous_value_text) && (NULL != metric->previous_unit_text)) {
      lv_label_set_text_fmt(prev_line_2, "%s %s", metric->previous_value_text,
                            metric->previous_unit_text);
    } else {
      lv_label_set_text(prev_line_2, "--");
    }
    lv_obj_set_style_text_color(prev_line_2, lv_color_hex(0x101828), LV_PART_MAIN);
    lv_obj_set_style_text_font(prev_line_2, USER_DETAIL_FONT_PREV_VALUE, LV_PART_MAIN);
  } else {
    prev_line_1 = lv_label_create(card);
    lv_label_set_text(prev_line_1, "No Previous");
    lv_obj_set_style_text_color(prev_line_1, lv_color_hex(0x101828), LV_PART_MAIN);
    lv_obj_set_style_text_font(prev_line_1, USER_DETAIL_FONT_PREV_VALUE, LV_PART_MAIN);

    prev_line_2 = lv_label_create(card);
    lv_label_set_text(prev_line_2, "Health Record");
    lv_obj_set_style_text_color(prev_line_2,
                                lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                                LV_PART_MAIN);
    lv_obj_set_style_text_font(prev_line_2, USER_DETAIL_FONT_PREV_TIME, LV_PART_MAIN);
  }

  attach_metric_navigation(card, metric_id);
}

static void rebuild_detail_content(void) {
  lv_obj_t *section_header_row;
  lv_obj_t *section_title;
  lv_obj_t *top_row;
  lv_obj_t *bottom_row;
  health_ui_user_detail_data_t detail_data;

  if ((NULL == s_content_root) || (!lv_obj_is_valid(s_content_root))) {
    s_content_root = NULL;
    return;
  }

  lv_obj_clean(s_content_root);
  health_ui_data_build_user_detail(s_selected_member_index, &detail_data);

  section_header_row =
      create_transparent_row(s_content_root, USER_DETAIL_CARD_GAP_PX, LV_FLEX_ALIGN_START);

  section_title = lv_label_create(section_header_row);
  lv_label_set_text(section_title, "Health information");
  lv_obj_set_style_text_color(section_title,
                              lv_color_hex(USER_DETAIL_TITLE_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(section_title, USER_DETAIL_FONT_SECTION_TITLE,
                             LV_PART_MAIN);

  s_loading_icon = lv_label_create(section_header_row);
  lv_label_set_text(s_loading_icon, LV_SYMBOL_REFRESH);
  lv_obj_set_style_text_color(s_loading_icon,
                              lv_color_hex(USER_DETAIL_MUTED_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(s_loading_icon, USER_DETAIL_FONT_SECTION_TITLE,
                             LV_PART_MAIN);
  apply_loading_icon_visibility();

  top_row = lv_obj_create(s_content_root);
  lv_obj_set_size(top_row, lv_pct(100), USER_DETAIL_TOP_ROW_H_PX);
  lv_obj_set_style_bg_opa(top_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(top_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(top_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(top_row, USER_DETAIL_CARD_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(top_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(top_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(top_row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(top_row, LV_OBJ_FLAG_SCROLLABLE);

  create_bp_summary_card(top_row, &detail_data.bp);
  create_metric_card(top_row, &detail_data.glucose, USER_DETAIL_GLUCOSE_CARD_GROW,
                     HEALTH_UI_METRIC_DETAIL_GLUCOSE);

  bottom_row = lv_obj_create(s_content_root);
  lv_obj_set_size(bottom_row, lv_pct(100), USER_DETAIL_BOTTOM_ROW_H_PX);
  lv_obj_set_style_bg_opa(bottom_row, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(bottom_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(bottom_row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(bottom_row, USER_DETAIL_CARD_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(bottom_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(bottom_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(bottom_row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(bottom_row, LV_OBJ_FLAG_SCROLLABLE);

  create_metric_card(bottom_row, &detail_data.body_temp, 1,
                     HEALTH_UI_METRIC_DETAIL_BODY_TEMP);
  create_metric_card(bottom_row, &detail_data.spo2, 1,
                     HEALTH_UI_METRIC_DETAIL_SPO2);
  create_metric_card(bottom_row, &detail_data.weight, 1,
                     HEALTH_UI_METRIC_DETAIL_WEIGHT);
  create_metric_card(bottom_row, &detail_data.sleep_duration, 1,
                     HEALTH_UI_METRIC_DETAIL_SLEEP);
}

void page_user_detail_layout_set_user_name(const char *user_name) {
  const char *name = "Member";

  if ((NULL != user_name) && (user_name[0] != '\0')) {
    name = user_name;
  }

  (void)snprintf(s_user_name_text, sizeof(s_user_name_text), "%s", name);
}

void page_user_detail_layout_set_member_index(uint32_t member_index) {
  s_selected_member_index = member_index;
  rebuild_detail_content();
}

void page_user_detail_layout_set_health_loading_icon(bool visible) {
  s_loading_icon_visible = visible;
  apply_loading_icon_visibility();
}

void page_user_detail_layout_build(lv_obj_t *tab) {
  lv_obj_t *page;
  comp_tab_menu_refs_t menu_refs;

  if (NULL == tab) {
    return;
  }

  page = health_layout_scaffold_create_page(tab);
  if (NULL == page) {
    return;
  }

  (void)comp_tab_menu_create(page, COMP_TAB_MENU_MODE_USER_DETAIL, &menu_refs);
  if (NULL != menu_refs.btn_back) {
    lv_obj_add_event_cb(menu_refs.btn_back, on_back_clicked, LV_EVENT_CLICKED,
                        NULL);
  }

  s_content_root = lv_obj_create(page);
  lv_obj_set_size(s_content_root, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(s_content_root, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(s_content_root, USER_DETAIL_SIDE_INSET_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(s_content_root, USER_DETAIL_SIDE_INSET_PX,
                             LV_PART_MAIN);
  lv_obj_set_style_pad_top(s_content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(s_content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(s_content_root, USER_DETAIL_SECTION_ROW_GAP_PX,
                           LV_PART_MAIN);
  lv_obj_set_layout(s_content_root, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(s_content_root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(s_content_root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(s_content_root, LV_OBJ_FLAG_SCROLLABLE);

  rebuild_detail_content();
}
