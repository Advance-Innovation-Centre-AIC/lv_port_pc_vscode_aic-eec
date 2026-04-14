#include "health_ui_root.h"

#include "layout/layout_i18n.h"
#include "components/global/comp_overlay_manager.h"
#include "components/global/comp_system_status_bar.h"
#include "components/header/comp_tab_menu.h"
#include "components/overlay/comp_add_device_flow_modal.h"
#include "health_ui_display_policy.h"
#include "metric_detail_catalog.h"
#include "layout/layout_scaffold.h"
#include "pages/dashboard/page_dashboard_main.h"
#include "pages/dashboard/page_home_main.h"
#include "pages/dashboard/page_metric_detail_layout.h"
#include "pages/dashboard/page_settings_layout.h"
#include "pages/dashboard/page_user_detail_layout.h"
#include "pages/pre_auth/page_pre_auth_overview.h"
#include "sim/health_ui_sim_data.h"
#if defined(ENABLE_BLE_DATA_BRIDGE) && (ENABLE_BLE_DATA_BRIDGE != 0)
#include "health_data_bridge.h"
#endif

#include "lvgl.h"

#include <stdio.h>

static health_layout_scaffold_t s_scaffold;
#define HEALTH_UI_SELECTED_USER_BUF_SIZE (64U)
#define HEALTH_UI_DEBUG_START_TAB_IDX ((uint32_t)HEALTH_UI_PAGE_HEALTH)
/* Layout-first mode: keep UI testable without IPC by feeding deterministic sim data. */
#define HEALTH_UI_ENABLE_SIM_DATA (1U)
/* Disable overlay debug card in normal layout review. */
#define HEALTH_UI_ENABLE_OVERLAY_DEBUG (0U)
static char s_selected_user_name[HEALTH_UI_SELECTED_USER_BUF_SIZE] = "Member";
static uint32_t s_selected_member_index = 0U;
static uint32_t s_selected_user_age_years = 0U;
static uint32_t s_selected_user_avatar_bg_color_hex =
    COMP_TAB_MENU_AVATAR_BG_COLOR_HEX;

static void sync_selected_user_views(void) {
  page_user_detail_layout_set_member_index(s_selected_member_index);
  page_metric_detail_layout_set_member_index(s_selected_member_index);
  page_user_detail_layout_set_user_name(s_selected_user_name);
  comp_tab_menu_set_user_detail_profile(
      s_selected_user_name, s_selected_user_age_years,
      s_selected_user_avatar_bg_color_hex);
}

void health_ui_root_init(void) {
  uint32_t metric_idx;
  lv_obj_t *pre_auth_tab;
  lv_obj_t *health_tab;
  lv_obj_t *home_tab;
  lv_obj_t *setting_tab;
  lv_obj_t *user_tab;
  lv_obj_t *metric_detail_tabs[HEALTH_UI_METRIC_DETAIL_COUNT] = {0};

  health_ui_i18n_init(HEALTH_UI_LOCALE_EN);
  health_ui_metric_detail_catalog_apply_i18n();
  health_layout_scaffold_create(&s_scaffold);
  comp_system_status_bar_create();

  pre_auth_tab = health_layout_scaffold_add_tab(&s_scaffold, "Pre-Auth");
  health_tab = health_layout_scaffold_add_tab(&s_scaffold, "Health");
  home_tab = health_layout_scaffold_add_tab(&s_scaffold, "Home");
  setting_tab = health_layout_scaffold_add_tab(&s_scaffold, "Setting");
  user_tab = health_layout_scaffold_add_tab(&s_scaffold, "User Detail");
  for (metric_idx = 0U; metric_idx < (uint32_t)HEALTH_UI_METRIC_DETAIL_COUNT;
       metric_idx++) {
    health_ui_metric_detail_t metric = (health_ui_metric_detail_t)metric_idx;
    metric_detail_tabs[metric_idx] = health_layout_scaffold_add_tab(
        &s_scaffold, health_ui_metric_detail_get_tab_title(metric));
  }

  page_pre_auth_overview_build(pre_auth_tab);
  page_dashboard_main_build(health_tab);
  page_home_main_build(home_tab);
  page_settings_layout_build(setting_tab);
  page_user_detail_layout_build(user_tab);
  for (metric_idx = 0U; metric_idx < (uint32_t)HEALTH_UI_METRIC_DETAIL_COUNT;
       metric_idx++) {
    health_ui_metric_detail_t metric = (health_ui_metric_detail_t)metric_idx;
    page_metric_detail_layout_build(metric_detail_tabs[metric_idx], metric);
  }
  sync_selected_user_views();

#if (HEALTH_UI_ENABLE_OVERLAY_DEBUG != 0U)
  comp_overlay_manager_create(s_scaffold.screen);
#endif
  lv_tabview_set_active(s_scaffold.tabview, HEALTH_UI_DEBUG_START_TAB_IDX,
                        LV_ANIM_OFF);
#if (HEALTH_UI_ENABLE_SIM_DATA != 0U)
  health_ui_sim_data_init();
#endif
#if defined(ENABLE_BLE_DATA_BRIDGE) && (ENABLE_BLE_DATA_BRIDGE != 0)
  health_data_bridge_init();
#endif
  health_ui_root_set_household_summary(_(STR_FAMILY_NAME), 10U);
  lv_scr_load(s_scaffold.screen);
}

