#include "health_ui_sim_data.h"

#include <stdbool.h>
#include <stdio.h>

#define SIM_BASE_HOUR (9U)
#define SIM_BASE_MINUTE (41U)
#define SIM_BASE_DAY_OF_WEEK_INDEX (2U) /* Tue */
#define SIM_BASE_DAY_OF_MONTH (1U)
#define SIM_TIME_STEP_SEC (5U)
#define SIM_ARRAY_SIZE(arr) ((uint32_t)(sizeof(arr) / sizeof((arr)[0])))

static const char *s_member_names[] = {
    "Zac",   "Zoey", "Monet", "Maggie", "Liam",  "Emma",
    "Noah",  "Ava",  "Mia",   "Ethan",  "Olivia","Lucas",
};

static const uint32_t s_member_ages[] = {
    31U, 24U, 26U, 22U, 29U, 27U, 33U, 21U, 25U, 30U, 28U, 32U,
};

static const uint32_t s_member_avatar_bg_colors[] = {
    0xD6DCE4, 0xE8DDBF, 0xCDE4D1, 0xE7DFC9, 0xD7E7F3, 0xF2DAD7,
    0xD7E9DB, 0xE2DBF4, 0xD5E5F5, 0xF0DFCD, 0xD4E8DE, 0xE7DBF1,
};

typedef struct {
  const char *instruction_text;
  const char *cta_text;
  const char *visual_text;
  uint32_t visual_bg_color_hex;
} sim_pair_step_entry_t;

typedef struct {
  uint32_t device_id;
  const char *name;
  const char *type_text;
  const char *avatar_text;
  uint32_t avatar_bg_color_hex;
  const sim_pair_step_entry_t *pairing_steps;
  uint32_t pairing_step_count;
  const char *pairing_guide_text;
  bool pairing_success;
} sim_device_entry_t;

static const sim_pair_step_entry_t s_pair_steps_omron[] = {
    {.instruction_text = "Press START on the monitor to wake device.",
     .cta_text = "Continue",
     .visual_text = "BP",
     .visual_bg_color_hex = 0xE7EAF0},
    {.instruction_text = "Keep monitor near gateway while searching.",
     .cta_text = "Continue",
     .visual_text = "BP",
     .visual_bg_color_hex = 0xE7EAF0},
    {.instruction_text = "Pairing check completed. Proceed to final validation.",
     .cta_text = "Continue",
     .visual_text = "BP",
     .visual_bg_color_hex = 0xE7EAF0},
};

static const sim_pair_step_entry_t s_pair_steps_accuchek[] = {
    {.instruction_text = "Turn on meter and open BLE pairing mode.",
     .cta_text = "Continue",
     .visual_text = "BG",
     .visual_bg_color_hex = 0xECE8F8},
    {.instruction_text = "Confirm pairing code on device screen.",
     .cta_text = "Continue",
     .visual_text = "BG",
     .visual_bg_color_hex = 0xECE8F8},
    {.instruction_text = "Wait for secure channel establishment.",
     .cta_text = "Continue",
     .visual_text = "BG",
     .visual_bg_color_hex = 0xECE8F8},
    {.instruction_text = "Connection profile prepared for sync.",
     .cta_text = "Continue",
     .visual_text = "BG",
     .visual_bg_color_hex = 0xECE8F8},
};

static const sim_pair_step_entry_t s_pair_steps_taidoc[] = {
    {.instruction_text = "Power on thermometer and hold for 3 seconds.",
     .cta_text = "Continue",
     .visual_text = "TH",
     .visual_bg_color_hex = 0xE3EEF9},
    {.instruction_text = "Keep thermometer within 1 meter from gateway.",
     .cta_text = "Continue",
     .visual_text = "TH",
     .visual_bg_color_hex = 0xE3EEF9},
    {.instruction_text = "Device channel ready for temperature upload.",
     .cta_text = "Continue",
     .visual_text = "TH",
     .visual_bg_color_hex = 0xE3EEF9},
};

static const sim_pair_step_entry_t s_pair_steps_fora[] = {
    {.instruction_text = "Turn on oximeter and keep finger inserted.",
     .cta_text = "Continue",
     .visual_text = "SP",
     .visual_bg_color_hex = 0xF1E8EE},
    {.instruction_text = "Wait while gateway reads advertising packet.",
     .cta_text = "Continue",
     .visual_text = "SP",
     .visual_bg_color_hex = 0xF1E8EE},
    {.instruction_text = "Pairing handshake complete.",
     .cta_text = "Continue",
     .visual_text = "SP",
     .visual_bg_color_hex = 0xF1E8EE},
};

