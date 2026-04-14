#include "comp_status_chip.h"

#include "../../layout/layout_tokens.h"

lv_obj_t *comp_status_chip_create(lv_obj_t *parent, const char *text,
                                  uint32_t bg_color, uint32_t fg_color) {
  lv_obj_t *chip;
  lv_obj_t *label;

  if (NULL == parent) {
    return NULL;
  }

  chip = lv_obj_create(parent);
  lv_obj_set_height(chip, 32);
  lv_obj_set_style_radius(chip, HL_RADIUS_XL, LV_PART_MAIN);
  lv_obj_set_style_bg_color(chip, lv_color_hex(bg_color), LV_PART_MAIN);
  lv_obj_set_style_border_width(chip, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(chip, HL_SPACE_MD, LV_PART_MAIN);
  lv_obj_set_style_pad_right(chip, HL_SPACE_MD, LV_PART_MAIN);
  lv_obj_set_style_pad_top(chip, HL_SPACE_XS, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(chip, HL_SPACE_XS, LV_PART_MAIN);
  lv_obj_clear_flag(chip, LV_OBJ_FLAG_SCROLLABLE);

  label = lv_label_create(chip);
  lv_label_set_text(label, (NULL != text) ? text : "");
  lv_obj_set_style_text_color(label, lv_color_hex(fg_color), LV_PART_MAIN);
  lv_obj_center(label);

  return chip;
}
