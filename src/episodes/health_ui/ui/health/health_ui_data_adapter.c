/*******************************************************************************
 * File Name        : health_ui_data_adapter.c
 *
 * Description      : Unified data adapter. Checks health_data_provider for
 *                    real BLE measurements; falls back to sim data when none
 *                    available. Formats raw values into display strings.
 *
 *******************************************************************************/

#include "health_ui_data_adapter.h"
#include "health_data_provider.h"
#include "sim/health_ui_sim_data.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * String formatting buffers (static — valid until next call)
 *******************************************************************************/
#define FMT_BUF_COUNT (32U)
#define FMT_BUF_SIZE  (32U)
static char s_fmt_bufs[FMT_BUF_COUNT][FMT_BUF_SIZE];
static uint32_t s_fmt_idx = 0U;

static const char *fmt_alloc(const char *fmt, ...) {
    char *buf = s_fmt_bufs[s_fmt_idx % FMT_BUF_COUNT];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, FMT_BUF_SIZE, fmt, args);
    va_end(args);
    s_fmt_idx++;
    return buf;
}

/*******************************************************************************
 * Timestamp formatting: epoch → "2 min ago", "1 hr ago", etc.
 *******************************************************************************/
static const char *format_time_ago(uint32_t measurement_epoch) {
    uint32_t now = hdp_get_current_epoch();
    if ((0U == measurement_epoch) || (0U == now) || (now < measurement_epoch)) {
        return "Just now";
    }
    uint32_t diff = now - measurement_epoch;
    if (diff < 60U) {
        return "Just now";
    } else if (diff < 3600U) {
        return fmt_alloc("%lu min ago", (unsigned long)(diff / 60U));
    } else if (diff < 86400U) {
        return fmt_alloc("%lu hr ago", (unsigned long)(diff / 3600U));
    } else {
        return fmt_alloc("%lu days ago", (unsigned long)(diff / 86400U));
    }
}

/*******************************************************************************
 * Status level inference
 *******************************************************************************/
static comp_user_status_level_t bp_status_level(uint16_t sys, uint16_t dia) {
    if ((sys >= 180U) || (dia >= 120U)) return COMP_USER_STATUS_CRITICAL;
    if ((sys >= 140U) || (dia >= 90U))  return COMP_USER_STATUS_WARNING;
    return COMP_USER_STATUS_NORMAL;
}

static const char *bp_status_text(comp_user_status_level_t level) {
    switch (level) {
        case COMP_USER_STATUS_CRITICAL: return "High";
        case COMP_USER_STATUS_WARNING:  return "Elevated";
        default:                        return "Normal";
    }
}

static comp_user_status_level_t glucose_status_level(int16_t mgdl) {
    if (mgdl >= 200) return COMP_USER_STATUS_CRITICAL;
    if (mgdl >= 126) return COMP_USER_STATUS_WARNING;
    return COMP_USER_STATUS_NORMAL;
}

static comp_user_status_level_t temp_status_level(int16_t temp_x10) {
    if (temp_x10 >= 385) return COMP_USER_STATUS_CRITICAL;
    if (temp_x10 >= 375) return COMP_USER_STATUS_WARNING;
    return COMP_USER_STATUS_NORMAL;
}

static comp_user_status_level_t spo2_status_level(uint16_t pct) {
    if (pct < 90U) return COMP_USER_STATUS_CRITICAL;
    if (pct < 95U) return COMP_USER_STATUS_WARNING;
    return COMP_USER_STATUS_NORMAL;
}

static const char *status_text(comp_user_status_level_t level) {
    switch (level) {
        case COMP_USER_STATUS_CRITICAL: return "Critical";
        case COMP_USER_STATUS_WARNING:  return "Warning";
        default:                        return "Normal";
    }
}

/*******************************************************************************
 * Fill metric card from real data
 *******************************************************************************/
static bool fill_bp_summary_real(uint8_t member_index,
                                  health_ui_user_detail_bp_summary_t *bp) {
    hdp_measurement_t meas;
    hdp_bp_values_t vals;
    comp_user_status_level_t level;

    if (!hdp_get_latest(member_index, HDP_MEAS_BLOOD_PRESSURE, &meas)) {
        return false;
    }
    hdp_unpack_bp(meas.raw_value, &vals);
    level = bp_status_level(vals.sys_mmhg, vals.dia_mmhg);

    bp->time_text = format_time_ago(meas.timestamp_sec);
    bp->sys_value_text = fmt_alloc("%u", (unsigned)vals.sys_mmhg);
    bp->sys_unit_text = "mmHg";
    bp->dia_value_text = fmt_alloc("%u", (unsigned)vals.dia_mmhg);
    bp->dia_unit_text = "mmHg";
    bp->pulse_value_text = fmt_alloc("%u", (unsigned)vals.pulse_bpm);
    bp->pulse_unit_text = "bpm";
    bp->status_text = bp_status_text(level);
    bp->status_level = level;
    return true;
}