static const sim_pair_step_entry_t s_pair_steps_xiaomi[] = {
    {.instruction_text = "Stand on the device to turn it on.",
     .cta_text = "Continue",
     .visual_text = "WT",
     .visual_bg_color_hex = 0xE8ECEF},
    {.instruction_text = "Stay still while BLE service is discovered.",
     .cta_text = "Continue",
     .visual_text = "WT",
     .visual_bg_color_hex = 0xE8ECEF},
    {.instruction_text = "Allow gateway to read profile characteristics.",
     .cta_text = "Continue",
     .visual_text = "WT",
     .visual_bg_color_hex = 0xE8ECEF},
    {.instruction_text = "Confirm scale assignment to this household.",
     .cta_text = "Continue",
     .visual_text = "WT",
     .visual_bg_color_hex = 0xE8ECEF},
    {.instruction_text = "Device setup complete. Ready for measurement sync.",
     .cta_text = "Continue",
     .visual_text = "WT",
     .visual_bg_color_hex = 0xE8ECEF},
};

/* Single source of truth for simulated devices shown on dashboard. */
static const sim_device_entry_t s_device_entries[] = {
    {.device_id = 1001U,
     .name = "Omron HEM-7130",
     .type_text = "Blood Pressure Monitor",
     .avatar_text = "BP",
     .avatar_bg_color_hex = 0xE7EAF0,
     .pairing_steps = s_pair_steps_omron,
     .pairing_step_count = SIM_ARRAY_SIZE(s_pair_steps_omron),
     .pairing_guide_text = "Please press START on the monitor and keep it near gateway.",
     .pairing_success = true},
    {.device_id = 1002U,
     .name = "Accu-Chek Instant",
     .type_text = "Blood Glucose Meter",
     .avatar_text = "BG",
     .avatar_bg_color_hex = 0xECE8F8,
     .pairing_steps = s_pair_steps_accuchek,
     .pairing_step_count = SIM_ARRAY_SIZE(s_pair_steps_accuchek),
     .pairing_guide_text = "Turn on the glucose meter and wait for Bluetooth icon.",
     .pairing_success = false},
    {.device_id = 1003U,
     .name = "TaiDoc TD-1241",
     .type_text = "Thermometer",
     .avatar_text = "TH",
     .avatar_bg_color_hex = 0xE3EEF9,
     .pairing_steps = s_pair_steps_taidoc,
     .pairing_step_count = SIM_ARRAY_SIZE(s_pair_steps_taidoc),
     .pairing_guide_text = "Power on the thermometer and hold for 3 seconds.",
     .pairing_success = true},
    {.device_id = 1004U,
     .name = "FORA TD-8255B",
     .type_text = "Pulse Oximeter",
     .avatar_text = "SP",
     .avatar_bg_color_hex = 0xF1E8EE,
     .pairing_steps = s_pair_steps_fora,
     .pairing_step_count = SIM_ARRAY_SIZE(s_pair_steps_fora),
     .pairing_guide_text = "Turn on the oximeter and keep finger inserted while pairing.",
     .pairing_success = false},
    {.device_id = 1005U,
     .name = "Xiaomi | Mi Smart Scale 2",
     .type_text = "Weight Scale",
     .avatar_text = "WT",
     .avatar_bg_color_hex = 0xE8ECEF,
     .pairing_steps = s_pair_steps_xiaomi,
     .pairing_step_count = SIM_ARRAY_SIZE(s_pair_steps_xiaomi),
     .pairing_guide_text = "Stand on the device to turn it on and wait for it to connect.",
     .pairing_success = true},
};

static const comp_user_condition_t s_conditions_low_prediabetes[] = {
    COMP_USER_CONDITION_LOW_FEVER,
    COMP_USER_CONDITION_PREDIABETES,
};

static const comp_user_condition_t s_conditions_high_prediabetes[] = {
    COMP_USER_CONDITION_HIGH_FEVER,
    COMP_USER_CONDITION_PREDIABETES,
};

static const comp_user_condition_t s_conditions_prediabetes_only[] = {
    COMP_USER_CONDITION_PREDIABETES,
};

static const comp_user_condition_t s_conditions_low_fever_only[] = {
    COMP_USER_CONDITION_LOW_FEVER,
};

static const comp_user_condition_t s_conditions_three_tags[] = {
    COMP_USER_CONDITION_LOW_FEVER,
    COMP_USER_CONDITION_PREDIABETES,
    COMP_USER_CONDITION_HIGH_FEVER,
};

static const comp_user_metric_preview_t s_metric_bp_normal = {
    .type = COMP_USER_METRIC_BLOOD_PRESSURE,
    .value_text = "120/80",
    .unit_text = "mmHg",
    .status_text = "Normal",
    .time_text = "08:49",
    .status_level = COMP_USER_STATUS_NORMAL,
};

