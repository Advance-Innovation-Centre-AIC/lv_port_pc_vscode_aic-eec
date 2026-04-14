#include "comp_section_header.h"

#include "../../layout/layout_tokens.h"

lv_obj_t *comp_section_header_create(lv_obj_t *parent, const char *title,
                                     const char *subtitle) {
  lv_obj_t *panel;
  lv_obj_t *title_label;
  lv_obj_t *subtitle_label;

  if (NULL == parent) {
    return NULL;
  }

  panel = lv_obj_create(parent);
  lv_obj_set_size(panel, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_style_radius(panel, HL_RADIUS_MD, LV_PART_MAIN);
  lv_obj_set_style_bg_color(panel, lv_color_hex(HL_COLOR_SURFACE), LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(panel, lv_color_hex(HL_COLOR_BORDER), LV_PART_MAIN);
  lv_obj_set_style_pad_all(panel, HL_SPACE_MD, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(panel, 8, LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(panel, LV_OPA_10, LV_PART_MAIN);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  title_label = lv_label_create(panel);
  lv_label_set_text(title_label, (NULL != title) ? title : "Section");
  lv_obj_set_style_text_color(title_label, lv_color_hex(HL_COLOR_TEXT_PRIMARY),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, LV_PART_MAIN);
  lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 0, 0);

  subtitle_label = lv_label_create(panel);
  lv_label_set_text(subtitle_label, (NULL != subtitle) ? subtitle : "");
  lv_obj_set_style_text_color(subtitle_label, lv_color_hex(HL_COLOR_TEXT_MUTED),
                              LV_PART_MAIN);
  lv_obj_align_to(subtitle_label, title_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0,
                  HL_SPACE_XS);

  return panel;
}
