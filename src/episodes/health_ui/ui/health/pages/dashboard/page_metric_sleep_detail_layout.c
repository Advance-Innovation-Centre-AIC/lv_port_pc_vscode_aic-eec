#include "page_metric_sleep_detail_layout.h"

#include <stdio.h>
#include "../../health_ui_display_policy.h"
#include "../../layout/layout_fonts.h"

#ifndef METRIC_SLEEP_PANEL_BG_COLOR_HEX
#define METRIC_SLEEP_PANEL_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef METRIC_SLEEP_PANEL_RADIUS_PX
#define METRIC_SLEEP_PANEL_RADIUS_PX (20)
#endif

#ifndef METRIC_SLEEP_PANEL_PAD_PX
#define METRIC_SLEEP_PANEL_PAD_PX (16)
#endif

#ifndef METRIC_SLEEP_PANEL_ROW_GAP_PX
#define METRIC_SLEEP_PANEL_ROW_GAP_PX (10)
#endif

#ifndef METRIC_SLEEP_TITLE_FONT
#define METRIC_SLEEP_TITLE_FONT HL_FONT_TITLE
#endif

#ifndef METRIC_SLEEP_TITLE_COLOR_HEX
#define METRIC_SLEEP_TITLE_COLOR_HEX (0x002329)
#endif

#ifndef METRIC_SLEEP_PERIOD_FONT
#define METRIC_SLEEP_PERIOD_FONT HL_FONT_HEADING
#endif

#ifndef METRIC_SLEEP_PERIOD_TEXT_COLOR_HEX
#define METRIC_SLEEP_PERIOD_TEXT_COLOR_HEX (0x1D2939)
#endif

#ifndef METRIC_SLEEP_PERIOD_BTN_SIZE_PX
#define METRIC_SLEEP_PERIOD_BTN_SIZE_PX HL_SCALE_V(38)
#endif

#ifndef METRIC_SLEEP_PERIOD_BTN_BG_COLOR_HEX
#define METRIC_SLEEP_PERIOD_BTN_BG_COLOR_HEX (0xE7E8EB)
#endif

#ifndef METRIC_SLEEP_PERIOD_BTN_TEXT_FONT
#define METRIC_SLEEP_PERIOD_BTN_TEXT_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SLEEP_PERIOD_BTN_TEXT_COLOR_HEX
#define METRIC_SLEEP_PERIOD_BTN_TEXT_COLOR_HEX (0x101828)
#endif

#ifndef METRIC_SLEEP_PERIOD_CHIP_MIN_W_PX
#define METRIC_SLEEP_PERIOD_CHIP_MIN_W_PX (232)
#endif

#ifndef METRIC_SLEEP_PERIOD_CHIP_HEIGHT_PX
#define METRIC_SLEEP_PERIOD_CHIP_HEIGHT_PX HL_SCALE_V(38)
#endif

#ifndef METRIC_SLEEP_PERIOD_CHIP_RADIUS_PX
#define METRIC_SLEEP_PERIOD_CHIP_RADIUS_PX (22)
#endif

#ifndef METRIC_SLEEP_PERIOD_CHIP_BG_COLOR_HEX
#define METRIC_SLEEP_PERIOD_CHIP_BG_COLOR_HEX (0xE7E8EB)
#endif

#ifndef METRIC_SLEEP_AVG_LABEL_FONT
#define METRIC_SLEEP_AVG_LABEL_FONT HL_FONT_TITLE
#endif

#ifndef METRIC_SLEEP_AVG_VALUE_FONT
#define METRIC_SLEEP_AVG_VALUE_FONT HL_FONT_HERO
#endif

#ifndef METRIC_SLEEP_AVG_UNIT_FONT
#define METRIC_SLEEP_AVG_UNIT_FONT HL_FONT_CAPTION
#endif

#ifndef METRIC_SLEEP_AVG_TEXT_COLOR_HEX
#define METRIC_SLEEP_AVG_TEXT_COLOR_HEX (0x1D2939)
#endif

#ifndef METRIC_SLEEP_AVG_MUTED_COLOR_HEX
#define METRIC_SLEEP_AVG_MUTED_COLOR_HEX (0x667085)
#endif