static const comp_user_metric_preview_t s_metric_temp_low = {
    .type = COMP_USER_METRIC_BODY_TEMP,
    .value_text = "37.8",
    .unit_text = "°C",
    .status_text = "Low fever",
    .time_text = "08:49",
    .status_level = COMP_USER_STATUS_WARNING,
};

static const comp_user_metric_preview_t s_metric_temp_high = {
    .type = COMP_USER_METRIC_BODY_TEMP,
    .value_text = "38.5",
    .unit_text = "°C",
    .status_text = "High fever",
    .time_text = "08:49",
    .status_level = COMP_USER_STATUS_CRITICAL,
};

static const comp_user_metric_preview_t s_metric_glucose_warning = {
    .type = COMP_USER_METRIC_BLOOD_GLUCOSE,
    .value_text = "110",
    .unit_text = "mg/dL",
    .status_text = "Prediabetes",
    .time_text = "08:49",
    .status_level = COMP_USER_STATUS_WARNING,
};

static const health_ui_user_detail_bp_summary_t s_detail_bp_normal = {
    .time_text = "08:49",
    .sys_value_text = "120",
    .sys_unit_text = "mmHg",
    .dia_value_text = "80",
    .dia_unit_text = "mmHg",
    .pulse_value_text = "72",
    .pulse_unit_text = "BPM",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
};

static const health_ui_user_detail_metric_card_t s_detail_glucose_warning = {
    .title_text = "Blood Glucose",
    .time_text = "08:49",
    .value_text = "110",
    .unit_text = "mg/dL",
    .status_text = "Prediabetes",
    .status_level = COMP_USER_STATUS_WARNING,
    .previous_time_text = NULL,
    .previous_value_text = NULL,
    .previous_unit_text = NULL,
    .previous_available = false,
    .highlighted = true,
};

static const health_ui_user_detail_metric_card_t s_detail_glucose_normal = {
    .title_text = "Blood Glucose",
    .time_text = "08:49",
    .value_text = "95",
    .unit_text = "mg/dL",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
    .previous_time_text = "Jun 20,2025 15:15",
    .previous_value_text = "92",
    .previous_unit_text = "mg/dL",
    .previous_available = true,
    .highlighted = false,
};

static const health_ui_user_detail_metric_card_t s_detail_temp_high = {
    .title_text = "Body Temp",
    .time_text = "08:49",
    .value_text = "38.5",
    .unit_text = "°C",
    .status_text = "High fever",
    .status_level = COMP_USER_STATUS_CRITICAL,
    .previous_time_text = "Jun 20,2025 15:15",
    .previous_value_text = "36.5",
    .previous_unit_text = "°C",
    .previous_available = true,
    .highlighted = true,
};

static const health_ui_user_detail_metric_card_t s_detail_temp_low = {
    .title_text = "Body Temp",
    .time_text = "08:49",
    .value_text = "37.8",
    .unit_text = "°C",
    .status_text = "Low fever",
    .status_level = COMP_USER_STATUS_WARNING,
    .previous_time_text = "Jun 20,2025 15:15",
    .previous_value_text = "36.9",
    .previous_unit_text = "°C",
    .previous_available = true,
    .highlighted = false,
};

static const health_ui_user_detail_metric_card_t s_detail_temp_normal = {
    .title_text = "Body Temp",
    .time_text = "08:49",
    .value_text = "36.8",
    .unit_text = "°C",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
    .previous_time_text = "Jun 20,2025 15:15",
    .previous_value_text = "36.7",
    .previous_unit_text = "°C",
    .previous_available = true,
    .highlighted = false,
};

static const health_ui_user_detail_metric_card_t s_detail_spo2_normal = {
    .title_text = "SpO2",
    .time_text = "08:49",
    .value_text = "99",
    .unit_text = "%",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
    .previous_time_text = "Jun 20,2025 15:15",
    .previous_value_text = "98",
    .previous_unit_text = "%",
    .previous_available = true,
    .highlighted = false,
};

static const health_ui_user_detail_metric_card_t s_detail_weight_normal = {
    .title_text = "Weight",
    .time_text = "08:49",
    .value_text = "68.1",
    .unit_text = "kg",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
    .previous_time_text = NULL,
    .previous_value_text = NULL,
    .previous_unit_text = NULL,
    .previous_available = false,
    .highlighted = false,
};