void health_ui_root_poll(void) {}

void health_ui_root_on_alive_tick(unsigned long alive_sec) {
#if (HEALTH_UI_ENABLE_SIM_DATA != 0U)
  health_ui_system_status_t status = {0};
  /* Keep dashboard status-bar redraw paused while modal is on top to avoid
   * intermittent text artifacts observed on this display pipeline. */
  if (comp_add_device_flow_modal_is_open()) {
    return;
  }
  health_ui_sim_data_build_system_status(alive_sec, &status);
  health_ui_root_update_system_status(&status);
#else
  comp_system_status_bar_set_alive(alive_sec);
#endif
}

void health_ui_root_set_active_page(health_ui_page_t page, bool animate) {
  if ((NULL == s_scaffold.tabview) ||
      ((uint32_t)page >= (uint32_t)HEALTH_UI_PAGE_COUNT)) {
    return;
  }

  lv_tabview_set_active(s_scaffold.tabview, (uint32_t)page,
                        animate ? LV_ANIM_ON : LV_ANIM_OFF);
}

void health_ui_root_set_household_summary(const char *family_name,
                                          uint32_t member_count) {
  comp_tab_menu_set_family_summary(family_name, member_count);
  page_dashboard_main_set_member_count(member_count);
}

void health_ui_root_set_selected_member_index(uint32_t member_index) {
  s_selected_member_index = member_index;
  page_user_detail_layout_set_member_index(s_selected_member_index);
}

void health_ui_root_set_selected_user(const char *user_name) {
  health_ui_root_set_selected_user_profile(user_name, s_selected_user_age_years,
                                           s_selected_user_avatar_bg_color_hex);
}

void health_ui_root_set_selected_user_profile(const char *user_name,
                                              uint32_t age_years,
                                              uint32_t avatar_bg_color_hex) {
  const char *name = "Member";

  if ((NULL != user_name) && (user_name[0] != '\0')) {
    name = user_name;
  }

  (void)snprintf(s_selected_user_name, sizeof(s_selected_user_name), "%s", name);
  s_selected_user_age_years = age_years;
  s_selected_user_avatar_bg_color_hex =
      (0U != avatar_bg_color_hex) ? avatar_bg_color_hex
                                  : COMP_TAB_MENU_AVATAR_BG_COLOR_HEX;

  sync_selected_user_views();
}

void health_ui_root_update_system_status(
    const health_ui_system_status_t *status) {
  comp_system_status_bar_state_t bar_state = {0};

  if (NULL == status) {
    return;
  }

  /* External API uses BLE naming; status-bar component uses BT naming. */
  bar_state.wifi_valid = status->wifi_valid;
  bar_state.wifi_connected = status->wifi_connected;
  bar_state.bt_valid = status->ble_valid;
  bar_state.bt_connected = status->ble_connected;
  bar_state.time_text = status->time_text;
  bar_state.date_text = status->date_text;
  comp_system_status_bar_update(&bar_state);
}

void health_ui_root_refresh_strings(void) {
  health_ui_metric_detail_catalog_apply_i18n();
  health_ui_root_set_household_summary(_(STR_FAMILY_NAME), 10U);
}

void health_ui_root_open_metric_detail(health_ui_metric_detail_t metric) {
  health_ui_root_set_active_page(health_ui_metric_detail_to_page(metric), false);
}
