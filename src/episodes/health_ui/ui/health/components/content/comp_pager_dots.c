#include "comp_pager_dots.h"

#include <stdbool.h>

static void set_dot_visual(lv_obj_t *dot, bool active) {
  if (NULL == dot) {
    return;
  }

  lv_obj_set_size(dot, COMP_PAGER_DOT_SIZE_PX, COMP_PAGER_DOT_SIZE_PX);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, LV_PART_MAIN);
  lv_obj_set_style_bg_color(
      dot,
      lv_color_hex(active ? COMP_PAGER_DOT_ACTIVE_COLOR_HEX
                          : COMP_PAGER_DOT_INACTIVE_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN);
  lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
}

lv_obj_t *comp_pager_dots_create(lv_obj_t *parent) {
  lv_obj_t *dots;

  if (NULL == parent) {
    return NULL;
  }

  dots = lv_obj_create(parent);
  lv_obj_set_size(dots, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(dots, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(dots, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(dots, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(dots, COMP_PAGER_DOT_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(dots, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(dots, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(dots, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(dots, LV_OBJ_FLAG_SCROLLABLE);

  return dots;
}

void comp_pager_dots_build(lv_obj_t *dots, uint32_t dot_count,
                           uint32_t active_idx) {
  uint32_t i;

  if (NULL == dots) {
    return;
  }

  lv_obj_clean(dots);
  for (i = 0U; i < dot_count; i++) {
    lv_obj_t *dot = lv_obj_create(dots);
    set_dot_visual(dot, i == active_idx);
  }
}

void comp_pager_dots_set_active(lv_obj_t *dots, uint32_t active_idx) {
  uint32_t i;
  uint32_t count;

  if (NULL == dots) {
    return;
  }

  count = (uint32_t)lv_obj_get_child_cnt(dots);
  for (i = 0U; i < count; i++) {
    lv_obj_t *dot = lv_obj_get_child(dots, (int32_t)i);
    set_dot_visual(dot, i == active_idx);
  }
}