static bool fill_metric_card_real(uint8_t member_index,
                                   hdp_measurement_type_t type,
                                   const char *title,
                                   health_ui_user_detail_metric_card_t *card) {
    hdp_measurement_t meas;
    hdp_measurement_t history[2];
    uint32_t count;

    if (!hdp_get_latest(member_index, type, &meas)) {
        return false;
    }

    card->title_text = title;
    card->time_text = format_time_ago(meas.timestamp_sec);
    card->highlighted = false;

    switch (type) {
        case HDP_MEAS_GLUCOSE: {
            int16_t mgdl = hdp_get_glucose_mgdl(meas.raw_value);
            card->value_text = fmt_alloc("%d", (int)mgdl);
            card->unit_text = "mg/dL";
            card->status_level = glucose_status_level(mgdl);
            break;
        }
        case HDP_MEAS_TEMPERATURE: {
            int16_t tx10 = hdp_get_temp_x10(meas.raw_value);
            card->value_text = fmt_alloc("%d.%d", (int)(tx10 / 10), (int)(tx10 % 10));
            card->unit_text = "\xC2\xB0""C";
            card->status_level = temp_status_level(tx10);
            break;
        }
        case HDP_MEAS_SPO2_HR: {
            hdp_spo2_hr_values_t v;
            hdp_unpack_spo2_hr(meas.raw_value, &v);
            card->value_text = fmt_alloc("%u%%", (unsigned)v.spo2_pct);
            card->unit_text = "SpO2";
            card->status_level = spo2_status_level(v.spo2_pct);
            break;
        }
        default:
            card->value_text = "--";
            card->unit_text = "";
            card->status_level = COMP_USER_STATUS_NORMAL;
            break;
    }
    card->status_text = status_text(card->status_level);

    /* Previous reading */
    count = hdp_get_history(member_index, type, history, 2U);
    if (count >= 2U) {
        card->previous_available = true;
        card->previous_time_text = "Previous";
        switch (type) {
            case HDP_MEAS_GLUCOSE:
                card->previous_value_text = fmt_alloc("%d", (int)hdp_get_glucose_mgdl(history[1].raw_value));
                card->previous_unit_text = "mg/dL";
                break;
            case HDP_MEAS_TEMPERATURE: {
                int16_t ptx10 = hdp_get_temp_x10(history[1].raw_value);
                card->previous_value_text = fmt_alloc("%d.%d", (int)(ptx10/10), (int)(ptx10%10));
                card->previous_unit_text = "\xC2\xB0""C";
                break;
            }
            case HDP_MEAS_SPO2_HR: {
                hdp_spo2_hr_values_t pv;
                hdp_unpack_spo2_hr(history[1].raw_value, &pv);
                card->previous_value_text = fmt_alloc("%u%%", (unsigned)pv.spo2_pct);
                card->previous_unit_text = "SpO2";
                break;
            }
            default:
                card->previous_value_text = "--";
                card->previous_unit_text = "";
                break;
        }
    } else {
        card->previous_available = false;
        card->previous_time_text = "";
        card->previous_value_text = "";
        card->previous_unit_text = "";
    }
    return true;
}

/*******************************************************************************
 * Public API — user detail
 *******************************************************************************/
void health_ui_data_build_user_detail(uint32_t member_index,
                                      health_ui_user_detail_data_t *out_data) {
    /* Start with sim data as base (fills all fields with defaults) */
    health_ui_sim_data_build_user_detail(member_index, out_data);

    /* Overlay real data where available */
    s_fmt_idx = 0U; /* reset format buffer pool */

    fill_bp_summary_real((uint8_t)member_index, &out_data->bp);
    fill_metric_card_real((uint8_t)member_index, HDP_MEAS_GLUCOSE,
                          "Blood Glucose", &out_data->glucose);
    fill_metric_card_real((uint8_t)member_index, HDP_MEAS_TEMPERATURE,
                          "Body Temperature", &out_data->body_temp);
    fill_metric_card_real((uint8_t)member_index, HDP_MEAS_SPO2_HR,
                          "SpO2", &out_data->spo2);
    /* Weight and sleep: no BLE sensor — keep sim data */
}

/*******************************************************************************
 * Public API — BP metric detail (chart)
 *******************************************************************************/
void health_ui_data_build_bp_metric_detail(
    uint32_t member_index, health_ui_bp_metric_detail_data_t *out_data) {
    hdp_measurement_t history[HEALTH_UI_BP_DETAIL_MAX_POINTS];
    uint32_t count, i;

    /* Start with sim data */
    health_ui_sim_data_build_bp_metric_detail(member_index, out_data);

    count = hdp_get_history((uint8_t)member_index, HDP_MEAS_BLOOD_PRESSURE,
                            history, HEALTH_UI_BP_DETAIL_MAX_POINTS);
    if (0U == count) {
        return; /* keep sim data */
    }

    s_fmt_idx = 0U;
    out_data->period_text = "Recent readings";
    out_data->point_count = count;

    /* History is most-recent-first; chart wants chronological (oldest left) */
    for (i = 0U; i < count; i++) {
        uint32_t ri = count - 1U - i; /* reverse index */
        hdp_bp_values_t v;
        hdp_unpack_bp(history[ri].raw_value, &v);
        out_data->sys_values[i] = (int16_t)v.sys_mmhg;
        out_data->dia_values[i] = (int16_t)v.dia_mmhg;
        out_data->pulse_values[i] = (int16_t)v.pulse_bpm;
        out_data->x_labels[i] = fmt_alloc("#%u", (unsigned)(i + 1U));
    }

    /* Update summary from latest */
    fill_bp_summary_real((uint8_t)member_index, &out_data->summary);
}