static const health_ui_user_detail_metric_card_t s_detail_sleep_normal = {
    .title_text = "Sleep Duration",
    .time_text = "08:49",
    .value_text = "9.45",
    .unit_text = "hr",
    .status_text = "Normal",
    .status_level = COMP_USER_STATUS_NORMAL,
    .previous_time_text = NULL,
    .previous_value_text = NULL,
    .previous_unit_text = NULL,
    .previous_available = false,
    .highlighted = false,
};

typedef struct {
  const comp_user_condition_t *conditions;
  uint32_t condition_count;
  const comp_user_metric_preview_t *dashboard_metric;
  const health_ui_user_detail_metric_card_t *glucose_detail;
  const health_ui_user_detail_metric_card_t *temp_detail;
} sim_member_style_profile_t;

static const sim_member_style_profile_t s_member_style_profiles[] = {
    {.conditions = NULL,
     .condition_count = 0U,
     .dashboard_metric = &s_metric_bp_normal,
     .glucose_detail = &s_detail_glucose_normal,
     .temp_detail = &s_detail_temp_normal},
    {.conditions = s_conditions_low_prediabetes,
     .condition_count = SIM_ARRAY_SIZE(s_conditions_low_prediabetes),
     .dashboard_metric = &s_metric_temp_low,
     .glucose_detail = &s_detail_glucose_warning,
     .temp_detail = &s_detail_temp_low},
    {.conditions = s_conditions_high_prediabetes,
     .condition_count = SIM_ARRAY_SIZE(s_conditions_high_prediabetes),
     .dashboard_metric = &s_metric_temp_high,
     .glucose_detail = &s_detail_glucose_warning,
     .temp_detail = &s_detail_temp_high},
    {.conditions = s_conditions_prediabetes_only,
     .condition_count = SIM_ARRAY_SIZE(s_conditions_prediabetes_only),
     .dashboard_metric = &s_metric_glucose_warning,
     .glucose_detail = &s_detail_glucose_warning,
     .temp_detail = &s_detail_temp_normal},
    {.conditions = s_conditions_low_fever_only,
     .condition_count = SIM_ARRAY_SIZE(s_conditions_low_fever_only),
     .dashboard_metric = &s_metric_temp_low,
     .glucose_detail = &s_detail_glucose_normal,
     .temp_detail = &s_detail_temp_low},
    /* Layout stress profile to keep "+N more" chip test path available. */
    {.conditions = s_conditions_three_tags,
     .condition_count = SIM_ARRAY_SIZE(s_conditions_three_tags),
     .dashboard_metric = &s_metric_glucose_warning,
     .glucose_detail = &s_detail_glucose_warning,
     .temp_detail = &s_detail_temp_high},
};

static const char *s_bp_detail_x_labels[HEALTH_UI_BP_DETAIL_MAX_POINTS] = {
    "July 1", "July 2", "July 3", "July 4", "July 5", "July 6", "July 7",
};

static const int16_t s_bp_detail_sys_values[HEALTH_UI_BP_DETAIL_MAX_POINTS] = {
    128, 131, 126, 134, 129, 124, 129,
};

static const int16_t s_bp_detail_dia_values[HEALTH_UI_BP_DETAIL_MAX_POINTS] = {
    83, 85, 81, 88, 84, 80, 83,
};

static const int16_t s_bp_detail_pulse_values[HEALTH_UI_BP_DETAIL_MAX_POINTS] = {
    70, 72, 69, 75, 73, 68, 71,
};

static const int16_t s_metric_glucose_values_normal[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    94, 99, 92, 103, 98, 90, 95,
};

static const int16_t s_metric_glucose_values_warning[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    96, 104, 92, 110, 101, 88, 110,
};

/* Temperature and weight are scaled x10 to preserve one decimal in chart values. */
static const int16_t s_metric_temp_values_normal[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    366, 368, 365, 371, 369, 364, 368,
};

static const int16_t s_metric_temp_values_low[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    366, 368, 365, 371, 369, 364, 378,
};

static const int16_t s_metric_temp_values_high[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    366, 368, 365, 371, 369, 364, 385,
};

static const int16_t s_metric_spo2_values[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    98, 97, 99, 96, 98, 97, 99,
};

static const int16_t s_metric_weight_values[HEALTH_UI_SINGLE_METRIC_MAX_POINTS] = {
    685, 682, 680, 687, 683, 679, 681,
};

static const char *s_metric_glucose_y_labels[] = {"160", "140", "120", "100", "80", "60"};
static const char *s_metric_temp_y_labels[] = {"39",  "38.5", "38", "37.5",
                                               "37", "36.5", "36", "35.5"};
static const char *s_metric_spo2_y_labels[] = {"100", "98", "96", "94", "92", "90"};
static const char *s_metric_weight_y_labels[] = {"70", "69", "68", "67",
                                                 "66", "65", "64", "63"};

