#include "comp_add_device_list_item.h"

lv_obj_t *comp_add_device_list_item_create(
    lv_obj_t *parent, const comp_add_device_list_item_data_t *data) {
  lv_obj_t *row;
  lv_obj_t *avatar;
  lv_obj_t *avatar_label;
  lv_obj_t *text_col;
  lv_obj_t *name_label;
  lv_obj_t *type_label;
  lv_obj_t *chevron_label;
  lv_obj_t *divider = NULL;
  const char *name = "Unknown Device";
  const char *type_text = "Unknown Type";
  const char *avatar_text = "";
  uint32_t avatar_bg_color = COMP_ADD_DEVICE_LIST_ITEM_AVATAR_BG_COLOR_HEX;
  bool show_divider = true;

  if (NULL == parent) {
    return NULL;
  }

  if (NULL != data) {
    if ((NULL != data->name) && (data->name[0] != '\0')) {
      name = data->name;
    }
    if ((NULL != data->type_text) && (data->type_text[0] != '\0')) {
      type_text = data->type_text;
    }
    if ((NULL != data->avatar_text) && (data->avatar_text[0] != '\0')) {
      avatar_text = data->avatar_text;
    }
    if (0U != data->avatar_bg_color_hex) {
      avatar_bg_color = data->avatar_bg_color_hex;
    }
    show_divider = data->show_divider;
  }

  row = lv_obj_create(parent);
  lv_obj_set_width(row, lv_pct(100));
  lv_obj_set_height(row, COMP_ADD_DEVICE_LIST_ITEM_HEIGHT_PX);
  lv_obj_set_style_bg_color(row, lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(row, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(row, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(row, 0, LV_PART_MAIN);
  lv_obj_set_style_outline_width(row, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(row, COMP_ADD_DEVICE_LIST_ITEM_SIDE_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(row, COMP_ADD_DEVICE_LIST_ITEM_SIDE_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(row, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(row, COMP_ADD_DEVICE_LIST_ITEM_COL_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(row, LV_OBJ_FLAG_CLICKABLE);

  avatar = lv_obj_create(row);
  lv_obj_set_size(avatar, COMP_ADD_DEVICE_LIST_ITEM_AVATAR_SIZE_PX,
                  COMP_ADD_DEVICE_LIST_ITEM_AVATAR_SIZE_PX);
  lv_obj_set_style_radius(avatar, COMP_ADD_DEVICE_LIST_ITEM_AVATAR_RADIUS_PX,
                          LV_PART_MAIN);
  lv_obj_set_style_bg_color(avatar, lv_color_hex(avatar_bg_color), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(avatar, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(avatar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(avatar, 0, LV_PART_MAIN);
  lv_obj_clear_flag(avatar, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(avatar, LV_OBJ_FLAG_EVENT_BUBBLE);

  avatar_label = lv_label_create(avatar);
  lv_label_set_text(avatar_label, avatar_text);
  lv_obj_set_style_text_color(avatar_label,
                              lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_AVATAR_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(avatar_label, COMP_ADD_DEVICE_LIST_ITEM_AVATAR_FONT,
                             LV_PART_MAIN);
  lv_obj_center(avatar_label);
  lv_obj_add_flag(avatar_label, LV_OBJ_FLAG_EVENT_BUBBLE);

  text_col = lv_obj_create(row);
  lv_obj_set_size(text_col, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_flex_grow(text_col, 1);
  lv_obj_set_style_bg_color(text_col, lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(text_col, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(text_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(text_col, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(text_col, COMP_ADD_DEVICE_LIST_ITEM_TEXT_ROW_GAP_PX,
                           LV_PART_MAIN);
  lv_obj_set_layout(text_col, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(text_col, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(text_col, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(text_col, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(text_col, LV_OBJ_FLAG_EVENT_BUBBLE);

  name_label = lv_label_create(text_col);
  lv_label_set_text(name_label, name);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(name_label, lv_pct(100));
  lv_obj_set_style_text_color(name_label,
                              lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_NAME_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(name_label, COMP_ADD_DEVICE_LIST_ITEM_NAME_FONT,
                             LV_PART_MAIN);
  lv_obj_add_flag(name_label, LV_OBJ_FLAG_EVENT_BUBBLE);

  type_label = lv_label_create(text_col);
  lv_label_set_text(type_label, type_text);
  lv_label_set_long_mode(type_label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(type_label, lv_pct(100));
  lv_obj_set_style_text_color(type_label,
                              lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_TYPE_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(type_label, COMP_ADD_DEVICE_LIST_ITEM_TYPE_FONT,
                             LV_PART_MAIN);
  lv_obj_add_flag(type_label, LV_OBJ_FLAG_EVENT_BUBBLE);

  chevron_label = lv_label_create(row);
  lv_label_set_text(chevron_label, COMP_ADD_DEVICE_LIST_ITEM_CHEVRON_TEXT);
  lv_obj_set_style_text_color(chevron_label,
                              lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_CHEVRON_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(chevron_label, COMP_ADD_DEVICE_LIST_ITEM_CHEVRON_FONT,
                             LV_PART_MAIN);
  lv_obj_add_flag(chevron_label, LV_OBJ_FLAG_EVENT_BUBBLE);

  if (show_divider) {
    /* Divider is drawn as a child object to avoid full-row border framing. */
    divider = lv_obj_create(row);
    lv_obj_add_flag(divider, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_clear_flag(divider, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(divider, lv_pct(100), COMP_ADD_DEVICE_LIST_ITEM_DIVIDER_WIDTH_PX);
    lv_obj_align(divider, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(divider,
                              lv_color_hex(COMP_ADD_DEVICE_LIST_ITEM_DIVIDER_COLOR_HEX),
                              LV_PART_MAIN);
    lv_obj_set_style_bg_opa(divider, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(divider, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(divider, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(divider, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(divider, 0, LV_PART_MAIN);
  }

  return row;
}
