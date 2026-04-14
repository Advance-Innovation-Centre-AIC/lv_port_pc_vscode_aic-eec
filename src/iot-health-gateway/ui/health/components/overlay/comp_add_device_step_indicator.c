#include "comp_add_device_step_indicator.h"

#include <stdio.h>

static void style_step_circle(lv_obj_t *circle, lv_obj_t *label, bool active) {
  if ((NULL == circle) || (NULL == label)) {
    return;
  }

  lv_obj_set_size(circle, COMP_ADD_DEVICE_STEP_INDICATOR_CIRCLE_SIZE_PX,
                  COMP_ADD_DEVICE_STEP_INDICATOR_CIRCLE_SIZE_PX);
  lv_obj_set_style_radius(circle, COMP_ADD_DEVICE_STEP_INDICATOR_CIRCLE_RADIUS_PX,
                          LV_PART_MAIN);
  lv_obj_set_style_bg_color(
      circle, lv_color_hex(active ? COMP_ADD_DEVICE_STEP_INDICATOR_ACTIVE_BG_COLOR_HEX
                                  : COMP_ADD_DEVICE_STEP_INDICATOR_INACTIVE_BG_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(circle, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(circle, 0, LV_PART_MAIN);
  lv_obj_clear_flag(circle, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_text_font(label, COMP_ADD_DEVICE_STEP_INDICATOR_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(
      label, lv_color_hex(active ? COMP_ADD_DEVICE_STEP_INDICATOR_ACTIVE_TEXT_COLOR_HEX
                                 : COMP_ADD_DEVICE_STEP_INDICATOR_INACTIVE_TEXT_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_center(label);
}

static lv_obj_t *create_step_circle(lv_obj_t *parent, uint32_t step_no, bool active) {
  lv_obj_t *circle;
  lv_obj_t *label;
  char text_buf[12];

  if (NULL == parent) {
    return NULL;
  }

  circle = lv_obj_create(parent);
  label = lv_label_create(circle);
  (void)snprintf(text_buf, sizeof(text_buf), "%lu", (unsigned long)step_no);
  lv_label_set_text(label, text_buf);
  style_step_circle(circle, label, active);
  return circle;
}

static lv_obj_t *create_step_line(lv_obj_t *parent, bool active) {
  lv_obj_t *line;

  if (NULL == parent) {
    return NULL;
  }

  line = lv_obj_create(parent);
  lv_obj_set_height(line, COMP_ADD_DEVICE_STEP_INDICATOR_LINE_HEIGHT_PX);
  lv_obj_set_flex_grow(line, 1);
  lv_obj_set_style_bg_color(
      line, lv_color_hex(active ? COMP_ADD_DEVICE_STEP_INDICATOR_LINE_ACTIVE_COLOR_HEX
                                : COMP_ADD_DEVICE_STEP_INDICATOR_LINE_INACTIVE_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_bg_opa(line, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(line, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(line, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(line, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(line, 0, LV_PART_MAIN);
  lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
  return line;
}

lv_obj_t *comp_add_device_step_indicator_create(lv_obj_t *parent, uint32_t active_step,
                                                uint32_t total_steps) {
  lv_obj_t *root;
  lv_color_t parent_bg_color;
  uint32_t step;

  if (NULL == parent) {
    return NULL;
  }

  if (0U == total_steps) {
    total_steps = 1U;
  }
  if ((active_step < 1U) || (active_step > total_steps)) {
    active_step = 1U;
  }

  root = lv_obj_create(parent);
  lv_obj_set_width(root, lv_pct(100));
  lv_obj_set_height(root, COMP_ADD_DEVICE_STEP_INDICATOR_HEIGHT_PX);
  parent_bg_color = lv_obj_get_style_bg_color(parent, LV_PART_MAIN);
  lv_obj_set_style_bg_color(root, parent_bg_color, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(root, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(root, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(root, 14, LV_PART_MAIN);
  lv_obj_set_layout(root, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(root, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);

  for (step = 1U; step <= total_steps; step++) {
    (void)create_step_circle(root, step, step == active_step);
    if (step < total_steps) {
      (void)create_step_line(root, step < active_step);
    }
  }

  return root;
}