/*******************************************************************************
 * Public API — single metric detail (glucose/temp/spo2/weight chart)
 *******************************************************************************/
void health_ui_data_build_single_metric_detail(
    uint32_t member_index, health_ui_metric_detail_t metric,
    health_ui_single_metric_detail_data_t *out_data) {
    hdp_measurement_type_t hdp_type;
    hdp_measurement_t history[HEALTH_UI_SINGLE_METRIC_MAX_POINTS];
    uint32_t count, i;

    /* Start with sim data */
    health_ui_sim_data_build_single_metric_detail(member_index, metric, out_data);

    /* Map UI metric to hdp type */
    switch (metric) {
        case HEALTH_UI_METRIC_DETAIL_GLUCOSE:   hdp_type = HDP_MEAS_GLUCOSE; break;
        case HEALTH_UI_METRIC_DETAIL_BODY_TEMP: hdp_type = HDP_MEAS_TEMPERATURE; break;
        case HEALTH_UI_METRIC_DETAIL_SPO2:      hdp_type = HDP_MEAS_SPO2_HR; break;
        default: return; /* weight — no BLE sensor, keep sim */
    }

    count = hdp_get_history((uint8_t)member_index, hdp_type, history,
                            HEALTH_UI_SINGLE_METRIC_MAX_POINTS);
    if (0U == count) {
        return; /* keep sim data */
    }

    s_fmt_idx = 0U;
    out_data->period_text = "Recent readings";
    out_data->point_count = count;

    for (i = 0U; i < count; i++) {
        uint32_t ri = count - 1U - i;
        switch (hdp_type) {
            case HDP_MEAS_GLUCOSE:
                out_data->values[i] = hdp_get_glucose_mgdl(history[ri].raw_value);
                break;
            case HDP_MEAS_TEMPERATURE:
                out_data->values[i] = hdp_get_temp_x10(history[ri].raw_value);
                break;
            case HDP_MEAS_SPO2_HR: {
                hdp_spo2_hr_values_t v;
                hdp_unpack_spo2_hr(history[ri].raw_value, &v);
                out_data->values[i] = (int16_t)v.spo2_pct;
                break;
            }
            default: break;
        }
        out_data->x_labels[i] = fmt_alloc("#%u", (unsigned)(i + 1U));
    }

    /* Update summary from latest */
    hdp_measurement_t latest;
    if (hdp_get_latest((uint8_t)member_index, hdp_type, &latest)) {
        switch (hdp_type) {
            case HDP_MEAS_GLUCOSE: {
                int16_t mgdl = hdp_get_glucose_mgdl(latest.raw_value);
                out_data->summary_value_text = fmt_alloc("%d", (int)mgdl);
                out_data->summary_status_level = glucose_status_level(mgdl);
                break;
            }
            case HDP_MEAS_TEMPERATURE: {
                int16_t tx10 = hdp_get_temp_x10(latest.raw_value);
                out_data->summary_value_text = fmt_alloc("%d.%d", (int)(tx10/10), (int)(tx10%10));
                out_data->summary_status_level = temp_status_level(tx10);
                break;
            }
            case HDP_MEAS_SPO2_HR: {
                hdp_spo2_hr_values_t sv;
                hdp_unpack_spo2_hr(latest.raw_value, &sv);
                out_data->summary_value_text = fmt_alloc("%u", (unsigned)sv.spo2_pct);
                out_data->summary_status_level = spo2_status_level(sv.spo2_pct);
                break;
            }
            default: break;
        }
        out_data->summary_status_text = status_text(out_data->summary_status_level);
    }
}

/*******************************************************************************
 * Public API — sleep (no BLE sensor, pure passthrough)
 *******************************************************************************/
void health_ui_data_build_sleep_metric_detail(
    uint32_t member_index, health_ui_sleep_metric_detail_data_t *out_data) {
    health_ui_sim_data_build_sleep_metric_detail(member_index, out_data);
}

/*******************************************************************************
 * Public API — dashboard cards
 *******************************************************************************/
void health_ui_data_build_member_card(uint32_t member_index,
                                      comp_user_card_data_t *out_card_data) {
    /* Sim data fills layout defaults; real data overlay in user detail only */
    health_ui_sim_data_build_member_card(member_index, out_card_data);
}

void health_ui_data_build_device_card(uint32_t device_index,
                                      comp_device_card_data_t *out_card_data) {
    health_ui_sim_data_build_device_card(device_index, out_card_data);
}

uint32_t health_ui_data_get_device_count(void) {
    return health_ui_sim_data_get_device_count();
}