static const uint8_t s_sleep_bar_top_pct[HEALTH_UI_SLEEP_DETAIL_MAX_DAYS] = {
    17, 15, 20, 12, 14, 16, 18, 21, 15, 19, 18, 16, 15, 14, 17, 19,
    23, 10, 12, 16, 18, 20, 21, 19, 13, 17, 14, 16, 18, 21, 22,
};

static const uint8_t s_sleep_bar_height_pct[HEALTH_UI_SLEEP_DETAIL_MAX_DAYS] = {
    64, 70, 68, 66, 65, 63, 62, 67, 64, 71, 63, 66, 68, 64, 62, 69,
    73, 56, 66, 64, 62, 61, 60, 68, 63, 58, 63, 64, 62, 69, 71,
};

static const char *s_sleep_x_tick_labels[HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT] = {"5",  "10", "15",
                                                                                  "20", "25", "30"};

static char s_time_text[8];
static char s_date_text[16];

static const sim_device_entry_t *get_device_entry(uint32_t device_index) {
  uint32_t count = health_ui_sim_data_get_device_count();
  uint32_t idx;

  if (0U == count) {
    return NULL;
  }

  idx = (device_index < count) ? device_index : (count - 1U);
  return &s_device_entries[idx];
}

static const sim_member_style_profile_t *get_member_style_profile(
    uint32_t member_index) {
  uint32_t profile_count = SIM_ARRAY_SIZE(s_member_style_profiles);
  if (0U == profile_count) {
    return NULL;
  }

  return &s_member_style_profiles[member_index % profile_count];
}

static const health_ui_user_detail_metric_card_t *get_single_metric_card(
    uint32_t member_index, health_ui_metric_detail_t metric) {
  const sim_member_style_profile_t *style_profile =
      get_member_style_profile(member_index);

  switch (metric) {
  case HEALTH_UI_METRIC_DETAIL_GLUCOSE:
    if ((NULL != style_profile) && (NULL != style_profile->glucose_detail)) {
      return style_profile->glucose_detail;
    }
    return &s_detail_glucose_normal;
  case HEALTH_UI_METRIC_DETAIL_BODY_TEMP:
    if ((NULL != style_profile) && (NULL != style_profile->temp_detail)) {
      return style_profile->temp_detail;
    }
    return &s_detail_temp_normal;
  case HEALTH_UI_METRIC_DETAIL_SPO2:
    return &s_detail_spo2_normal;
  case HEALTH_UI_METRIC_DETAIL_WEIGHT:
    return &s_detail_weight_normal;
  case HEALTH_UI_METRIC_DETAIL_BP:
  case HEALTH_UI_METRIC_DETAIL_SLEEP:
  default:
    return NULL;
  }
}

static void copy_axis_labels(const char **dst, uint32_t dst_max,
                             const char *const *src, uint32_t src_count) {
  uint32_t i;
  uint32_t copy_count = (src_count < dst_max) ? src_count : dst_max;

  if ((NULL == dst) || (NULL == src)) {
    return;
  }

  for (i = 0U; i < copy_count; i++) {
    dst[i] = src[i];
  }
  for (i = copy_count; i < dst_max; i++) {
    dst[i] = NULL;
  }
}

static void copy_points(int16_t *dst, uint32_t dst_max, const int16_t *src,
                        uint32_t src_count) {
  uint32_t i;
  uint32_t copy_count = (src_count < dst_max) ? src_count : dst_max;

  if ((NULL == dst) || (NULL == src)) {
    return;
  }

  for (i = 0U; i < copy_count; i++) {
    dst[i] = src[i];
  }
  for (i = copy_count; i < dst_max; i++) {
    dst[i] = 0;
  }
}

static void fill_system_status(unsigned long alive_sec,
                               health_ui_system_status_t *out_status) {
  static const char *k_day_names[7] = {"Sun", "Mon", "Tue", "Wed",
                                       "Thu", "Fri", "Sat"};
  uint32_t sim_minutes;
  uint32_t total_minutes;
  uint32_t minutes_of_day;
  uint32_t day_offset;
  uint32_t hour24;
  uint32_t minute;
  uint32_t day_idx;
  uint32_t day_of_month;

  if (NULL == out_status) {
    return;
  }

  sim_minutes = (uint32_t)(alive_sec / SIM_TIME_STEP_SEC);
  total_minutes = (SIM_BASE_HOUR * 60U) + SIM_BASE_MINUTE + sim_minutes;

  minutes_of_day = total_minutes % (24U * 60U);
  day_offset = total_minutes / (24U * 60U);
  hour24 = minutes_of_day / 60U;
  minute = minutes_of_day % 60U;
  day_idx = (SIM_BASE_DAY_OF_WEEK_INDEX + day_offset) % 7U;
  day_of_month = ((SIM_BASE_DAY_OF_MONTH - 1U) + day_offset) % 30U + 1U;

  (void)snprintf(s_time_text, sizeof(s_time_text), "%lu:%02lu",
                 (unsigned long)hour24, (unsigned long)minute);
  (void)snprintf(s_date_text, sizeof(s_date_text), "%s Apr %lu",
                 k_day_names[day_idx], (unsigned long)day_of_month);

  out_status->wifi_valid = true;
  out_status->ble_valid = true;
  out_status->wifi_connected = ((alive_sec / 12U) % 4U) != 3U;
  out_status->ble_connected = ((alive_sec / 8U) % 4U) <= 1U;
  out_status->time_text = s_time_text;
  out_status->date_text = s_date_text;
}

