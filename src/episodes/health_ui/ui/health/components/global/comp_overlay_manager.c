#include "comp_overlay_manager.h"

#include "../../layout/layout_tokens.h"
#include "../../layout/layout_fonts.h"

void comp_overlay_manager_create(lv_obj_t *screen) {
  lv_obj_t *overlay;
  lv_obj_t *title;
  lv_obj_t *body;

  if (NULL == screen) {
    return;
  }

  overlay = lv_obj_create(screen);
  lv_obj_set_size(overlay, 280, 74);
  lv_obj_align(overlay, LV_ALIGN_BOTTOM_RIGHT, -HL_SPACE_MD, -HL_SPACE_MD);
  lv_obj_set_style_radius(overlay, HL_RADIUS_LG, LV_PART_MAIN);
  lv_obj_set_style_bg_color(overlay, lv_color_hex(0x102A43), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(overlay, LV_OPA_80, LV_PART_MAIN);
  lv_obj_set_style_border_width(overlay, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(overlay, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(overlay, 12, LV_PART_MAIN);
  lv_obj_set_style_shadow_opa(overlay, LV_OPA_30, LV_PART_MAIN);
  lv_obj_clear_flag(overlay, LV_OBJ_FLAG_SCROLLABLE);

  title = lv_label_create(overlay);
  lv_label_set_text(title, "Overlay Manager");
  lv_obj_set_style_text_color(title, lv_color_hex(0xDCEEFB), LV_PART_MAIN);
  lv_obj_set_style_text_font(title, HL_FONT_HEADING, LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  body = lv_label_create(overlay);
  lv_label_set_text(body, "Popup/loading slot (layout ready)");
  lv_obj_set_style_text_color(body, lv_color_hex(0xBCCCDC), LV_PART_MAIN);
  lv_obj_align(body, LV_ALIGN_BOTTOM_LEFT, 0, 0);
}
