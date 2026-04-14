#include "comp_system_status_bar.h"

#include "../../layout/layout_i18n.h"
#include "../../layout/layout_tokens.h"

#include <stdio.h>
#include <string.h>

extern void health_ui_root_refresh_strings(void);

static lv_obj_t *s_status_root = NULL;
static lv_obj_t *s_status_shell = NULL;
static lv_obj_t *s_wifi_icon = NULL;
static lv_obj_t *s_bt_icon = NULL;
static lv_obj_t *s_time_label = NULL;
static lv_obj_t *s_date_label = NULL;
static lv_obj_t *s_locale_btn = NULL;
static lv_obj_t *s_locale_label = NULL;
static bool s_wifi_known = false;
static bool s_bt_known = false;
static bool s_wifi_cached = false;
static bool s_bt_cached = false;
static char s_time_cache[COMP_STATUS_BAR_TEXT_CACHE_SIZE];
static char s_date_cache[COMP_STATUS_BAR_TEXT_CACHE_SIZE];

static void set_status_text_style(lv_obj_t *label, const lv_font_t *font) {
  if (NULL == label) {
    return;
  }

  lv_obj_set_style_text_color(label, lv_color_hex(COMP_STATUS_BAR_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(label, font, LV_PART_MAIN);
}

static void reset_status_instances(void) {
  /* One delete at shell level is enough; children are owned by shell. */
  if (NULL != s_status_shell) {
    lv_obj_delete(s_status_shell);
  }

  s_status_shell = NULL;
  s_status_root = NULL;
  s_wifi_icon = NULL;
  s_bt_icon = NULL;
  s_time_label = NULL;
  s_date_label = NULL;
  s_locale_btn = NULL;
  s_locale_label = NULL;
}

static lv_obj_t *create_slot_label(lv_obj_t *parent, const char *text,
                                   lv_coord_t width, lv_text_align_t align,
                                   const lv_font_t *font) {
  lv_obj_t *label;

  label = lv_label_create(parent);
  lv_label_set_text(label, (NULL != text) ? text : "");
  set_status_text_style(label, font);
  lv_obj_set_width(label, width);
  lv_obj_set_style_text_align(label, align, LV_PART_MAIN);
  lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
  lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

  return label;
}

static void set_icon_state(lv_obj_t *icon, bool connected) {
  lv_color_t color;

  if (NULL == icon) {
    return;
  }

  /* Connectivity-specific visual variants can be introduced here later. */
  (void)connected;
  color = lv_color_hex(COMP_STATUS_BAR_ICON_COLOR_HEX);
  lv_obj_set_style_text_color(icon, color, LV_PART_MAIN);
}

static void update_icon_if_changed(lv_obj_t *icon, bool connected, bool *known,
                                   bool *cached) {
  if ((NULL == icon) || (NULL == known) || (NULL == cached)) {
    return;
  }

  if ((*known) && (*cached == connected)) {
    return;
  }

  set_icon_state(icon, connected);
  *known = true;
  *cached = connected;
}

static void update_label_if_changed(lv_obj_t *label, char *cache,
                                    const char *next_text) {
  if ((NULL == label) || (NULL == cache) || (NULL == next_text)) {
    return;
  }

  if (0 == strcmp(cache, next_text)) {
    return;
  }

  (void)snprintf(cache, COMP_STATUS_BAR_TEXT_CACHE_SIZE, "%s", next_text);
  lv_label_set_text(label, cache);
}

static void on_locale_toggle(lv_event_t *e) {
  (void)e;
  health_ui_locale_t cur = health_ui_i18n_get_locale();
  health_ui_locale_t next =
      (cur == HEALTH_UI_LOCALE_EN) ? HEALTH_UI_LOCALE_TH : HEALTH_UI_LOCALE_EN;
  health_ui_i18n_set_locale(next);
  lv_label_set_text(s_locale_label,
                    (next == HEALTH_UI_LOCALE_TH) ? "TH" : "EN");
  health_ui_root_refresh_strings();
}

void comp_system_status_bar_create(void) {
  lv_obj_t *top = lv_layer_top();
  comp_system_status_bar_state_t initial_state = {
      .wifi_valid = true,
      .wifi_connected = true,
      .bt_valid = true,
      .bt_connected = true,
      .time_text = COMP_STATUS_BAR_DEFAULT_TIME_TEXT,
      .date_text = COMP_STATUS_BAR_DEFAULT_DATE_TEXT,
  };
  if (NULL == top) {
    return;
  }

  reset_status_instances();

  s_status_shell = lv_obj_create(top);
  lv_obj_set_size(s_status_shell, lv_pct(COMP_STATUS_BAR_SHELL_WIDTH_PCT),
                  COMP_STATUS_BAR_HEIGHT_PX);
  lv_obj_set_style_bg_color(s_status_shell,
                            lv_color_hex(COMP_STATUS_BAR_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(s_status_shell, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(s_status_shell, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_status_shell, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_status_shell, 0, LV_PART_MAIN);
  lv_obj_clear_flag(s_status_shell, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(s_status_shell, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_align(s_status_shell, LV_ALIGN_TOP_MID, 0, COMP_STATUS_BAR_TOP_MARGIN_PX);

  s_status_root = lv_obj_create(s_status_shell);
  lv_obj_set_size(s_status_root, LV_SIZE_CONTENT, lv_pct(100));
  lv_obj_set_style_bg_opa(s_status_root, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_radius(s_status_root, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(s_status_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_status_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(s_status_root, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_layout(s_status_root, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(s_status_root, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(s_status_root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(s_status_root, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(s_status_root, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_align(s_status_root, LV_ALIGN_TOP_RIGHT, -COMP_STATUS_BAR_RIGHT_MARGIN_PX,
               0);

  /* Fixed slot widths prevent icon/date jitter when text values change. */
  s_wifi_icon = create_slot_label(s_status_root, LV_SYMBOL_WIFI,
                                  COMP_STATUS_BAR_SLOT_ICON_W_PX,
                                  LV_TEXT_ALIGN_CENTER, COMP_STATUS_BAR_ICON_FONT);
  s_bt_icon = create_slot_label(s_status_root, LV_SYMBOL_BLUETOOTH,
                                COMP_STATUS_BAR_SLOT_ICON_W_PX,
                                LV_TEXT_ALIGN_CENTER, COMP_STATUS_BAR_ICON_FONT);
  s_time_label = create_slot_label(s_status_root, "",
                                   COMP_STATUS_BAR_SLOT_TIME_W_PX,
                                   LV_TEXT_ALIGN_RIGHT, COMP_STATUS_BAR_TEXT_FONT);
  s_date_label = create_slot_label(s_status_root, "",
                                   COMP_STATUS_BAR_SLOT_DATE_W_PX,
                                   LV_TEXT_ALIGN_RIGHT, COMP_STATUS_BAR_TEXT_FONT);
  lv_obj_set_style_pad_right(s_date_label, COMP_STATUS_BAR_DATE_TEXT_RIGHT_INSET_PX,
                             LV_PART_MAIN);

  /* TH/EN locale toggle — minimal pill button on the left side of the bar */
  s_locale_btn = lv_btn_create(s_status_shell);
  lv_obj_set_size(s_locale_btn, 36, 22);
  lv_obj_set_style_radius(s_locale_btn, 11, LV_PART_MAIN);
  lv_obj_set_style_bg_color(s_locale_btn, lv_color_hex(0x667085), LV_PART_MAIN);
  lv_obj_set_style_pad_all(s_locale_btn, 0, LV_PART_MAIN);
  lv_obj_align(s_locale_btn, LV_ALIGN_LEFT_MID, 4, 0);

  s_locale_label = lv_label_create(s_locale_btn);
  lv_label_set_text(s_locale_label,
                    (health_ui_i18n_get_locale() == HEALTH_UI_LOCALE_TH) ? "TH"
                                                                         : "EN");
  lv_obj_set_style_text_color(s_locale_label, lv_color_hex(0xFFFFFF),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(s_locale_label, &lv_font_montserrat_12,
                             LV_PART_MAIN);
  lv_obj_center(s_locale_label);

  lv_obj_add_event_cb(s_locale_btn, on_locale_toggle, LV_EVENT_CLICKED, NULL);

  /* Reset cache so first update always paints all fields. */
  s_wifi_known = false;
  s_bt_known = false;
  s_time_cache[0] = '\0';
  s_date_cache[0] = '\0';
  comp_system_status_bar_update(&initial_state);
}

void comp_system_status_bar_set_alive(unsigned long alive_sec) {
  /* Reserved for legacy call sites until IPC route is wired. */
  (void)alive_sec;
}

void comp_system_status_bar_set_mock(const char *time_text,
                                     const char *date_text) {
  comp_system_status_bar_state_t state = {0};
  state.time_text = time_text;
  state.date_text = date_text;
  comp_system_status_bar_update(&state);
}

void comp_system_status_bar_set_connectivity(bool wifi_connected,
                                             bool bt_connected) {
  comp_system_status_bar_state_t state = {0};
  state.wifi_valid = true;
  state.wifi_connected = wifi_connected;
  state.bt_valid = true;
  state.bt_connected = bt_connected;
  comp_system_status_bar_update(&state);
}

void comp_system_status_bar_update(const comp_system_status_bar_state_t *state) {
  if (NULL == state) {
    return;
  }

  if (state->wifi_valid) {
    update_icon_if_changed(s_wifi_icon, state->wifi_connected, &s_wifi_known,
                           &s_wifi_cached);
  }

  if (state->bt_valid) {
    update_icon_if_changed(s_bt_icon, state->bt_connected, &s_bt_known,
                           &s_bt_cached);
  }

  update_label_if_changed(s_time_label, s_time_cache, state->time_text);
  update_label_if_changed(s_date_label, s_date_cache, state->date_text);
}
