#include "comp_add_device_step_modal.h"

#include "../../sim/health_ui_sim_data.h"
#include "comp_add_device_flow_modal.h"
#include "comp_add_device_step_indicator.h"

#include <stdio.h>

static lv_obj_t *s_msgbox = NULL;
static lv_obj_t *s_title_label = NULL;
static lv_obj_t *s_close_btn = NULL;
static lv_obj_t *s_content = NULL;
static lv_obj_t *s_step_root = NULL;
static lv_obj_t *s_indicator_slot = NULL;
static lv_obj_t *s_image_box = NULL;
static lv_obj_t *s_image_label = NULL;
static lv_obj_t *s_guide_label = NULL;
static lv_obj_t *s_cta_btn = NULL;
static lv_obj_t *s_cta_label = NULL;

static uint32_t s_selected_device_index = 0U;
static uint32_t s_selected_device_id = 0U;
static uint32_t s_active_step_index = 0U;
static char s_title_text[96];

static comp_add_device_step_modal_back_cb_t s_on_back = NULL;
static void *s_on_back_user_data = NULL;
static bool s_emit_back_on_delete = false;

static void on_msgbox_deleted(lv_event_t *e);
static void on_close_clicked(lv_event_t *e);
static void on_back_clicked(lv_event_t *e);
static void on_step_continue_clicked(lv_event_t *e);
static void build_step_modal_content(void);
static void close_internal(bool emit_back_on_delete);
static void force_modal_rerender(void);
static void reset_step_view_refs(void);
static void ensure_step_view_created(void);
static void apply_step_debug_colors(void);
#if (ADD_DEVICE_MODAL_STEP_DEBUG_LAYOUT != 0)
static void apply_debug_zone_style(lv_obj_t *obj, uint32_t color_hex);
#endif