void health_ui_sim_data_init(void) {
  s_time_text[0] = '\0';
  s_date_text[0] = '\0';
}

void health_ui_sim_data_build_snapshot(unsigned long alive_sec,
                                       health_ui_sim_snapshot_t *out_snapshot) {
  if (NULL == out_snapshot) {
    return;
  }

  fill_system_status(alive_sec, &out_snapshot->system_status);
  out_snapshot->sequence = (uint32_t)alive_sec;
  out_snapshot->scenario = (uint32_t)((alive_sec / 20U) % 3U);
}

void health_ui_sim_data_build_system_status(
    unsigned long alive_sec, health_ui_system_status_t *out_status) {
  fill_system_status(alive_sec, out_status);
}

void health_ui_sim_data_build_member_card(uint32_t member_index,
                                          comp_user_card_data_t *out_card_data) {
  const sim_member_style_profile_t *style_profile;
  uint32_t name_count;
  uint32_t age_count;

  if (NULL == out_card_data) {
    return;
  }

  name_count = SIM_ARRAY_SIZE(s_member_names);
  age_count = SIM_ARRAY_SIZE(s_member_ages);
  style_profile = get_member_style_profile(member_index);

  out_card_data->name = s_member_names[member_index % name_count];
  out_card_data->card_bg_color_hex = COMP_USER_CARD_BG_COLOR_HEX;
  out_card_data->avatar_bg_color_hex =
      s_member_avatar_bg_colors[member_index % SIM_ARRAY_SIZE(s_member_avatar_bg_colors)];
  out_card_data->age_years = s_member_ages[member_index % age_count];
  out_card_data->conditions = NULL;
  out_card_data->condition_count = 0U;
  out_card_data->metric = s_metric_bp_normal;

  if (NULL != style_profile) {
    out_card_data->conditions = style_profile->conditions;
    out_card_data->condition_count = style_profile->condition_count;
    if (NULL != style_profile->dashboard_metric) {
      out_card_data->metric = *style_profile->dashboard_metric;
    }
  }
}

uint32_t health_ui_sim_data_get_device_count(void) {
  return SIM_ARRAY_SIZE(s_device_entries);
}

void health_ui_sim_data_build_device_card(
    uint32_t device_index, comp_device_card_data_t *out_card_data) {
  const sim_device_entry_t *entry;

  if (NULL == out_card_data) {
    return;
  }

  entry = get_device_entry(device_index);
  if (NULL == entry) {
    out_card_data->name = "Unknown Device";
    out_card_data->type_text = "Unknown Type";
    out_card_data->avatar_bg_color_hex = 0xE7EAF0;
    out_card_data->avatar_text = "--";
    return;
  }

  out_card_data->name = entry->name;
  out_card_data->type_text = entry->type_text;
  out_card_data->avatar_bg_color_hex = entry->avatar_bg_color_hex;
  out_card_data->avatar_text = entry->avatar_text;
}

uint32_t health_ui_sim_data_get_pairable_device_id(uint32_t device_index) {
  const sim_device_entry_t *entry = get_device_entry(device_index);
  if (NULL == entry) {
    return 0U;
  }

  return entry->device_id;
}

uint32_t health_ui_sim_data_get_pairing_step_count(uint32_t device_index) {
  const sim_device_entry_t *entry = get_device_entry(device_index);
  if (NULL == entry) {
    return 0U;
  }

  return entry->pairing_step_count;
}

