#ifndef HEALTH_UI_SIM_DATA_H
#define HEALTH_UI_SIM_DATA_H

#include "../components/content/comp_device_card.h"
#include "../components/content/comp_user_card.h"
#include "../health_ui_root.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  health_ui_system_status_t system_status;
  uint32_t sequence;
  uint32_t scenario;
} health_ui_sim_snapshot_t;

typedef struct {
  const char *instruction_text;
  const char *cta_text;
  const char *visual_text;
  uint32_t visual_bg_color_hex;
} health_ui_pairing_step_data_t;

#ifndef HEALTH_UI_BP_DETAIL_MAX_POINTS
#define HEALTH_UI_BP_DETAIL_MAX_POINTS (7U)
#endif

#ifndef HEALTH_UI_SINGLE_METRIC_MAX_POINTS
#define HEALTH_UI_SINGLE_METRIC_MAX_POINTS (7U)
#endif

#ifndef HEALTH_UI_SINGLE_METRIC_MAX_Y_LABELS
#define HEALTH_UI_SINGLE_METRIC_MAX_Y_LABELS (8U)
#endif

#ifndef HEALTH_UI_SLEEP_DETAIL_MAX_DAYS
#define HEALTH_UI_SLEEP_DETAIL_MAX_DAYS (31U)
#endif

#ifndef HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT
#define HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT (6U)
#endif

typedef struct {
  const char *time_text;
  const char *sys_value_text;
  const char *sys_unit_text;
  const char *dia_value_text;
  const char *dia_unit_text;
  const char *pulse_value_text;
  const char *pulse_unit_text;
  const char *status_text;
  comp_user_status_level_t status_level;
} health_ui_user_detail_bp_summary_t;

typedef struct {
  const char *title_text;
  const char *time_text;
  const char *value_text;
  const char *unit_text;
  const char *status_text;
  comp_user_status_level_t status_level;
  const char *previous_time_text;
  const char *previous_value_text;
  const char *previous_unit_text;
  bool previous_available;
  bool highlighted;
} health_ui_user_detail_metric_card_t;

typedef struct {
  health_ui_user_detail_bp_summary_t bp;
  health_ui_user_detail_metric_card_t glucose;
  health_ui_user_detail_metric_card_t body_temp;
  health_ui_user_detail_metric_card_t spo2;
  health_ui_user_detail_metric_card_t weight;
  health_ui_user_detail_metric_card_t sleep_duration;
} health_ui_user_detail_data_t;

typedef struct {
  const char *period_text;
  uint32_t point_count;
  const char *x_labels[HEALTH_UI_BP_DETAIL_MAX_POINTS];
  int16_t sys_values[HEALTH_UI_BP_DETAIL_MAX_POINTS];
  int16_t dia_values[HEALTH_UI_BP_DETAIL_MAX_POINTS];
  int16_t pulse_values[HEALTH_UI_BP_DETAIL_MAX_POINTS];
  health_ui_user_detail_bp_summary_t summary;
} health_ui_bp_metric_detail_data_t;

typedef struct {
  const char *title_text;
  const char *period_text;
  const char *summary_label_text;
  const char *summary_unit_text;
  const char *summary_value_text;
  const char *summary_status_text;
  comp_user_status_level_t summary_status_level;
  uint32_t accent_color_hex;
  uint32_t point_count;
  const char *x_labels[HEALTH_UI_SINGLE_METRIC_MAX_POINTS];
  int16_t values[HEALTH_UI_SINGLE_METRIC_MAX_POINTS];
  int16_t y_min;
  int16_t y_max;
  const char *y_axis_title_text;
  uint32_t y_label_count;
  const char *y_labels[HEALTH_UI_SINGLE_METRIC_MAX_Y_LABELS];
  uint8_t normal_band_enabled;
  int16_t normal_band_min;
  int16_t normal_band_max;
  uint32_t normal_band_color_hex;
} health_ui_single_metric_detail_data_t;

typedef struct {
  const char *period_text;
  uint32_t avg_time_in_bed_hour;
  uint32_t avg_time_in_bed_minute;
  uint32_t avg_time_asleep_hour;
  uint32_t avg_time_asleep_minute;
  uint32_t day_count;
  uint8_t bar_top_pct[HEALTH_UI_SLEEP_DETAIL_MAX_DAYS];
  uint8_t bar_height_pct[HEALTH_UI_SLEEP_DETAIL_MAX_DAYS];
  uint32_t avg_awake_minute;
  uint32_t avg_rem_hour;
  uint32_t avg_rem_minute;
  uint32_t avg_core_hour;
  uint32_t avg_core_minute;
  uint32_t avg_deep_minute;
  const char *x_tick_labels[HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT];
} health_ui_sleep_metric_detail_data_t;

void health_ui_sim_data_init(void);
void health_ui_sim_data_build_snapshot(unsigned long alive_sec,
                                       health_ui_sim_snapshot_t *out_snapshot);
void health_ui_sim_data_build_system_status(
    unsigned long alive_sec, health_ui_system_status_t *out_status);
/* Fills dashboard/user-detail profile fields (name/age/avatar) and metric preview. */
void health_ui_sim_data_build_member_card(uint32_t member_index,
                                          comp_user_card_data_t *out_card_data);
/* Returns current simulated device inventory count (source of truth for Devices: N). */
uint32_t health_ui_sim_data_get_device_count(void);
/* Fills one dashboard device-card preview from simulated inventory by index. */
void health_ui_sim_data_build_device_card(
    uint32_t device_index, comp_device_card_data_t *out_card_data);
/* Returns stable simulated device id for selected pairable-device index. */
uint32_t health_ui_sim_data_get_pairable_device_id(uint32_t device_index);
/* Returns number of pairing steps for selected pairable-device index. */
uint32_t health_ui_sim_data_get_pairing_step_count(uint32_t device_index);
/* Fills one pairing-step snapshot for selected pairable-device index and step index. */
bool health_ui_sim_data_build_pairing_step(uint32_t device_index, uint32_t step_index,
                                           health_ui_pairing_step_data_t *out_step_data);
/* Returns pairing guide text for selected device index. */
const char *health_ui_sim_data_get_device_pairing_guide(uint32_t device_index);
/* Returns deterministic pairing result for layout/sim flow testing. */
bool health_ui_sim_data_get_device_pairing_result(uint32_t device_index);
/* Builds user-detail page snapshot for selected member index. */
void health_ui_sim_data_build_user_detail(uint32_t member_index,
                                          health_ui_user_detail_data_t *out_data);
/* Builds blood-pressure metric detail snapshot used by BP detail page layout. */
void health_ui_sim_data_build_bp_metric_detail(
    uint32_t member_index, health_ui_bp_metric_detail_data_t *out_data);
/* Builds single-sensor metric detail for glucose/temp/spo2/weight pages. */
void health_ui_sim_data_build_single_metric_detail(
    uint32_t member_index, health_ui_metric_detail_t metric,
    health_ui_single_metric_detail_data_t *out_data);
/* Builds sleep metric detail snapshot used by Sleep detail page layout. */
void health_ui_sim_data_build_sleep_metric_detail(
    uint32_t member_index, health_ui_sleep_metric_detail_data_t *out_data);

#endif