static void style_primary_text(lv_obj_t *obj, const lv_font_t *font) {
  if (NULL == obj) {
    return;
  }

  lv_obj_set_style_text_font(obj, font, LV_PART_MAIN);
  lv_obj_set_style_text_color(obj, lv_color_hex(ADD_DEVICE_MODAL_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_opa(obj, LV_OPA_100, LV_PART_MAIN);
}

#if (ADD_DEVICE_MODAL_STEP_DEBUG_LAYOUT != 0)
static void apply_debug_zone_style(lv_obj_t *obj, uint32_t color_hex) {
  if (NULL == obj) {
    return;
  }

  lv_obj_set_style_bg_color(obj, lv_color_hex(color_hex), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(obj, ADD_DEVICE_MODAL_STEP_DEBUG_OPA, LV_PART_MAIN);
  lv_obj_set_style_border_color(obj, lv_color_hex(color_hex), LV_PART_MAIN);
  lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN);
}
#endif

static void apply_step_debug_colors(void) {
#if (ADD_DEVICE_MODAL_STEP_DEBUG_LAYOUT != 0)
  lv_obj_t *header = NULL;
  if (NULL != s_msgbox) {
    header = lv_msgbox_get_header(s_msgbox);
  }

  apply_debug_zone_style(header, ADD_DEVICE_MODAL_STEP_DEBUG_HEADER_COLOR_HEX);
  apply_debug_zone_style(s_content, ADD_DEVICE_MODAL_STEP_DEBUG_CONTENT_COLOR_HEX);
  apply_debug_zone_style(s_step_root, ADD_DEVICE_MODAL_STEP_DEBUG_ROOT_COLOR_HEX);
  apply_debug_zone_style(s_indicator_slot,
                         ADD_DEVICE_MODAL_STEP_DEBUG_INDICATOR_COLOR_HEX);
  apply_debug_zone_style(s_image_box, ADD_DEVICE_MODAL_STEP_DEBUG_IMAGE_COLOR_HEX);
  apply_debug_zone_style(s_guide_label, ADD_DEVICE_MODAL_STEP_DEBUG_GUIDE_COLOR_HEX);
  apply_debug_zone_style(s_cta_btn, ADD_DEVICE_MODAL_STEP_DEBUG_CTA_COLOR_HEX);
#endif
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
  apply_step_debug_colors();
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
  apply_step_debug_colors();

  return content;
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

static lv_obj_t *create_close_button(lv_obj_t *msgbox) {
  lv_obj_t *btn;
  lv_obj_t *label;

  if (NULL == msgbox) {
    return NULL;
  }

  btn = lv_msgbox_add_header_button(msgbox, NULL);
  if (NULL == btn) {
    return NULL;
  }
  style_header_slot_button(btn);
  lv_obj_add_event_cb(btn, on_close_clicked, LV_EVENT_CLICKED, msgbox);

  label = lv_label_create(btn);
  lv_label_set_text(label, ADD_DEVICE_MODAL_CLOSE_LABEL_TEXT);
  style_primary_text(label, ADD_DEVICE_MODAL_CLOSE_FONT);
  lv_obj_center(label);
  return btn;
}

static void reset_step_view_refs(void) {
  s_step_root = NULL;
  s_indicator_slot = NULL;
  s_image_box = NULL;
  s_image_label = NULL;
  s_guide_label = NULL;
  s_cta_btn = NULL;
  s_cta_label = NULL;
}

static void ensure_step_view_created(void) {
  if (NULL == s_content) {
    return;
  }
  if (NULL != s_step_root) {
    return;
  }

  lv_obj_clean(s_content);

  s_step_root = lv_obj_create(s_content);
  lv_obj_set_width(s_step_root, lv_pct(100));
  lv_obj_set_height(s_step_root, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(s_step_root, lv_color_hex(ADD_DEVICE_MODAL_SURFACE_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_step_root, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_step_root, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(s_step_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(s_step_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_right(s_step_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_top(s_step_root, ADD_DEVICE_MODAL_STEP_TOP_PAD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(s_step_root, ADD_DEVICE_MODAL_STEP_BOTTOM_PAD_PX,
                              LV_PART_MAIN);
  lv_obj_set_style_pad_row(s_step_root, ADD_DEVICE_MODAL_STEP_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(s_step_root, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(s_step_root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(s_step_root, ADD_DEVICE_MODAL_STEP_MAIN_ALIGN,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(s_step_root, LV_OBJ_FLAG_SCROLLABLE);

  s_indicator_slot = lv_obj_create(s_step_root);
  lv_obj_set_width(s_indicator_slot, lv_pct(100));
  lv_obj_set_height(s_indicator_slot, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(s_indicator_slot,
                            lv_color_hex(ADD_DEVICE_MODAL_SURFACE_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_indicator_slot, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_indicator_slot, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(s_indicator_slot, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_indicator_slot, 0, LV_PART_MAIN);
  lv_obj_clear_flag(s_indicator_slot, LV_OBJ_FLAG_SCROLLABLE);

  s_image_box = lv_obj_create(s_step_root);
  lv_obj_set_size(s_image_box, ADD_DEVICE_MODAL_STEP_IMAGE_BOX_SIZE_PX,
                  ADD_DEVICE_MODAL_STEP_IMAGE_BOX_SIZE_PX);
  lv_obj_set_style_radius(s_image_box, ADD_DEVICE_MODAL_STEP_IMAGE_BOX_RADIUS_PX,
                          LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_image_box, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_image_box, 2, LV_PART_MAIN);
  lv_obj_set_style_border_color(
      s_image_box, lv_color_hex(ADD_DEVICE_MODAL_STEP_IMAGE_BOX_BORDER_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_shadow_width(s_image_box, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_image_box, 0, LV_PART_MAIN);
  lv_obj_clear_flag(s_image_box, LV_OBJ_FLAG_SCROLLABLE);

  s_image_label = lv_label_create(s_image_box);
  lv_obj_set_style_text_font(s_image_label, ADD_DEVICE_MODAL_STEP_IMAGE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(
      s_image_label, lv_color_hex(ADD_DEVICE_MODAL_STEP_IMAGE_TEXT_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_center(s_image_label);

  s_guide_label = lv_label_create(s_step_root);
  lv_label_set_long_mode(s_guide_label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_pad_left(s_guide_label, ADD_DEVICE_MODAL_STEP_GUIDE_SIDE_PAD_PX,
                            LV_PART_MAIN);
  lv_obj_set_style_pad_right(s_guide_label, ADD_DEVICE_MODAL_STEP_GUIDE_SIDE_PAD_PX,
                             LV_PART_MAIN);
  lv_obj_set_style_bg_color(s_guide_label, lv_color_hex(ADD_DEVICE_MODAL_SURFACE_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_guide_label, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_text_align(s_guide_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_style_text_font(s_guide_label, ADD_DEVICE_MODAL_STEP_GUIDE_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(
      s_guide_label, lv_color_hex(ADD_DEVICE_MODAL_STEP_GUIDE_COLOR_HEX),
      LV_PART_MAIN);
  lv_obj_set_style_text_opa(s_guide_label, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_height(s_guide_label, ADD_DEVICE_MODAL_STEP_GUIDE_HEIGHT_PX);

  s_cta_btn = lv_button_create(s_step_root);
  lv_obj_set_size(s_cta_btn, ADD_DEVICE_MODAL_STEP_CTA_WIDTH_PX,
                  ADD_DEVICE_MODAL_STEP_CTA_HEIGHT_PX);
  lv_obj_set_style_radius(s_cta_btn, ADD_DEVICE_MODAL_STEP_CTA_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(s_cta_btn, lv_color_hex(ADD_DEVICE_MODAL_STEP_CTA_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_cta_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_cta_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(s_cta_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_cta_btn, 0, LV_PART_MAIN);
  lv_obj_add_event_cb(s_cta_btn, on_step_continue_clicked, LV_EVENT_CLICKED, NULL);

  s_cta_label = lv_label_create(s_cta_btn);
  lv_obj_set_style_text_font(s_cta_label, ADD_DEVICE_MODAL_STEP_CTA_FONT, LV_PART_MAIN);
  lv_obj_set_style_text_color(
      s_cta_label, lv_color_hex(ADD_DEVICE_MODAL_STEP_CTA_TEXT_COLOR_HEX), LV_PART_MAIN);
  lv_obj_center(s_cta_label);

  apply_step_debug_colors();
}

static void build_step_content(const health_ui_pairing_step_data_t *step_data,
                               uint32_t active_step, uint32_t total_steps) {
  const char *visual_text = "--";
  const char *instruction_text = "Follow device guide.";
  const char *cta_text = "Continue";
  uint32_t visual_bg_color_hex = ADD_DEVICE_MODAL_STEP_IMAGE_BOX_BG_COLOR_HEX;
  lv_coord_t guide_width_px;
  lv_obj_t *step_indicator;

  if (NULL == s_content) {
    return;
  }

  if (NULL != step_data) {
    if ((NULL != step_data->visual_text) && (step_data->visual_text[0] != '\0')) {
      visual_text = step_data->visual_text;
    }
    if ((NULL != step_data->instruction_text) &&
        (step_data->instruction_text[0] != '\0')) {
      instruction_text = step_data->instruction_text;
    }
    if ((NULL != step_data->cta_text) && (step_data->cta_text[0] != '\0')) {
      cta_text = step_data->cta_text;
    }
    if (0U != step_data->visual_bg_color_hex) {
      visual_bg_color_hex = step_data->visual_bg_color_hex;
    }
  }

  ensure_step_view_created();
  if ((NULL == s_step_root) || (NULL == s_indicator_slot) || (NULL == s_image_box) ||
      (NULL == s_image_label) || (NULL == s_guide_label) || (NULL == s_cta_btn) ||
      (NULL == s_cta_label)) {
    return;
  }

  lv_obj_update_layout(s_step_root);
  lv_obj_clean(s_indicator_slot);
  step_indicator =
      comp_add_device_step_indicator_create(s_indicator_slot, active_step, total_steps);
  if (NULL != step_indicator) {
    lv_obj_center(step_indicator);
  }

  lv_obj_set_style_bg_color(s_image_box, lv_color_hex(visual_bg_color_hex), LV_PART_MAIN);
  lv_label_set_text(s_image_label, visual_text);
  lv_obj_center(s_image_label);

  guide_width_px = (lv_obj_get_content_width(s_step_root) *
                    ADD_DEVICE_MODAL_STEP_GUIDE_WIDTH_PCT) /
                   100;
  if (guide_width_px < 280) {
    guide_width_px = 280;
  }
  lv_obj_set_width(s_guide_label, guide_width_px);
#if (ADD_DEVICE_MODAL_STEP_FORCE_STATIC_GUIDE_TEXT != 0)
  lv_label_set_text(s_guide_label, ADD_DEVICE_MODAL_STEP_STATIC_GUIDE_TEXT);
#else
  lv_label_set_text(s_guide_label, instruction_text);
#endif
  lv_obj_set_style_text_align(s_guide_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_set_height(s_guide_label, ADD_DEVICE_MODAL_STEP_GUIDE_HEIGHT_PX);
  lv_obj_update_layout(s_guide_label);
  lv_obj_invalidate(s_guide_label);
  lv_obj_invalidate(s_step_root);

  lv_label_set_text(s_cta_label, cta_text);
  lv_obj_center(s_cta_label);

  apply_step_debug_colors();
}

static void build_step_modal_content(void) {
  comp_device_card_data_t selected_device_data;
  health_ui_pairing_step_data_t step_data;
  uint32_t step_count;
  bool has_step_data;

  if ((NULL == s_msgbox) || (NULL == s_content)) {
    return;
  }

  health_ui_sim_data_build_device_card(s_selected_device_index, &selected_device_data);

  step_count = health_ui_sim_data_get_pairing_step_count(s_selected_device_index);
  if (0U == step_count) {
    step_count = 1U;
  }
  if (s_active_step_index >= step_count) {
    s_active_step_index = step_count - 1U;
  }

  has_step_data = health_ui_sim_data_build_pairing_step(s_selected_device_index,
                                                        s_active_step_index, &step_data);
  if (!has_step_data) {
    step_data.visual_text = selected_device_data.avatar_text;
    step_data.visual_bg_color_hex = selected_device_data.avatar_bg_color_hex;
    step_data.instruction_text =
        health_ui_sim_data_get_device_pairing_guide(s_selected_device_index);
    step_data.cta_text = "Continue";
  }

  build_step_content(&step_data, s_active_step_index + 1U, step_count);
  force_modal_rerender();
}

static void on_msgbox_deleted(lv_event_t *e) {
  (void)e;
  s_msgbox = NULL;
  s_title_label = NULL;
  s_close_btn = NULL;
  s_content = NULL;
  reset_step_view_refs();
  s_active_step_index = 0U;
  s_selected_device_index = 0U;
  s_selected_device_id = 0U;

  if (s_emit_back_on_delete && (NULL != s_on_back)) {
    s_on_back(s_on_back_user_data);
  }
  s_emit_back_on_delete = false;
}

static void on_close_clicked(lv_event_t *e) {
  lv_obj_t *msgbox = (lv_obj_t *)lv_event_get_user_data(e);
  if (NULL != msgbox) {
    (void)e;
    close_internal(false);
  }
}

static void on_back_clicked(lv_event_t *e) {
  (void)e;
  close_internal(true);
}

static void on_step_continue_clicked(lv_event_t *e) {
  uint32_t step_count;
  (void)e;

  step_count = health_ui_sim_data_get_pairing_step_count(s_selected_device_index);
  if (0U == step_count) {
    close_internal(true);
    return;
  }

  if ((s_active_step_index + 1U) < step_count) {
    s_active_step_index++;
    build_step_modal_content();
    return;
  }

  close_internal(true);
}

static void close_internal(bool emit_back_on_delete) {
  s_emit_back_on_delete = emit_back_on_delete;
  if (NULL != s_msgbox) {
    lv_msgbox_close(s_msgbox);
  }
}

static void force_modal_rerender(void) {
  if ((NULL == s_msgbox) || (NULL == s_content)) {
    return;
  }

  lv_obj_update_layout(s_content);
  lv_obj_update_layout(s_msgbox);
  lv_obj_invalidate(s_content);
  lv_obj_invalidate(s_msgbox);
}

void comp_add_device_step_modal_open(uint32_t device_index, uint32_t device_id,
                                     comp_add_device_step_modal_back_cb_t on_back,
                                     void *user_data) {
  comp_device_card_data_t selected_device_data;
  const char *safe_name = "Device";

  s_selected_device_index = device_index;
  s_selected_device_id = device_id;
  s_on_back = on_back;
  s_on_back_user_data = user_data;
  s_active_step_index = 0U;
  s_emit_back_on_delete = false;

  if (NULL != s_msgbox) {
    lv_obj_move_foreground(s_msgbox);
    build_step_modal_content();
    return;
  }

  health_ui_sim_data_build_device_card(s_selected_device_index, &selected_device_data);
  if ((NULL != selected_device_data.name) && (selected_device_data.name[0] != '\0')) {
    safe_name = selected_device_data.name;
  }
  (void)snprintf(s_title_text, sizeof(s_title_text), "%s %s",
                 ADD_DEVICE_MODAL_BACK_LABEL_TEXT, safe_name);

  s_msgbox = lv_msgbox_create(NULL);
  lv_obj_add_event_cb(s_msgbox, on_msgbox_deleted, LV_EVENT_DELETE, NULL);
  style_modal_backdrop(s_msgbox);
  style_modal_sheet(s_msgbox);

  s_title_label = lv_msgbox_add_title(s_msgbox, s_title_text);
  if (NULL != s_title_label) {
    style_primary_text(s_title_label, ADD_DEVICE_MODAL_HEADER_BACK_FONT);
    lv_obj_set_style_text_align(s_title_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_set_width(s_title_label, lv_pct(100));
    lv_obj_set_flex_grow(s_title_label, 1);
    lv_obj_set_style_pad_left(s_title_label, ADD_DEVICE_MODAL_HEADER_BACK_PAD_LEFT_PX,
                              LV_PART_MAIN);
    lv_label_set_long_mode(s_title_label, LV_LABEL_LONG_DOT);
    lv_obj_add_flag(s_title_label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(s_title_label, on_back_clicked, LV_EVENT_CLICKED, NULL);
  }

  s_close_btn = create_close_button(s_msgbox);
  style_header(s_msgbox);
  s_content = style_content_area(s_msgbox);
  build_step_modal_content();
}

void comp_add_device_step_modal_close(void) {
  close_internal(false);
}

bool comp_add_device_step_modal_is_open(void) { return (NULL != s_msgbox); }