bool health_ui_sim_data_build_pairing_step(uint32_t device_index, uint32_t step_index,
                                           health_ui_pairing_step_data_t *out_step_data) {
  const sim_device_entry_t *entry;
  const sim_pair_step_entry_t *step_entry;
  uint32_t idx;

  if (NULL == out_step_data) {
    return false;
  }

  entry = get_device_entry(device_index);
  if ((NULL == entry) || (NULL == entry->pairing_steps) || (0U == entry->pairing_step_count)) {
    return false;
  }

  idx = (step_index < entry->pairing_step_count) ? step_index
                                                  : (entry->pairing_step_count - 1U);
  step_entry = &entry->pairing_steps[idx];

  out_step_data->instruction_text = step_entry->instruction_text;
  out_step_data->cta_text = step_entry->cta_text;
  out_step_data->visual_text = step_entry->visual_text;
  out_step_data->visual_bg_color_hex = step_entry->visual_bg_color_hex;
  return true;
}

const char *health_ui_sim_data_get_device_pairing_guide(uint32_t device_index) {
  const sim_device_entry_t *entry = get_device_entry(device_index);
  if ((NULL == entry) || (NULL == entry->pairing_guide_text)) {
    return "Turn on device and keep it in range while connecting.";
  }

  return entry->pairing_guide_text;
}

bool health_ui_sim_data_get_device_pairing_result(uint32_t device_index) {
  const sim_device_entry_t *entry = get_device_entry(device_index);
  if (NULL == entry) {
    return false;
  }

  return entry->pairing_success;
}

void health_ui_sim_data_build_user_detail(uint32_t member_index,
                                          health_ui_user_detail_data_t *out_data) {
  const sim_member_style_profile_t *style_profile;

  if (NULL == out_data) {
    return;
  }

  style_profile = get_member_style_profile(member_index);

  out_data->bp = s_detail_bp_normal;
  out_data->glucose = s_detail_glucose_normal;
  out_data->body_temp = s_detail_temp_normal;
  out_data->spo2 = s_detail_spo2_normal;
  out_data->weight = s_detail_weight_normal;
  out_data->sleep_duration = s_detail_sleep_normal;

  if (NULL != style_profile) {
    if (NULL != style_profile->glucose_detail) {
      out_data->glucose = *style_profile->glucose_detail;
    }
    if (NULL != style_profile->temp_detail) {
      out_data->body_temp = *style_profile->temp_detail;
    }
  }
}

void health_ui_sim_data_build_bp_metric_detail(
    uint32_t member_index, health_ui_bp_metric_detail_data_t *out_data) {
  uint32_t i;

  if (NULL == out_data) {
    return;
  }

  out_data->period_text = "July 1 - July 7, 2025";
  out_data->point_count = HEALTH_UI_BP_DETAIL_MAX_POINTS;
  out_data->summary = s_detail_bp_normal;

  /* Keep BP detail deterministic for layout verification on every profile. */
  (void)member_index;

  for (i = 0U; i < HEALTH_UI_BP_DETAIL_MAX_POINTS; i++) {
    out_data->x_labels[i] = s_bp_detail_x_labels[i];
    out_data->sys_values[i] = s_bp_detail_sys_values[i];
    out_data->dia_values[i] = s_bp_detail_dia_values[i];
    out_data->pulse_values[i] = s_bp_detail_pulse_values[i];
  }
}

