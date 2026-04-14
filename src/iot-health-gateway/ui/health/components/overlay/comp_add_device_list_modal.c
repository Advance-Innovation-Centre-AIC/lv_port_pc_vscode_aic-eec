#include "comp_add_device_list_modal.h"

#include "../../sim/health_ui_sim_data.h"
#include "comp_add_device_flow_modal.h"
#include "comp_add_device_list_item.h"

typedef struct {
  uint32_t device_index;
  uint32_t device_id;
} add_device_row_ctx_t;

static lv_obj_t *s_msgbox = NULL;
static lv_obj_t *s_content = NULL;

static comp_add_device_list_modal_select_cb_t s_on_select = NULL;
static void *s_on_select_user_data = NULL;
static bool s_emit_select_on_delete = false;
static uint32_t s_selected_device_index = 0U;
static uint32_t s_selected_device_id = 0U;
static add_device_row_ctx_t s_row_ctx[ADD_DEVICE_MODAL_MAX_LIST_ITEMS];

static void on_msgbox_deleted(lv_event_t *e);
static void on_close_clicked(lv_event_t *e);
static void on_device_row_clicked(lv_event_t *e);
static void close_internal(bool keep_select_emit);

static void style_primary_text(lv_obj_t *obj, const lv_font_t *font) {
  if (NULL == obj) {
    return;
  }

  lv_obj_set_style_text_font(obj, font, LV_PART_MAIN);
  lv_obj_set_style_text_color(obj, lv_color_hex(ADD_DEVICE_MODAL_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_opa(obj, LV_OPA_100, LV_PART_MAIN);
}

static void style_modal_backdrop(lv_obj_t *msgbox) {
  lv_obj_t *backdrop;

  if (NULL == msgbox) {
    return;
  }

  backdrop = lv_obj_get_parent(msgbox);
  if (NULL == backdrop) {
    return;
  }

  lv_obj_set_style_bg_color(backdrop, lv_color_hex(ADD_DEVICE_MODAL_BACKDROP_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(backdrop, ADD_DEVICE_MODAL_BACKDROP_OPA, LV_PART_MAIN);
  lv_obj_set_style_border_width(backdrop, 0, LV_PART_MAIN);
}

static void style_modal_sheet(lv_obj_t *msgbox) {
  if (NULL == msgbox) {
    return;
  }

  lv_obj_set_size(msgbox, lv_pct(ADD_DEVICE_MODAL_WIDTH_PERCENT),
                  lv_pct(ADD_DEVICE_MODAL_HEIGHT_PERCENT));
  lv_obj_center(msgbox);
  lv_obj_set_style_radius(msgbox, ADD_DEVICE_MODAL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(msgbox, lv_color_hex(ADD_DEVICE_MODAL_SURFACE_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(msgbox, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(msgbox, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(msgbox, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(msgbox, ADD_DEVICE_MODAL_PAD_H_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(msgbox, ADD_DEVICE_MODAL_PAD_H_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(msgbox, ADD_DEVICE_MODAL_PAD_V_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(msgbox, ADD_DEVICE_MODAL_PAD_V_PX, LV_PART_MAIN);
}

static void style_header(lv_obj_t *msgbox) {
  lv_obj_t *header;

  if (NULL == msgbox) {
    return;
  }

  header = lv_msgbox_get_header(msgbox);
  if (NULL == header) {
    return;
  }

  lv_obj_set_style_bg_color(header, lv_color_hex(ADD_DEVICE_MODAL_HEADER_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(header, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(header, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(header, 0, LV_PART_MAIN);
}

static lv_obj_t *style_content_area(lv_obj_t *msgbox) {
  lv_obj_t *content;

  if (NULL == msgbox) {
    return NULL;
  }

  content = lv_msgbox_get_content(msgbox);
  if (NULL == content) {
    return NULL;
  }

  lv_obj_clear_flag(content, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_style_bg_color(content, lv_color_hex(ADD_DEVICE_MODAL_SURFACE_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(content, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(content, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(content, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(content, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_right(content, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_top(content, ADD_DEVICE_MODAL_CONTENT_TOP_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(content, ADD_DEVICE_MODAL_CONTENT_BOTTOM_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(content, 0, LV_PART_MAIN);
  lv_obj_set_layout(content, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
#if (ADD_DEVICE_MODAL_CONTENT_SCROLLABLE != 0)
  lv_obj_set_scroll_dir(content, LV_DIR_VER);
  lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
#else
  lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(content, LV_DIR_NONE);
  lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
#endif

  return content;
}

static lv_obj_t *create_list_panel(lv_obj_t *parent) {
  lv_obj_t *panel;

  if (NULL == parent) {
    return NULL;
  }

  panel = lv_obj_create(parent);
  lv_obj_set_width(panel, lv_pct(100));
  lv_obj_set_height(panel, LV_SIZE_CONTENT);
  lv_obj_set_style_radius(panel, ADD_DEVICE_MODAL_LIST_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(panel, lv_color_hex(ADD_DEVICE_MODAL_LIST_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(panel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(panel, ADD_DEVICE_MODAL_LIST_PANEL_PAD_H_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(panel, ADD_DEVICE_MODAL_LIST_PANEL_PAD_H_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(panel, ADD_DEVICE_MODAL_LIST_PANEL_PAD_V_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(panel, ADD_DEVICE_MODAL_LIST_PANEL_PAD_V_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_row(panel, 0, LV_PART_MAIN);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  return panel;
}

static void style_header_slot_button(lv_obj_t *btn) {
  if (NULL == btn) {
    return;
  }

  lv_obj_set_size(btn, ADD_DEVICE_MODAL_HEADER_SIDE_SLOT_W_PX,
                  ADD_DEVICE_MODAL_HEADER_SIDE_SLOT_H_PX);
  lv_obj_set_style_bg_color(btn, lv_color_hex(ADD_DEVICE_MODAL_HEADER_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
}

static lv_obj_t *create_header_slot_button(lv_obj_t *msgbox, const char *text,
                                           const lv_font_t *font, lv_event_cb_t cb,
                                           bool clickable, bool visible,
                                           lv_obj_t **out_label) {
  lv_obj_t *btn;
  lv_obj_t *label;

  if ((NULL == msgbox) || (NULL == text) || (NULL == font)) {
    return NULL;
  }

  btn = lv_msgbox_add_header_button(msgbox, NULL);
  if (NULL == btn) {
    return NULL;
  }

  style_header_slot_button(btn);
  if (NULL != cb) {
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, msgbox);
  }
  if (!clickable) {
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  } else {
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  }

  label = lv_label_create(btn);
  lv_label_set_text(label, text);
  style_primary_text(label, font);
  lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
  lv_obj_set_width(label, lv_pct(100));
  if (!visible) {
    lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
  }
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_center(label);

  if (NULL != out_label) {
    *out_label = label;
  }

  return btn;
}

static void build_pairable_device_list(lv_obj_t *panel) {
  uint32_t i;
  uint32_t device_count;
  uint32_t render_count;

  if (NULL == panel) {
    return;
  }

  device_count = health_ui_sim_data_get_device_count();
  if (0U == device_count) {
    lv_obj_t *empty_label = lv_label_create(panel);
    lv_label_set_text(empty_label, "No pairable device.");
    style_primary_text(empty_label, ADD_DEVICE_MODAL_TITLE_FONT);
    return;
  }

  render_count = device_count;
  if (render_count > ADD_DEVICE_MODAL_MAX_LIST_ITEMS) {
    render_count = ADD_DEVICE_MODAL_MAX_LIST_ITEMS;
  }

  for (i = 0U; i < render_count; i++) {
    comp_device_card_data_t device_data;
    comp_add_device_list_item_data_t item_data;
    lv_obj_t *row;

    health_ui_sim_data_build_device_card(i, &device_data);
    item_data.name = device_data.name;
    item_data.type_text = device_data.type_text;
    item_data.avatar_text = device_data.avatar_text;
    item_data.avatar_bg_color_hex = device_data.avatar_bg_color_hex;
    item_data.show_divider = (i + 1U) < render_count;

    row = comp_add_device_list_item_create(panel, &item_data);
    if (NULL == row) {
      continue;
    }

    s_row_ctx[i].device_index = i;
    s_row_ctx[i].device_id = health_ui_sim_data_get_pairable_device_id(i);
    lv_obj_add_event_cb(row, on_device_row_clicked, LV_EVENT_CLICKED, &s_row_ctx[i]);
  }
}

static void on_msgbox_deleted(lv_event_t *e) {
  (void)e;
  s_content = NULL;
  s_msgbox = NULL;

  if (s_emit_select_on_delete && (NULL != s_on_select)) {
    s_on_select(s_selected_device_index, s_selected_device_id, s_on_select_user_data);
  }

  s_emit_select_on_delete = false;
  s_selected_device_index = 0U;
  s_selected_device_id = 0U;
}

static void on_close_clicked(lv_event_t *e) {
  lv_obj_t *msgbox = (lv_obj_t *)lv_event_get_user_data(e);
  if (NULL != msgbox) {
    (void)e;
    close_internal(false);
  }
}

static void on_device_row_clicked(lv_event_t *e) {
  add_device_row_ctx_t *ctx = (add_device_row_ctx_t *)lv_event_get_user_data(e);
  if (NULL == ctx) {
    return;
  }

  s_selected_device_index = ctx->device_index;
  s_selected_device_id = ctx->device_id;
  s_emit_select_on_delete = true;
  close_internal(true);
}

static void close_internal(bool keep_select_emit) {
  if (!keep_select_emit) {
    s_emit_select_on_delete = false;
  }

  if (NULL != s_msgbox) {
    lv_msgbox_close(s_msgbox);
  }
}

void comp_add_device_list_modal_open(comp_add_device_list_modal_select_cb_t on_select,
                                     void *user_data) {
  lv_obj_t *title_label;
  lv_obj_t *back_btn;
  lv_obj_t *back_label;

  s_on_select = on_select;
  s_on_select_user_data = user_data;

  if (NULL != s_msgbox) {
    lv_obj_move_foreground(s_msgbox);
    return;
  }

  s_emit_select_on_delete = false;
  s_selected_device_index = 0U;
  s_selected_device_id = 0U;

  s_msgbox = lv_msgbox_create(NULL);
  lv_obj_add_event_cb(s_msgbox, on_msgbox_deleted, LV_EVENT_DELETE, NULL);
  style_modal_backdrop(s_msgbox);
  style_modal_sheet(s_msgbox);

  back_btn = create_header_slot_button(s_msgbox, ADD_DEVICE_MODAL_BACK_LABEL_TEXT,
                                       ADD_DEVICE_MODAL_HEADER_BACK_FONT, NULL, false,
                                       false, &back_label);
  title_label = lv_msgbox_add_title(s_msgbox, ADD_DEVICE_MODAL_TITLE_TEXT);
  (void)create_header_slot_button(s_msgbox, ADD_DEVICE_MODAL_CLOSE_LABEL_TEXT,
                                  ADD_DEVICE_MODAL_CLOSE_FONT, on_close_clicked, true,
                                  true, NULL);

  if (NULL != back_btn) {
    lv_obj_move_to_index(back_btn, 0);
  }
  if (NULL != title_label) {
    style_primary_text(title_label, ADD_DEVICE_MODAL_TITLE_FONT);
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  }
  (void)back_label;

  style_header(s_msgbox);
  s_content = style_content_area(s_msgbox);

  if (NULL != s_content) {
    lv_obj_t *list_panel = create_list_panel(s_content);
    build_pairable_device_list(list_panel);
  }
}

void comp_add_device_list_modal_close(void) {
  close_internal(false);
}

bool comp_add_device_list_modal_is_open(void) { return (NULL != s_msgbox); }
