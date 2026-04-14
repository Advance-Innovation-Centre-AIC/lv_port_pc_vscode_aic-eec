#include "comp_device_card.h"

lv_obj_t *comp_device_card_create(lv_obj_t *parent,
                                  const comp_device_card_data_t *data) {
  lv_obj_t *card;
  lv_obj_t *avatar;
  lv_obj_t *avatar_text;
  lv_obj_t *text_col;
  lv_obj_t *name_label;
  lv_obj_t *type_label;
  const char *name = "Device";
  const char *type = "";
  const char *avatar_label = "";
  uint32_t avatar_bg_color = COMP_DEVICE_CARD_AVATAR_BG_COLOR_HEX;

  if (NULL == parent) {
    return NULL;
  }

  if (NULL != data) {
    name = ((NULL != data->name) && (data->name[0] != '\0')) ? data->name : name;
    type = ((NULL != data->type_text) && (data->type_text[0] != '\0')) ? data->type_text
                                                                        : type;
    avatar_label =
        ((NULL != data->avatar_text) && (data->avatar_text[0] != '\0'))
            ? data->avatar_text
            : avatar_label;
    if (0U != data->avatar_bg_color_hex) {
      avatar_bg_color = data->avatar_bg_color_hex;
    }
  }

  card = lv_obj_create(parent);
  /* Width may be overridden by dashboard runtime-fit logic for 4-card pages. */
  lv_obj_set_width(card, lv_pct(COMP_DEVICE_CARD_WIDTH_PCT));
  lv_obj_set_height(card, COMP_DEVICE_CARD_HEIGHT_PX);
  lv_obj_set_style_radius(card, COMP_DEVICE_CARD_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(card, lv_color_hex(COMP_DEVICE_CARD_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(card, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(card, COMP_DEVICE_CARD_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(card, COMP_DEVICE_CARD_PAD_HOR_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(card, COMP_DEVICE_CARD_PAD_VER_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(card, COMP_DEVICE_CARD_PAD_VER_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_column(card, COMP_DEVICE_CARD_COL_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(card, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(card, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  avatar = lv_obj_create(card);
  lv_obj_set_size(avatar, COMP_DEVICE_CARD_AVATAR_SIZE_PX,
                  COMP_DEVICE_CARD_AVATAR_SIZE_PX);
  lv_obj_set_style_radius(avatar, COMP_DEVICE_CARD_AVATAR_RADIUS_PX,
                          LV_PART_MAIN);
  lv_obj_set_style_bg_color(avatar, lv_color_hex(avatar_bg_color), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(avatar, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(avatar, 0, LV_PART_MAIN);
  lv_obj_clear_flag(avatar, LV_OBJ_FLAG_SCROLLABLE);

  avatar_text = lv_label_create(avatar);
  lv_label_set_text(avatar_text, avatar_label);
  lv_obj_set_style_text_color(avatar_text,
                              lv_color_hex(COMP_DEVICE_CARD_AVATAR_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(avatar_text, COMP_DEVICE_CARD_AVATAR_FONT,
                             LV_PART_MAIN);
  lv_obj_center(avatar_text);

  text_col = lv_obj_create(card);
  lv_obj_set_size(text_col, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_flex_grow(text_col, 1);
  lv_obj_set_style_bg_opa(text_col, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(text_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(text_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(text_col, COMP_DEVICE_CARD_TEXT_ROW_GAP_PX,
                           LV_PART_MAIN);
  lv_obj_set_layout(text_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(text_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(text_col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(text_col, LV_OBJ_FLAG_SCROLLABLE);

  name_label = lv_label_create(text_col);
  lv_label_set_text(name_label, name);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(name_label, lv_pct(100));
  lv_obj_set_style_text_color(name_label, lv_color_hex(COMP_DEVICE_CARD_NAME_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(name_label, COMP_DEVICE_CARD_NAME_FONT, LV_PART_MAIN);

  type_label = lv_label_create(text_col);
  lv_label_set_text(type_label, type);
  lv_label_set_long_mode(type_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(type_label, lv_pct(100));
  lv_obj_set_style_text_color(type_label, lv_color_hex(COMP_DEVICE_CARD_TYPE_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(type_label, COMP_DEVICE_CARD_TYPE_FONT, LV_PART_MAIN);

  return card;
}
