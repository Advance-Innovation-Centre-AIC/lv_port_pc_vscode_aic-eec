#include "page_metric_bp_detail_layout.h"

#include "../../components/content/comp_bp_metric_summary_card.h"

/* Accent colors mapped to SYS / DIA / PULSE lines and card dots. */
#define METRIC_BP_DETAIL_SYS_COLOR_HEX (0x4A89E8)
#define METRIC_BP_DETAIL_DIA_COLOR_HEX (0xC494F1)
#define METRIC_BP_DETAIL_PULSE_COLOR_HEX (0x6EBD8B)

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
                                    const char *const *labels, uint32_t label_count,
                                    lv_text_align_t text_align) {
  uint32_t i;
  lv_obj_t *col = create_transparent_box(parent, width, lv_pct(100), 0);
  lv_obj_set_layout(col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(col, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  for (i = 0U; i < label_count; i++) {
    lv_obj_t *label = lv_label_create(col);
    lv_label_set_text(label, labels[i]);
    lv_obj_set_width(label, lv_pct(100));
    lv_obj_set_style_text_font(label, METRIC_BP_DETAIL_AXIS_LABEL_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(METRIC_BP_DETAIL_AXIS_TEXT_COLOR_HEX),
                                LV_PART_MAIN);
    lv_obj_set_style_text_align(label, text_align, LV_PART_MAIN);
  }

  return col;
}

static void populate_bp_chart(lv_obj_t *chart,
                              const health_ui_bp_metric_detail_data_t *data,
                              uint32_t point_count) {
  lv_chart_series_t *sys_series;
  lv_chart_series_t *dia_series;
  lv_chart_series_t *pulse_series;
  int32_t *sys_y;
  int32_t *dia_y;
  int32_t *pulse_y;
  uint32_t i;

  if ((NULL == chart) || (NULL == data) || (point_count == 0U)) {
    return;
  }

  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count(chart, point_count);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 60, 160);
  lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, 60, 100);
  lv_chart_set_div_line_count(chart, 5, (point_count > 1U) ? (point_count - 1U) : 1U);
  lv_obj_set_style_line_width(chart, METRIC_BP_DETAIL_CHART_LINE_WIDTH_PX, LV_PART_ITEMS);
  lv_obj_set_style_size(chart, METRIC_BP_DETAIL_CHART_POINT_SIZE_PX,
                        METRIC_BP_DETAIL_CHART_POINT_SIZE_PX, LV_PART_INDICATOR);
  lv_obj_set_style_line_color(chart,
                              lv_color_hex(METRIC_BP_DETAIL_CHART_DIV_LINE_COLOR_HEX),
                              LV_PART_MAIN);

  sys_series =
      lv_chart_add_series(chart, lv_color_hex(METRIC_BP_DETAIL_SYS_COLOR_HEX),
                          LV_CHART_AXIS_PRIMARY_Y);
  dia_series =
      lv_chart_add_series(chart, lv_color_hex(METRIC_BP_DETAIL_DIA_COLOR_HEX),
                          LV_CHART_AXIS_PRIMARY_Y);
  pulse_series =
      lv_chart_add_series(chart, lv_color_hex(METRIC_BP_DETAIL_PULSE_COLOR_HEX),
                          LV_CHART_AXIS_SECONDARY_Y);

  sys_y = lv_chart_get_y_array(chart, sys_series);
  dia_y = lv_chart_get_y_array(chart, dia_series);
  pulse_y = lv_chart_get_y_array(chart, pulse_series);
  if ((NULL == sys_y) || (NULL == dia_y) || (NULL == pulse_y)) {
    return;
  }

  for (i = 0U; i < point_count; i++) {
    sys_y[i] = data->sys_values[i];
    dia_y[i] = data->dia_values[i];
    pulse_y[i] = data->pulse_values[i];
  }

  lv_chart_refresh(chart);
}