#ifndef METRIC_SLEEP_CHART_PANEL_H_PX
#define METRIC_SLEEP_CHART_PANEL_H_PX HL_SCALE_V(250)
#endif

#ifndef METRIC_SLEEP_CHART_PANEL_BG_COLOR_HEX
#define METRIC_SLEEP_CHART_PANEL_BG_COLOR_HEX (0xF7F8FA)
#endif

#ifndef METRIC_SLEEP_CHART_PANEL_RADIUS_PX
#define METRIC_SLEEP_CHART_PANEL_RADIUS_PX (14)
#endif

#ifndef METRIC_SLEEP_CHART_PANEL_PAD_PX
#define METRIC_SLEEP_CHART_PANEL_PAD_PX (12)
#endif

#ifndef METRIC_SLEEP_AXIS_W_PX
#define METRIC_SLEEP_AXIS_W_PX (58)
#endif

#ifndef METRIC_SLEEP_AXIS_FONT
#define METRIC_SLEEP_AXIS_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SLEEP_AXIS_COLOR_HEX
#define METRIC_SLEEP_AXIS_COLOR_HEX (0x8A94A2)
#endif

#ifndef METRIC_SLEEP_DAY_WRAP_W_PX
#define METRIC_SLEEP_DAY_WRAP_W_PX (18)
#endif

#ifndef METRIC_SLEEP_BAR_BG_COLOR_HEX
#define METRIC_SLEEP_BAR_BG_COLOR_HEX (0xDDE6F7)
#endif

#ifndef METRIC_SLEEP_STAGE_AWAKE_COLOR_HEX
#define METRIC_SLEEP_STAGE_AWAKE_COLOR_HEX (0xF58D73)
#endif

#ifndef METRIC_SLEEP_STAGE_REM_COLOR_HEX
#define METRIC_SLEEP_STAGE_REM_COLOR_HEX (0x16B7A8)
#endif

#ifndef METRIC_SLEEP_STAGE_CORE_COLOR_HEX
#define METRIC_SLEEP_STAGE_CORE_COLOR_HEX (0x2E90FA)
#endif

#ifndef METRIC_SLEEP_STAGE_DEEP_COLOR_HEX
#define METRIC_SLEEP_STAGE_DEEP_COLOR_HEX (0x3B2EA3)
#endif

#ifndef METRIC_SLEEP_LEGEND_ITEM_RADIUS_PX
#define METRIC_SLEEP_LEGEND_ITEM_RADIUS_PX (20)
#endif

#ifndef METRIC_SLEEP_LEGEND_ITEM_BG_COLOR_HEX
#define METRIC_SLEEP_LEGEND_ITEM_BG_COLOR_HEX (0xF7F8FA)
#endif

#ifndef METRIC_SLEEP_LEGEND_DOT_SIZE_PX
#define METRIC_SLEEP_LEGEND_DOT_SIZE_PX (10)
#endif

#ifndef METRIC_SLEEP_LEGEND_LABEL_FONT
#define METRIC_SLEEP_LEGEND_LABEL_FONT HL_FONT_BODY
#endif

#ifndef METRIC_SLEEP_LEGEND_VALUE_FONT
#define METRIC_SLEEP_LEGEND_VALUE_FONT HL_FONT_HERO
#endif

static uint32_t get_sleep_stage_color_by_id(uint32_t stage_id) {
  switch (stage_id) {
  case 1U:
    return METRIC_SLEEP_STAGE_AWAKE_COLOR_HEX;
  case 2U:
    return METRIC_SLEEP_STAGE_REM_COLOR_HEX;
  case 3U:
    return METRIC_SLEEP_STAGE_DEEP_COLOR_HEX;
  case 0U:
  default:
    return METRIC_SLEEP_STAGE_CORE_COLOR_HEX;
  }
}

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

