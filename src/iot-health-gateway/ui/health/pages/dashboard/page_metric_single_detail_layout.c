#include "page_metric_single_detail_layout.h"

#include "../../components/content/comp_bp_metric_summary_card.h"
#include "../../health_ui_display_policy.h"
#include "../../layout/layout_fonts.h"

#ifndef METRIC_SINGLE_PANEL_BG_COLOR_HEX
#define METRIC_SINGLE_PANEL_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef METRIC_SINGLE_PANEL_RADIUS_PX
#define METRIC_SINGLE_PANEL_RADIUS_PX (20)
#endif

#ifndef METRIC_SINGLE_PANEL_PAD_PX
#define METRIC_SINGLE_PANEL_PAD_PX (16)
#endif

#ifndef METRIC_SINGLE_PANEL_ROW_GAP_PX
#define METRIC_SINGLE_PANEL_ROW_GAP_PX (10)
#endif

#ifndef METRIC_SINGLE_TITLE_FONT
#define METRIC_SINGLE_TITLE_FONT HL_FONT_TITLE
#endif

#ifndef METRIC_SINGLE_TITLE_COLOR_HEX
#define METRIC_SINGLE_TITLE_COLOR_HEX (0x002329)
#endif

#ifndef METRIC_SINGLE_PERIOD_FONT
#define METRIC_SINGLE_PERIOD_FONT HL_FONT_HEADING
#endif

#ifndef METRIC_SINGLE_PERIOD_TEXT_COLOR_HEX
#define METRIC_SINGLE_PERIOD_TEXT_COLOR_HEX (0x1D2939)
#endif

#ifndef METRIC_SINGLE_PERIOD_BTN_SIZE_PX
#define METRIC_SINGLE_PERIOD_BTN_SIZE_PX HL_SCALE_V(38)
#endif

#ifndef METRIC_SINGLE_PERIOD_BTN_BG_COLOR_HEX
#define METRIC_SINGLE_PERIOD_BTN_BG_COLOR_HEX (0xE7E8EB)
#endif

#ifndef METRIC_SINGLE_PERIOD_BTN_TEXT_FONT
#define METRIC_SINGLE_PERIOD_BTN_TEXT_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SINGLE_PERIOD_BTN_TEXT_COLOR_HEX
#define METRIC_SINGLE_PERIOD_BTN_TEXT_COLOR_HEX (0x101828)
#endif

#ifndef METRIC_SINGLE_PERIOD_CHIP_MIN_W_PX
#define METRIC_SINGLE_PERIOD_CHIP_MIN_W_PX (232)
#endif

#ifndef METRIC_SINGLE_PERIOD_CHIP_HEIGHT_PX
#define METRIC_SINGLE_PERIOD_CHIP_HEIGHT_PX HL_SCALE_V(38)
#endif

#ifndef METRIC_SINGLE_PERIOD_CHIP_RADIUS_PX
#define METRIC_SINGLE_PERIOD_CHIP_RADIUS_PX (22)
#endif

#ifndef METRIC_SINGLE_PERIOD_CHIP_BG_COLOR_HEX
#define METRIC_SINGLE_PERIOD_CHIP_BG_COLOR_HEX (0xE7E8EB)
#endif

#ifndef METRIC_SINGLE_SUMMARY_ROW_H_PX
#define METRIC_SINGLE_SUMMARY_ROW_H_PX HL_SCALE_V(114)
#endif

#ifndef METRIC_SINGLE_SUMMARY_CARD_W_PX
#define METRIC_SINGLE_SUMMARY_CARD_W_PX (400)
#endif

#ifndef METRIC_SINGLE_CHART_PANEL_H_PX
#define METRIC_SINGLE_CHART_PANEL_H_PX HL_SCALE_V(236)
#endif

#ifndef METRIC_SINGLE_CHART_PANEL_BG_COLOR_HEX
#define METRIC_SINGLE_CHART_PANEL_BG_COLOR_HEX (0xF7F8FA)
#endif

