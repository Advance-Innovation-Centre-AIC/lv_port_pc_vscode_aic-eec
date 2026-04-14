#include "layout_debug_zone.h"

#include "layout_tokens.h"

lv_obj_t *health_layout_debug_zone_create(lv_obj_t *parent, const char *name,
                                          lv_coord_t width, lv_coord_t height,
                                          uint32_t bg_color_hex) {
  lv_obj_t *zone;
  lv_obj_t *label;

  if (NULL == parent) {
    return NULL;
  }

  zone = lv_obj_create(parent);
  lv_obj_set_size(zone, width, height);
  lv_obj_set_style_bg_color(zone, lv_color_hex(bg_color_hex), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(zone, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_border_width(zone, 2, LV_PART_MAIN);
  lv_obj_set_style_border_color(zone, lv_color_hex(0x7B8794), LV_PART_MAIN);
  lv_obj_set_style_radius(zone, HL_RADIUS_MD, LV_PART_MAIN);
  lv_obj_set_style_pad_all(zone, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(zone, 0, LV_PART_MAIN);
  lv_obj_clear_flag(zone, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(zone);
  lv_label_set_text(label, (NULL != name) ? name : "ZONE");
  lv_obj_set_style_text_color(label, lv_color_hex(0x102A43), LV_PART_MAIN);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

  return zone;
}
