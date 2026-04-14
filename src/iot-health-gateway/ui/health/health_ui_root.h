#ifndef HEALTH_UI_ROOT_H
#define HEALTH_UI_ROOT_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  HEALTH_UI_PAGE_PRE_AUTH = 0,
  HEALTH_UI_PAGE_HEALTH = 1,
  HEALTH_UI_PAGE_HOME = 2,
  HEALTH_UI_PAGE_SETTING = 3,
  HEALTH_UI_PAGE_USER_DETAIL = 4,
  HEALTH_UI_PAGE_METRIC_BP_DETAIL = 5,
  HEALTH_UI_PAGE_METRIC_GLUCOSE_DETAIL = 6,
  HEALTH_UI_PAGE_METRIC_BODY_TEMP_DETAIL = 7,
  HEALTH_UI_PAGE_METRIC_SPO2_DETAIL = 8,
  HEALTH_UI_PAGE_METRIC_WEIGHT_DETAIL = 9,
  HEALTH_UI_PAGE_METRIC_SLEEP_DETAIL = 10,
  HEALTH_UI_PAGE_COUNT
} health_ui_page_t;

typedef enum {
  HEALTH_UI_METRIC_DETAIL_BP = 0,
  HEALTH_UI_METRIC_DETAIL_GLUCOSE = 1,
  HEALTH_UI_METRIC_DETAIL_BODY_TEMP = 2,
  HEALTH_UI_METRIC_DETAIL_SPO2 = 3,
  HEALTH_UI_METRIC_DETAIL_WEIGHT = 4,
  HEALTH_UI_METRIC_DETAIL_SLEEP = 5,
  HEALTH_UI_METRIC_DETAIL_COUNT
} health_ui_metric_detail_t;

typedef struct {
  bool wifi_valid;
  bool wifi_connected;
  bool ble_valid;
  bool ble_connected;
  const char *time_text;
  const char *date_text;
} health_ui_system_status_t;

void health_ui_root_init(void);
void health_ui_root_poll(void);
void health_ui_root_on_alive_tick(unsigned long alive_sec);
void health_ui_root_set_active_page(health_ui_page_t page, bool animate);
void health_ui_root_set_household_summary(const char *family_name,
                                          uint32_t member_count);
void health_ui_root_set_selected_member_index(uint32_t member_index);
void health_ui_root_set_selected_user(const char *user_name);
void health_ui_root_set_selected_user_profile(const char *user_name,
                                              uint32_t age_years,
                                              uint32_t avatar_bg_color_hex);
void health_ui_root_update_system_status(
    const health_ui_system_status_t *status);
void health_ui_root_open_metric_detail(health_ui_metric_detail_t metric);

#endif