static void create_duration_block(lv_obj_t *parent, const char *title, uint32_t hour,
                                  uint32_t minute) {
  lv_obj_t *col;
  lv_obj_t *title_label;
  lv_obj_t *value_row;
  lv_obj_t *hour_label;
  lv_obj_t *hour_unit;
  lv_obj_t *min_label;
  lv_obj_t *min_unit;
  char hour_text[8];
  char min_text[8];

  col = create_transparent_box(parent, 0, LV_SIZE_CONTENT, 1);
  lv_obj_set_layout(col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_row(col, 2, LV_PART_MAIN);

  title_label = lv_label_create(col);
  lv_label_set_text(title_label, title);
  lv_obj_set_style_text_font(title_label, METRIC_SLEEP_AVG_LABEL_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_label, lv_color_hex(METRIC_SLEEP_AVG_TEXT_COLOR_HEX),
                              LV_PART_MAIN);

  value_row = create_transparent_row(col, 6, LV_FLEX_ALIGN_START);
  (void)snprintf(hour_text, sizeof(hour_text), "%lu", (unsigned long)hour);
  (void)snprintf(min_text, sizeof(min_text), "%02lu", (unsigned long)minute);

  hour_label = lv_label_create(value_row);
  lv_label_set_text(hour_label, hour_text);
  lv_obj_set_style_text_font(hour_label, METRIC_SLEEP_AVG_VALUE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(hour_label, lv_color_hex(METRIC_SLEEP_AVG_TEXT_COLOR_HEX),
                              LV_PART_MAIN);

  hour_unit = lv_label_create(value_row);
  lv_label_set_text(hour_unit, "hrs");
  lv_obj_set_style_text_font(hour_unit, METRIC_SLEEP_AVG_UNIT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(hour_unit, lv_color_hex(METRIC_SLEEP_AVG_MUTED_COLOR_HEX),
                              LV_PART_MAIN);

  min_label = lv_label_create(value_row);
  lv_label_set_text(min_label, min_text);
  lv_obj_set_style_text_font(min_label, METRIC_SLEEP_AVG_VALUE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(min_label, lv_color_hex(METRIC_SLEEP_AVG_TEXT_COLOR_HEX),
                              LV_PART_MAIN);

  min_unit = lv_label_create(value_row);
  lv_label_set_text(min_unit, "min");
  lv_obj_set_style_text_font(min_unit, METRIC_SLEEP_AVG_UNIT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(min_unit, lv_color_hex(METRIC_SLEEP_AVG_MUTED_COLOR_HEX),
                              LV_PART_MAIN);
}

static void create_sleep_bar(lv_obj_t *parent, uint8_t top_pct, uint8_t height_pct,
                             uint32_t day_index) {
  static const uint8_t k_seg_pct_base[10] = {10U, 12U, 14U, 8U, 14U,
                                             10U, 12U, 9U,  7U,  4U};
  static const uint8_t k_stage_pattern[10] = {0U, 3U, 0U, 2U, 0U,
                                              3U, 0U, 2U, 0U, 1U};
  lv_obj_t *day_wrap;
  lv_obj_t *spacer;
  lv_obj_t *bar;
  lv_obj_t *segment;
  lv_coord_t bar_h;
  lv_coord_t pct_h;
  lv_coord_t min_seg_h;
  lv_coord_t tail_min_h;
  lv_coord_t alloc_h;
  lv_coord_t remaining_h;
  lv_coord_t seg_h;
  uint8_t seg_pct[10];
  uint32_t segment_count;
  uint32_t i;
  uint32_t stage_id;
  uint32_t stage_color_hex;

  day_wrap = create_transparent_box(parent, METRIC_SLEEP_DAY_WRAP_W_PX, lv_pct(100), 0);
  lv_obj_set_layout(day_wrap, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(day_wrap, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(day_wrap, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

  spacer = lv_obj_create(day_wrap);
  lv_obj_set_size(spacer, lv_pct(100), lv_pct(top_pct));
  lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(spacer, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(spacer, 0, LV_PART_MAIN);
  lv_obj_clear_flag(spacer, LV_OBJ_FLAG_SCROLLABLE);

  bar = lv_obj_create(day_wrap);
  lv_obj_set_size(bar, lv_pct(100), lv_pct(height_pct));
  lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
  lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(bar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(bar, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(bar, 0, LV_PART_MAIN);
  lv_obj_set_layout(bar, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(bar, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(bar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_update_layout(bar);
  bar_h = lv_obj_get_height(bar);
  if (bar_h <= 0) {
    return;
  }

  segment_count = 10U;
  for (i = 0U; i < segment_count; i++) {
    seg_pct[i] = k_seg_pct_base[i];
  }
  /* Mild deterministic variation while keeping realistic contiguous sleep cycles. */
  if ((day_index % 6U) == 0U) {
    if (seg_pct[1] > 1U) {
      seg_pct[1] -= 1U;
    }
    if (seg_pct[5] > 1U) {
      seg_pct[5] -= 1U;
    }
    seg_pct[8] += 1U;
    seg_pct[9] += 1U;
  }
  if ((day_index % 5U) == 0U) {
    if (seg_pct[4] > 1U) {
      seg_pct[4] -= 1U;
    }
    if (seg_pct[6] > 1U) {
      seg_pct[6] -= 1U;
    }
    seg_pct[3] += 1U;
    seg_pct[7] += 1U;
  }

  min_seg_h = 2;
  remaining_h = bar_h;
  for (i = 0U; i < segment_count; i++) {
    if (i == (segment_count - 1U)) {
      alloc_h = remaining_h;
    } else {
      pct_h = (lv_coord_t)(((int32_t)bar_h * (int32_t)seg_pct[i]) / 100);
      if (pct_h < min_seg_h) {
        pct_h = min_seg_h;
      }
      tail_min_h = (lv_coord_t)((segment_count - i - 1U) * min_seg_h);
      if (remaining_h <= tail_min_h) {
        alloc_h = min_seg_h;
      } else {
        lv_coord_t max_h = (lv_coord_t)(remaining_h - tail_min_h);
        alloc_h = (pct_h < max_h) ? pct_h : max_h;
      }
    }
    if (alloc_h < min_seg_h) {
      alloc_h = min_seg_h;
    }
    if (alloc_h > remaining_h) {
      alloc_h = remaining_h;
    }
    seg_h = alloc_h;
    remaining_h -= seg_h;

    stage_id = k_stage_pattern[i];
    if ((8U == i) && ((day_index % 7U) == 0U)) {
      stage_id = 1U;
    }
    stage_color_hex = get_sleep_stage_color_by_id(stage_id);
    segment = lv_obj_create(bar);
    lv_obj_set_size(segment, lv_pct(100), seg_h);
    lv_obj_set_style_bg_color(segment, lv_color_hex(stage_color_hex), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(segment, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(segment, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(segment, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(segment, 0, LV_PART_MAIN);
    lv_obj_clear_flag(segment, LV_OBJ_FLAG_SCROLLABLE);

    if (0 == remaining_h) {
      break;
    }
  }
}

static void create_legend_item(lv_obj_t *parent, uint32_t dot_color_hex,
                               const char *label_text, const char *value_text) {
  lv_obj_t *item;
  lv_obj_t *dot;
  lv_obj_t *label;
  lv_obj_t *value;

  item = lv_obj_create(parent);
  lv_obj_set_size(item, 0, 40);
  lv_obj_set_flex_grow(item, 1);
  lv_obj_set_style_bg_color(item, lv_color_hex(METRIC_SLEEP_LEGEND_ITEM_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(item, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(item, METRIC_SLEEP_LEGEND_ITEM_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(item, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(item, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(item, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_top(item, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(item, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(item, 8, LV_PART_MAIN);
  lv_obj_set_layout(item, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(item, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(item, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);

  dot = lv_obj_create(item);
  lv_obj_set_size(dot, METRIC_SLEEP_LEGEND_DOT_SIZE_PX, METRIC_SLEEP_LEGEND_DOT_SIZE_PX);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(dot, lv_color_hex(dot_color_hex), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(dot, 0, LV_PART_MAIN);
  lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(item);
  lv_label_set_text(label, label_text);
  lv_obj_set_style_text_font(label, METRIC_SLEEP_LEGEND_LABEL_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(label, lv_color_hex(METRIC_SLEEP_AVG_TEXT_COLOR_HEX), LV_PART_MAIN);

  value = lv_label_create(item);
  lv_label_set_text(value, value_text);
  lv_obj_set_style_text_font(value, METRIC_SLEEP_LEGEND_LABEL_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(value, lv_color_hex(METRIC_SLEEP_AVG_TEXT_COLOR_HEX), LV_PART_MAIN);
}

void page_metric_sleep_detail_layout_build(
    lv_obj_t *parent, const health_ui_sleep_metric_detail_data_t *data) {
  static const char *k_axis_labels[] = {"8 PM", "10 PM", "12 AM", "2 AM", "4 AM", "6 AM", "8 AM"};
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
  lv_obj_t *avg_row;
  lv_obj_t *chart_panel;
  lv_obj_t *chart_row;
  lv_obj_t *axis_col;
  lv_obj_t *bars_col;
  lv_obj_t *bars_area;
  lv_obj_t *ticks_row;
  lv_obj_t *legend_row;
  uint32_t i;
  char legend_buf[32];

  if ((NULL == parent) || (NULL == data)) {
    return;
  }

  panel = lv_obj_create(parent);
  lv_obj_set_size(panel, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(panel, lv_color_hex(METRIC_SLEEP_PANEL_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(panel, METRIC_SLEEP_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(panel, METRIC_SLEEP_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(panel, METRIC_SLEEP_PANEL_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  header_row = create_transparent_row(panel, 8, LV_FLEX_ALIGN_SPACE_BETWEEN);
  title_label = lv_label_create(header_row);
  lv_label_set_text(title_label, "Sleep");
  lv_obj_set_style_text_font(title_label, METRIC_SLEEP_TITLE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_label, lv_color_hex(METRIC_SLEEP_TITLE_COLOR_HEX), LV_PART_MAIN);

  range_row = create_transparent_row(header_row, 8, LV_FLEX_ALIGN_START);
  lv_obj_set_width(range_row, LV_SIZE_CONTENT);
  prev_btn = lv_button_create(range_row);
  lv_obj_set_size(prev_btn, METRIC_SLEEP_PERIOD_BTN_SIZE_PX, METRIC_SLEEP_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(prev_btn, lv_color_hex(METRIC_SLEEP_PERIOD_BTN_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(prev_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(prev_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(prev_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(prev_btn, 0, LV_PART_MAIN);
  prev_label = lv_label_create(prev_btn);
  lv_label_set_text(prev_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_font(prev_label, METRIC_SLEEP_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(prev_label, lv_color_hex(METRIC_SLEEP_PERIOD_BTN_TEXT_COLOR_HEX), LV_PART_MAIN);
  lv_obj_center(prev_label);

  period_chip = lv_obj_create(range_row);
  lv_obj_set_size(period_chip, METRIC_SLEEP_PERIOD_CHIP_MIN_W_PX, METRIC_SLEEP_PERIOD_CHIP_HEIGHT_PX);
  lv_obj_set_style_bg_color(period_chip, lv_color_hex(METRIC_SLEEP_PERIOD_CHIP_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(period_chip, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(period_chip, METRIC_SLEEP_PERIOD_CHIP_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(period_chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(period_chip, 0, LV_PART_MAIN);
  lv_obj_clear_flag(period_chip, LV_OBJ_FLAG_SCROLLABLE);
  period_label = lv_label_create(period_chip);
  lv_label_set_text(period_label, (NULL != data->period_text) ? data->period_text : "--");
  lv_obj_set_style_text_font(period_label, METRIC_SLEEP_PERIOD_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(period_label, lv_color_hex(METRIC_SLEEP_PERIOD_TEXT_COLOR_HEX), LV_PART_MAIN);
  lv_obj_center(period_label);

  next_btn = lv_button_create(range_row);
  lv_obj_set_size(next_btn, METRIC_SLEEP_PERIOD_BTN_SIZE_PX, METRIC_SLEEP_PERIOD_BTN_SIZE_PX);
  lv_obj_set_style_bg_color(next_btn, lv_color_hex(METRIC_SLEEP_PERIOD_BTN_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(next_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(next_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_border_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(next_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(next_btn, 0, LV_PART_MAIN);
  next_label = lv_label_create(next_btn);
  lv_label_set_text(next_label, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_font(next_label, METRIC_SLEEP_PERIOD_BTN_TEXT_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(next_label, lv_color_hex(METRIC_SLEEP_PERIOD_BTN_TEXT_COLOR_HEX), LV_PART_MAIN);
  lv_obj_center(next_label);

  avg_row = create_transparent_row(panel, 16, LV_FLEX_ALIGN_START);
  create_duration_block(avg_row, "AVG. Time in bed", data->avg_time_in_bed_hour,
                        data->avg_time_in_bed_minute);
  create_duration_block(avg_row, "AVG. Time Asleep", data->avg_time_asleep_hour,
                        data->avg_time_asleep_minute);

  chart_panel = lv_obj_create(panel);
  lv_obj_set_size(chart_panel, lv_pct(100), METRIC_SLEEP_CHART_PANEL_H_PX);
  lv_obj_set_style_bg_color(chart_panel, lv_color_hex(METRIC_SLEEP_CHART_PANEL_BG_COLOR_HEX), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(chart_panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(chart_panel, METRIC_SLEEP_CHART_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(chart_panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(chart_panel, METRIC_SLEEP_CHART_PANEL_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(chart_panel, 6, LV_PART_MAIN);
  lv_obj_set_layout(chart_panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(chart_panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(chart_panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(chart_panel, LV_OBJ_FLAG_SCROLLABLE);

  chart_row = create_transparent_row(chart_panel, 6, LV_FLEX_ALIGN_START);
  lv_obj_set_size(chart_row, lv_pct(100), 0);
  lv_obj_set_flex_grow(chart_row, 1);

  axis_col = create_transparent_box(chart_row, METRIC_SLEEP_AXIS_W_PX, lv_pct(100), 0);
  lv_obj_set_layout(axis_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(axis_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(axis_col, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  for (i = 0U; i < (uint32_t)(sizeof(k_axis_labels) / sizeof(k_axis_labels[0])); i++) {
    lv_obj_t *label = lv_label_create(axis_col);
    lv_label_set_text(label, k_axis_labels[i]);
    lv_obj_set_style_text_font(label, METRIC_SLEEP_AXIS_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(METRIC_SLEEP_AXIS_COLOR_HEX), LV_PART_MAIN);
  }

  bars_col = create_transparent_box(chart_row, 0, lv_pct(100), 1);
  lv_obj_set_layout(bars_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(bars_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(bars_col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_row(bars_col, 6, LV_PART_MAIN);

  bars_area = create_transparent_row(bars_col, 4, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_size(bars_area, lv_pct(100), 0);
  lv_obj_set_flex_grow(bars_area, 1);
  for (i = 0U; i < data->day_count && i < HEALTH_UI_SLEEP_DETAIL_MAX_DAYS; i++) {
    create_sleep_bar(bars_area, data->bar_top_pct[i], data->bar_height_pct[i], i);
  }

  ticks_row = create_transparent_row(bars_col, 0, LV_FLEX_ALIGN_SPACE_BETWEEN);
  lv_obj_set_width(ticks_row, lv_pct(100));
  for (i = 0U; i < HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT; i++) {
    lv_obj_t *tick = lv_label_create(ticks_row);
    lv_label_set_text(tick, (NULL != data->x_tick_labels[i]) ? data->x_tick_labels[i] : "");
    lv_obj_set_style_text_font(tick, METRIC_SLEEP_AXIS_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(tick, lv_color_hex(METRIC_SLEEP_AXIS_COLOR_HEX), LV_PART_MAIN);
  }

  legend_row = create_transparent_row(panel, 10, LV_FLEX_ALIGN_SPACE_BETWEEN);
  (void)snprintf(legend_buf, sizeof(legend_buf), "%lu min", (unsigned long)data->avg_awake_minute);
  create_legend_item(legend_row, METRIC_SLEEP_STAGE_AWAKE_COLOR_HEX, "Average Awake", legend_buf);
  (void)snprintf(legend_buf, sizeof(legend_buf), "%lu hr %02lu min",
                 (unsigned long)data->avg_rem_hour, (unsigned long)data->avg_rem_minute);
  create_legend_item(legend_row, METRIC_SLEEP_STAGE_REM_COLOR_HEX, "Average REM", legend_buf);
  (void)snprintf(legend_buf, sizeof(legend_buf), "%lu hr %02lu min",
                 (unsigned long)data->avg_core_hour, (unsigned long)data->avg_core_minute);
  create_legend_item(legend_row, METRIC_SLEEP_STAGE_CORE_COLOR_HEX, "Average Core", legend_buf);
  (void)snprintf(legend_buf, sizeof(legend_buf), "%lu min", (unsigned long)data->avg_deep_minute);
  create_legend_item(legend_row, METRIC_SLEEP_STAGE_DEEP_COLOR_HEX, "Average Deep", legend_buf);
}