#ifndef METRIC_SINGLE_CHART_PANEL_RADIUS_PX
#define METRIC_SINGLE_CHART_PANEL_RADIUS_PX (14)
#endif

#ifndef METRIC_SINGLE_CHART_PANEL_PAD_PX
#define METRIC_SINGLE_CHART_PANEL_PAD_PX (12)
#endif

#ifndef METRIC_SINGLE_CHART_LEFT_AXIS_W_PX
#define METRIC_SINGLE_CHART_LEFT_AXIS_W_PX (72)
#endif

#ifndef METRIC_SINGLE_AXIS_LABEL_FONT
#define METRIC_SINGLE_AXIS_LABEL_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SINGLE_AXIS_TEXT_COLOR_HEX
#define METRIC_SINGLE_AXIS_TEXT_COLOR_HEX (0x667085)
#endif

#ifndef METRIC_SINGLE_CHART_LINE_WIDTH_PX
#define METRIC_SINGLE_CHART_LINE_WIDTH_PX (2)
#endif

#ifndef METRIC_SINGLE_CHART_POINT_SIZE_PX
#define METRIC_SINGLE_CHART_POINT_SIZE_PX (8)
#endif

#ifndef METRIC_SINGLE_CHART_DIV_LINE_COLOR_HEX
#define METRIC_SINGLE_CHART_DIV_LINE_COLOR_HEX (0xD9DEE5)
#endif

#ifndef METRIC_SINGLE_CHART_BG_COLOR_HEX
#define METRIC_SINGLE_CHART_BG_COLOR_HEX (0xFFFFFF)
#endif

#ifndef METRIC_SINGLE_NORMAL_BAND_COLOR_HEX
#define METRIC_SINGLE_NORMAL_BAND_COLOR_HEX (0xEAF0F8)
#endif

#ifndef METRIC_SINGLE_NORMAL_BAND_OPA
#define METRIC_SINGLE_NORMAL_BAND_OPA (LV_OPA_80)
#endif

#ifndef METRIC_SINGLE_X_LABEL_FONT
#define METRIC_SINGLE_X_LABEL_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SINGLE_X_LABEL_COLOR_HEX
#define METRIC_SINGLE_X_LABEL_COLOR_HEX (0x667085)
#endif

static lv_obj_t *create_transparent_box(lv_obj_t *parent, lv_coord_t width,
                                        lv_coord_t height, lv_coord_t flex_grow) {
  lv_obj_t *obj = lv_obj_create(parent);
  lv_obj_set_size(obj, width, height);
  if (flex_grow > 0) {
    lv_obj_set_flex_grow(obj, flex_grow);
  }
  lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  return obj;
}