void page_metric_bp_detail_layout_build(
    lv_obj_t *parent, const health_ui_bp_metric_detail_data_t *data) {
  static const char *k_left_axis_labels[] = {"160", "135", "110", "85", "60"};
  static const char *k_right_axis_labels[] = {"100", "90", "80", "70", "60"};
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
  lv_obj_t *chart_panel;
  lv_obj_t *chart_body_row;
  lv_obj_t *chart;
  lv_obj_t *x_row;
  lv_obj_t *x_center_row;
  uint32_t point_count = HEALTH_UI_BP_DETAIL_MAX_POINTS;
  uint32_t i;
  const char *period_text = "July 1 - July 7, 2025";
  const char *status_text = "Normal";

  if ((NULL == parent) || (NULL == data)) {
    return;
  }

  if ((data->point_count > 0U) && (data->point_count <= HEALTH_UI_BP_DETAIL_MAX_POINTS)) {
    point_count = data->point_count;
  }
  if ((NULL != data->period_text) && (data->period_text[0] != '\0')) {
    period_text = data->period_text;
  }
  if ((NULL != data->summary.status_text) && (data->summary.status_text[0] != '\0')) {
    status_text = data->summary.status_text;
  }

  panel = lv_obj_create(parent);
  lv_obj_set_size(panel, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(panel, lv_color_hex(METRIC_BP_DETAIL_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(panel, METRIC_BP_DETAIL_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(panel, METRIC_BP_DETAIL_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(panel, METRIC_BP_DETAIL_PANEL_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  header_row = create_transparent_row(panel, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);

  title_label = lv_label_create(header_row);
  lv_label_set_text(title_label, "Blood Pressure");
  lv_obj_set_style_text_font(title_label, METRIC_BP_DETAIL_TITLE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_label, lv_color_hex(METRIC_BP_DETAIL_TITLE_COLOR_HEX),
                              LV_PART_MAIN);

  range_row = create_transparent_row(header_row, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_width(range_row, LV_SIZE_CONTENT);

  prev_btn = lv_button_create(range_row);
  lv_obj_set_size(prev_btn, METRIC_BP_DETAIL_PERIOD_BTN_SIZE_PX,
                  METRIC_BP_DETAIL_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(prev_btn, lv_color_hex(METRIC_BP_DETAIL_PERIOD_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(prev_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(prev_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(prev_btn, 0, LV_PART_MAIN);
  prev_label = lv_label_create(prev_btn);
  lv_label_set_text(prev_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_font(prev_label, METRIC_BP_DETAIL_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(prev_label,
                              lv_color_hex(METRIC_BP_DETAIL_PERIOD_BTN_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(prev_label);

  period_chip = lv_obj_create(range_row);
  lv_obj_set_size(period_chip, METRIC_BP_DETAIL_PERIOD_CHIP_MIN_W_PX,
                  METRIC_BP_DETAIL_PERIOD_CHIP_HEIGHT_PX);
  lv_obj_set_style_bg_color(period_chip, lv_color_hex(METRIC_BP_DETAIL_PERIOD_CHIP_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(period_chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(period_chip, METRIC_BP_DETAIL_PERIOD_CHIP_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(period_chip, 0, LV_PART_MAIN);
  lv_obj_clear_flag(period_chip, LV_OBJ_FLAG_SCROLLABLE);
  period_label = lv_label_create(period_chip);
  lv_label_set_text(period_label, period_text);
  lv_obj_set_style_text_font(period_label, METRIC_BP_DETAIL_PERIOD_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(period_label,
                              lv_color_hex(METRIC_BP_DETAIL_PERIOD_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(period_label);

  next_btn = lv_button_create(range_row);
  lv_obj_set_size(next_btn, METRIC_BP_DETAIL_PERIOD_BTN_SIZE_PX,
                  METRIC_BP_DETAIL_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(next_btn, lv_color_hex(METRIC_BP_DETAIL_PERIOD_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(next_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(next_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(next_btn, 0, LV_PART_MAIN);
  next_label = lv_label_create(next_btn);
  lv_label_set_text(next_label, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_font(next_label, METRIC_BP_DETAIL_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(next_label,
                              lv_color_hex(METRIC_BP_DETAIL_PERIOD_BTN_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_center(next_label);

  summary_row = create_transparent_row(panel, METRIC_BP_DETAIL_SUMMARY_COL_GAP_PX,
                                       LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_height(summary_row, METRIC_BP_DETAIL_SUMMARY_ROW_H_PX);

  summary_data.label_text = "SYS";
  summary_data.unit_text = (NULL != data->summary.sys_unit_text) ? data->summary.sys_unit_text : "mmHg";
  summary_data.value_text = (NULL != data->summary.sys_value_text) ? data->summary.sys_value_text : "--";
  summary_data.status_text = status_text;
  summary_data.status_level = data->summary.status_level;
  summary_data.accent_color_hex = METRIC_BP_DETAIL_SYS_COLOR_HEX;
  (void)comp_bp_metric_summary_card_create(summary_row, &summary_data);

  summary_data.label_text = "DIA";
  summary_data.unit_text = (NULL != data->summary.dia_unit_text) ? data->summary.dia_unit_text : "mmHg";
  summary_data.value_text = (NULL != data->summary.dia_value_text) ? data->summary.dia_value_text : "--";
  summary_data.status_text = status_text;
  summary_data.status_level = data->summary.status_level;
  summary_data.accent_color_hex = METRIC_BP_DETAIL_DIA_COLOR_HEX;
  (void)comp_bp_metric_summary_card_create(summary_row, &summary_data);

  summary_data.label_text = "PULSE";
  summary_data.unit_text = (NULL != data->summary.pulse_unit_text) ? data->summary.pulse_unit_text : "bpm";
  summary_data.value_text = (NULL != data->summary.pulse_value_text) ? data->summary.pulse_value_text : "--";
  summary_data.status_text = status_text;
  summary_data.status_level = data->summary.status_level;
  summary_data.accent_color_hex = METRIC_BP_DETAIL_PULSE_COLOR_HEX;
  (void)comp_bp_metric_summary_card_create(summary_row, &summary_data);

  chart_panel = lv_obj_create(panel);
  lv_obj_set_size(chart_panel, lv_pct(100), METRIC_BP_DETAIL_CHART_PANEL_H_PX);
  lv_obj_set_style_bg_color(chart_panel, lv_color_hex(METRIC_BP_DETAIL_CHART_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chart_panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(chart_panel, METRIC_BP_DETAIL_CHART_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart_panel, METRIC_BP_DETAIL_CHART_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(chart_panel, 6, LV_PART_MAIN);
  lv_obj_set_layout(chart_panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(chart_panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(chart_panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(chart_panel, LV_OBJ_FLAG_SCROLLABLE);

  chart_body_row = create_transparent_row(chart_panel, 6, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_size(chart_body_row, lv_pct(100), 0);
  lv_obj_set_flex_grow(chart_body_row, 1);

  (void)create_axis_column(chart_body_row, METRIC_BP_DETAIL_CHART_LEFT_AXIS_W_PX,
                           k_left_axis_labels,
                           (uint32_t)(sizeof(k_left_axis_labels) / sizeof(k_left_axis_labels[0])),
                           LV_TEXT_ALIGN_LEFT);

  chart = lv_chart_create(chart_body_row);
  lv_obj_set_size(chart, 0, lv_pct(100));
  lv_obj_set_flex_grow(chart, 1);
  lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart, 0, LV_PART_MAIN);
  lv_obj_clear_flag(chart, LV_OBJ_FLAG_SCROLLABLE);
  populate_bp_chart(chart, data, point_count);

  (void)create_axis_column(
      chart_body_row, METRIC_BP_DETAIL_CHART_RIGHT_AXIS_W_PX, k_right_axis_labels,
      (uint32_t)(sizeof(k_right_axis_labels) / sizeof(k_right_axis_labels[0])),
      LV_TEXT_ALIGN_RIGHT);

  x_row = create_transparent_row(chart_panel, 0, LV_FLEX_ALIGN_START);
  lv_obj_set_width(x_row, lv_pct(100));
  (void)create_transparent_box(x_row, METRIC_BP_DETAIL_CHART_LEFT_AXIS_W_PX, 0, 0);
  x_center_row = create_transparent_row(x_row, 0, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_size(x_center_row, 0, LV_SIZE_CONTENT);
  lv_obj_set_flex_grow(x_center_row, 1);
  for (i = 0U; i < point_count; i++) {
    lv_obj_t *x_label = lv_label_create(x_center_row);
    lv_label_set_text(x_label,
                      (NULL != data->x_labels[i]) ? data->x_labels[i] : "--");
    lv_obj_set_style_text_font(x_label, METRIC_BP_DETAIL_X_LABEL_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(x_label,
                                lv_color_hex(METRIC_BP_DETAIL_X_LABEL_COLOR_HEX),
                                LV_PART_MAIN);
  }
  (void)create_transparent_box(x_row, METRIC_BP_DETAIL_CHART_RIGHT_AXIS_W_PX, 0, 0);
}