void health_ui_sim_data_build_single_metric_detail(
    uint32_t member_index, health_ui_metric_detail_t metric,
    health_ui_single_metric_detail_data_t *out_data) {
  const health_ui_user_detail_metric_card_t *card;
  const int16_t *series_values = NULL;
  uint32_t series_count = HEALTH_UI_SINGLE_METRIC_MAX_POINTS;
  uint32_t y_label_count = 0U;
  const char *const *y_labels = NULL;
  uint32_t i;

  if (NULL == out_data) {
    return;
  }

  card = get_single_metric_card(member_index, metric);
  if (NULL == card) {
    return;
  }

  out_data->title_text = card->title_text;
  out_data->period_text = "July 1 - July 7, 2025";
  out_data->summary_label_text = card->title_text;
  out_data->summary_unit_text = card->unit_text;
  out_data->summary_value_text = card->value_text;
  out_data->summary_status_text = card->status_text;
  out_data->summary_status_level = card->status_level;
  out_data->accent_color_hex = 0x4A89E8;
  out_data->point_count = HEALTH_UI_SINGLE_METRIC_MAX_POINTS;
  out_data->y_min = 0;
  out_data->y_max = 100;
  out_data->y_axis_title_text = card->title_text;
  out_data->y_label_count = 0U;
  out_data->normal_band_enabled = 0U;
  out_data->normal_band_min = 0;
  out_data->normal_band_max = 0;
  out_data->normal_band_color_hex = 0xEAF0F8;
  copy_axis_labels(out_data->x_labels, HEALTH_UI_SINGLE_METRIC_MAX_POINTS,
                   s_bp_detail_x_labels, HEALTH_UI_SINGLE_METRIC_MAX_POINTS);

  switch (metric) {
  case HEALTH_UI_METRIC_DETAIL_GLUCOSE:
    out_data->y_min = 60;
    out_data->y_max = 160;
    out_data->y_axis_title_text = "Blood Glucose (mg/dL)";
    out_data->normal_band_enabled = 1U;
    out_data->normal_band_min = 70;
    out_data->normal_band_max = 100;
    if (COMP_USER_STATUS_WARNING == card->status_level) {
      series_values = s_metric_glucose_values_warning;
    } else {
      series_values = s_metric_glucose_values_normal;
    }
    y_labels = s_metric_glucose_y_labels;
    y_label_count = SIM_ARRAY_SIZE(s_metric_glucose_y_labels);
    break;
  case HEALTH_UI_METRIC_DETAIL_BODY_TEMP:
    out_data->y_min = 355;
    out_data->y_max = 390;
    out_data->y_axis_title_text = "Body Temperature (°C)";
    out_data->normal_band_enabled = 1U;
    out_data->normal_band_min = 365;
    out_data->normal_band_max = 374;
    if (COMP_USER_STATUS_CRITICAL == card->status_level) {
      series_values = s_metric_temp_values_high;
    } else if (COMP_USER_STATUS_WARNING == card->status_level) {
      series_values = s_metric_temp_values_low;
    } else {
      series_values = s_metric_temp_values_normal;
    }
    y_labels = s_metric_temp_y_labels;
    y_label_count = SIM_ARRAY_SIZE(s_metric_temp_y_labels);
    break;
  case HEALTH_UI_METRIC_DETAIL_SPO2:
    out_data->y_min = 90;
    out_data->y_max = 100;
    out_data->y_axis_title_text = "SpO2 (%)";
    out_data->normal_band_enabled = 1U;
    out_data->normal_band_min = 95;
    out_data->normal_band_max = 100;
    series_values = s_metric_spo2_values;
    y_labels = s_metric_spo2_y_labels;
    y_label_count = SIM_ARRAY_SIZE(s_metric_spo2_y_labels);
    break;
  case HEALTH_UI_METRIC_DETAIL_WEIGHT:
    out_data->y_min = 630;
    out_data->y_max = 700;
    out_data->y_axis_title_text = "Weight (kg)";
    out_data->normal_band_enabled = 1U;
    out_data->normal_band_min = 630;
    out_data->normal_band_max = 680;
    series_values = s_metric_weight_values;
    y_labels = s_metric_weight_y_labels;
    y_label_count = SIM_ARRAY_SIZE(s_metric_weight_y_labels);
    break;
  case HEALTH_UI_METRIC_DETAIL_BP:
  case HEALTH_UI_METRIC_DETAIL_SLEEP:
  default:
    return;
  }

  copy_points(out_data->values, HEALTH_UI_SINGLE_METRIC_MAX_POINTS, series_values,
              series_count);
  copy_axis_labels(out_data->y_labels, HEALTH_UI_SINGLE_METRIC_MAX_Y_LABELS, y_labels,
                   y_label_count);
  out_data->y_label_count = y_label_count;

  for (i = y_label_count; i < HEALTH_UI_SINGLE_METRIC_MAX_Y_LABELS; i++) {
    out_data->y_labels[i] = NULL;
  }
}

void health_ui_sim_data_build_sleep_metric_detail(
    uint32_t member_index, health_ui_sleep_metric_detail_data_t *out_data) {
  uint32_t i;

  (void)member_index;
  if (NULL == out_data) {
    return;
  }

  out_data->period_text = "July 1 - July 7, 2025";
  out_data->avg_time_in_bed_hour = 8U;
  out_data->avg_time_in_bed_minute = 14U;
  out_data->avg_time_asleep_hour = 7U;
  out_data->avg_time_asleep_minute = 48U;
  out_data->day_count = HEALTH_UI_SLEEP_DETAIL_MAX_DAYS;
  out_data->avg_awake_minute = 29U;
  out_data->avg_rem_hour = 1U;
  out_data->avg_rem_minute = 32U;
  out_data->avg_core_hour = 5U;
  out_data->avg_core_minute = 29U;
  out_data->avg_deep_minute = 44U;

  for (i = 0U; i < HEALTH_UI_SLEEP_DETAIL_MAX_DAYS; i++) {
    out_data->bar_top_pct[i] = s_sleep_bar_top_pct[i];
    out_data->bar_height_pct[i] = s_sleep_bar_height_pct[i];
  }
  for (i = 0U; i < HEALTH_UI_SLEEP_DETAIL_X_TICK_COUNT; i++) {
    out_data->x_tick_labels[i] = s_sleep_x_tick_labels[i];
  }
}