static lv_obj_t *create_transparent_row(lv_obj_t *parent, lv_coord_t col_gap,
                                        lv_flex_align_t main_align) {
  lv_obj_t *row = create_transparent_box(parent, lv_pct(100), LV_SIZE_CONTENT, 0);
  lv_obj_set_style_pad_column(row, col_gap, LV_PART_MAIN);
  lv_obj_set_layout(row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, main_align, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  return row;
}

static lv_obj_t *create_axis_column(lv_obj_t *parent, lv_coord_t width,
                                    const char *const *labels, uint32_t label_count) {
  uint32_t i;
  lv_obj_t *col = create_transparent_box(parent, width, lv_pct(100), 0);
  lv_obj_set_layout(col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(col, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  for (i = 0U; i < label_count; i++) {
    lv_obj_t *label = lv_label_create(col);
    lv_label_set_text(label, (NULL != labels[i]) ? labels[i] : "");
    lv_obj_set_width(label, lv_pct(100));
    lv_obj_set_style_text_font(label, METRIC_SINGLE_AXIS_LABEL_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(METRIC_SINGLE_AXIS_TEXT_COLOR_HEX),
                                LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
  }

  return col;
}

static void populate_single_chart(lv_obj_t *chart,
                                  const health_ui_single_metric_detail_data_t *data,
                                  uint32_t point_count) {
  lv_chart_series_t *series;
  int32_t *y_values;
  uint32_t i;

  if ((NULL == chart) || (NULL == data) || (0U == point_count)) {
    return;
  }

  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count(chart, point_count);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, data->y_min, data->y_max);
  lv_chart_set_div_line_count(chart, (data->y_label_count > 1U) ? (data->y_label_count - 1U) : 1U,
                              (point_count > 1U) ? (point_count - 1U) : 1U);
  lv_obj_set_style_line_width(chart, METRIC_SINGLE_CHART_LINE_WIDTH_PX, LV_PART_ITEMS);
  lv_obj_set_style_size(chart, METRIC_SINGLE_CHART_POINT_SIZE_PX,
                        METRIC_SINGLE_CHART_POINT_SIZE_PX, LV_PART_INDICATOR);
  lv_obj_set_style_line_color(chart,
                              lv_color_hex(METRIC_SINGLE_CHART_DIV_LINE_COLOR_HEX),
                              LV_PART_MAIN);

  series = lv_chart_add_series(chart, lv_color_hex(data->accent_color_hex),
                               LV_CHART_AXIS_PRIMARY_Y);
  y_values = lv_chart_get_y_array(chart, series);
  if (NULL == y_values) {
    return;
  }

  for (i = 0U; i < point_count; i++) {
    y_values[i] = data->values[i];
  }

  lv_chart_refresh(chart);
}

static void create_normal_range_band(
    lv_obj_t *chart_stack, const health_ui_single_metric_detail_data_t *data) {
  lv_obj_t *band;
  int32_t chart_min;
  int32_t chart_max;
  int32_t normal_min;
  int32_t normal_max;
  int32_t visible_min;
  int32_t visible_max;
  int32_t range;
  int32_t top_px;
  int32_t bottom_px;
  lv_coord_t band_y;
  lv_coord_t band_h;
  lv_coord_t chart_h;
  uint32_t band_color_hex;

  if ((NULL == chart_stack) || (NULL == data) || (0U == data->normal_band_enabled)) {
    return;
  }

  chart_h = lv_obj_get_height(chart_stack);
  if (chart_h <= 0) {
    return;
  }

  chart_min = data->y_min;
  chart_max = data->y_max;
  if (chart_max <= chart_min) {
    return;
  }

  normal_min = data->normal_band_min;
  normal_max = data->normal_band_max;
  if (normal_max < normal_min) {
    int32_t tmp = normal_max;
    normal_max = normal_min;
    normal_min = tmp;
  }

  visible_min = (normal_min > chart_min) ? normal_min : chart_min;
  visible_max = (normal_max < chart_max) ? normal_max : chart_max;
  if (visible_max <= visible_min) {
    return;
  }

  range = chart_max - chart_min;
  top_px = (int32_t)(((int64_t)(chart_max - visible_max) * chart_h) / range);
  bottom_px = (int32_t)(((int64_t)(chart_max - visible_min) * chart_h) / range);
  if (bottom_px <= top_px) {
    return;
  }

  band_y = (lv_coord_t)top_px;
  band_h = (lv_coord_t)(bottom_px - top_px);
  band_color_hex = (0U != data->normal_band_color_hex) ? data->normal_band_color_hex
                                                        : METRIC_SINGLE_NORMAL_BAND_COLOR_HEX;

  band = lv_obj_create(chart_stack);
  lv_obj_set_pos(band, 0, band_y);
  lv_obj_set_size(band, lv_pct(100), band_h);
  lv_obj_set_style_bg_color(band, lv_color_hex(band_color_hex), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(band, METRIC_SINGLE_NORMAL_BAND_OPA, LV_PART_MAIN);
  lv_obj_set_style_radius(band, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(band, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(band, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(band, 0, LV_PART_MAIN);
  lv_obj_clear_flag(band, LV_OBJ_FLAG_SCROLLABLE);
}

void page_metric_single_detail_layout_build(
    lv_obj_t *parent, const health_ui_single_metric_detail_data_t *data) {
  comp_bp_metric_summary_card_data_t summary_data;
  lv_obj_t *panel;
  lv_obj_t *header_row;
  lv_obj_t *title_label;
  lv_obj_t *range_row;
  lv_obj_t *prev_btn;
  lv_obj_t *prev_label;
  lv_obj_t *period_chip;
  lv_obj_t *period_label;
  lv_obj_t *next_btn;
  lv_obj_t *next_label;
  lv_obj_t *summary_row;
  lv_obj_t *summary_card;
  lv_obj_t *chart_panel;
  lv_obj_t *chart_body_row;
  lv_obj_t *chart_stack;
  lv_obj_t *chart;
  lv_obj_t *x_row;
  lv_obj_t *x_center_row;
  uint32_t point_count = 0U;
  uint32_t i;

  if ((NULL == parent) || (NULL == data)) {
    return;
  }

  point_count = data->point_count;
  if ((0U == point_count) || (point_count > HEALTH_UI_SINGLE_METRIC_MAX_POINTS)) {
    point_count = HEALTH_UI_SINGLE_METRIC_MAX_POINTS;
  }

  panel = lv_obj_create(parent);
  lv_obj_set_size(panel, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(panel, lv_color_hex(METRIC_SINGLE_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(panel, METRIC_SINGLE_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(panel, METRIC_SINGLE_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(panel, METRIC_SINGLE_PANEL_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  header_row = create_transparent_row(panel, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);

  title_label = lv_label_create(header_row);
  lv_label_set_text(title_label, (NULL != data->title_text) ? data->title_text : "Metric");
  lv_obj_set_style_text_font(title_label, METRIC_SINGLE_TITLE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_label, lv_color_hex(METRIC_SINGLE_TITLE_COLOR_HEX),
                              LV_PART_MAIN);

  range_row = create_transparent_row(header_row, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_width(range_row, LV_SIZE_CONTENT);

  prev_btn = lv_button_create(range_row);
  lv_obj_set_size(prev_btn, METRIC_SINGLE_PERIOD_BTN_SIZE_PX, METRIC_SINGLE_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(prev_btn, lv_color_hex(METRIC_SINGLE_PERIOD_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(prev_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(prev_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(prev_btn, 0, LV_PART_MAIN);
  prev_label = lv_label_create(prev_btn);
  lv_label_set_text(prev_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_font(prev_label, METRIC_SINGLE_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(prev_label,
                              lv_color_hex(METRIC_SINGLE_PERIOD_BTN_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(prev_label);

  period_chip = lv_obj_create(range_row);
  lv_obj_set_size(period_chip, METRIC_SINGLE_PERIOD_CHIP_MIN_W_PX,
                  METRIC_SINGLE_PERIOD_CHIP_HEIGHT_PX);
  lv_obj_set_style_bg_color(period_chip, lv_color_hex(METRIC_SINGLE_PERIOD_CHIP_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(period_chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(period_chip, METRIC_SINGLE_PERIOD_CHIP_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(period_chip, 0, LV_PART_MAIN);
  lv_obj_clear_flag(period_chip, LV_OBJ_FLAG_SCROLLABLE);
  period_label = lv_label_create(period_chip);
  lv_label_set_text(period_label, (NULL != data->period_text) ? data->period_text : "--");
  lv_obj_set_style_text_font(period_label, METRIC_SINGLE_PERIOD_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(period_label,
                              lv_color_hex(METRIC_SINGLE_PERIOD_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(period_label);

  next_btn = lv_button_create(range_row);
  lv_obj_set_size(next_btn, METRIC_SINGLE_PERIOD_BTN_SIZE_PX, METRIC_SINGLE_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(next_btn, lv_color_hex(METRIC_SINGLE_PERIOD_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(next_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(next_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(next_btn, 0, LV_PART_MAIN);
  next_label = lv_label_create(next_btn);
  lv_label_set_text(next_label, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_font(next_label, METRIC_SINGLE_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(next_label,
                              lv_color_hex(METRIC_SINGLE_PERIOD_BTN_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(next_label);

  summary_row = create_transparent_row(panel, 0, LV_FLEX_ALIGN_START);
  lv_obj_set_height(summary_row, METRIC_SINGLE_SUMMARY_ROW_H_PX);

  summary_data.label_text = data->summary_label_text;
  summary_data.unit_text = data->summary_unit_text;
  summary_data.value_text = data->summary_value_text;
  summary_data.status_text = data->summary_status_text;
  summary_data.status_level = data->summary_status_level;
  summary_data.accent_color_hex = data->accent_color_hex;
  summary_card = comp_bp_metric_summary_card_create(summary_row, &summary_data);
  if (NULL != summary_card) {
    lv_obj_set_width(summary_card, METRIC_SINGLE_SUMMARY_CARD_W_PX);
    lv_obj_set_flex_grow(summary_card, 0);
  }

  chart_panel = lv_obj_create(panel);
  lv_obj_set_size(chart_panel, lv_pct(100), METRIC_SINGLE_CHART_PANEL_H_PX);
  lv_obj_set_style_bg_color(chart_panel, lv_color_hex(METRIC_SINGLE_CHART_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chart_panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(chart_panel, METRIC_SINGLE_CHART_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart_panel, METRIC_SINGLE_CHART_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(chart_panel, 6, LV_PART_MAIN);
  lv_obj_set_layout(chart_panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(chart_panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(chart_panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(chart_panel, LV_OBJ_FLAG_SCROLLABLE);

  chart_body_row = create_transparent_row(chart_panel, 6, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_size(chart_body_row, lv_pct(100), 0);
  lv_obj_set_flex_grow(chart_body_row, 1);

  (void)create_axis_column(chart_body_row, METRIC_SINGLE_CHART_LEFT_AXIS_W_PX,
                           data->y_labels, data->y_label_count);

  chart_stack = lv_obj_create(chart_body_row);
  lv_obj_set_size(chart_stack, 0, lv_pct(100));
  lv_obj_set_flex_grow(chart_stack, 1);
  lv_obj_set_style_bg_color(chart_stack, lv_color_hex(METRIC_SINGLE_CHART_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chart_stack, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(chart_stack, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart_stack, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart_stack, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart_stack, 0, LV_PART_MAIN);
  lv_obj_set_layout(chart_stack, LV_LAYOUT_NONE);
  lv_obj_clear_flag(chart_stack, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_update_layout(chart_body_row);
  create_normal_range_band(chart_stack, data);

  chart = lv_chart_create(chart_stack);
  lv_obj_set_size(chart, lv_pct(100), lv_pct(100));
  lv_obj_set_align(chart, LV_ALIGN_CENTER);
  lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart, 0, LV_PART_MAIN);
  lv_obj_clear_flag(chart, LV_OBJ_FLAG_SCROLLABLE);
  populate_single_chart(chart, data, point_count);

  x_row = create_transparent_row(chart_panel, 0, LV_FLEX_ALIGN_START);
  lv_obj_set_width(x_row, lv_pct(100));
  (void)create_transparent_box(x_row, METRIC_SINGLE_CHART_LEFT_AXIS_W_PX, 0, 0);
  x_center_row = create_transparent_row(x_row, 0, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_size(x_center_row, 0, LV_SIZE_CONTENT);
  lv_obj_set_flex_grow(x_center_row, 1);
  for (i = 0U; i < point_count; i++) {
    lv_obj_t *x_label = lv_label_create(x_center_row);
    lv_label_set_text(x_label, (NULL != data->x_labels[i]) ? data->x_labels[i] : "--");
    lv_obj_set_style_text_font(x_label, METRIC_SINGLE_X_LABEL_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(x_label, lv_color_hex(METRIC_SINGLE_X_LABEL_COLOR_HEX),
                                LV_PART_MAIN);
  }
}
