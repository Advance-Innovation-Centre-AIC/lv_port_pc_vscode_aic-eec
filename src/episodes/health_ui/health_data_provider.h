/*******************************************************************************
 * File Name        : health_data_provider.h
 *
 * Description      : Stub health data provider for SIM-ONLY mode.
 *                    All hdp_* functions return "no real data" so the
 *                    health_ui_data_adapter falls back to simulation data.
 *
 *                    When BLE IPC is integrated (ENABLE_BLE_DATA_BRIDGE=1),
 *                    replace this with the real health_data_provider.h/c from
 *                    tesaiot-health-gateway.
 *
 *******************************************************************************/

#ifndef HEALTH_DATA_PROVIDER_H
#define HEALTH_DATA_PROVIDER_H

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * Configuration
 *******************************************************************************/
#define HDP_MAX_MEMBERS         (4U)
#define HDP_MAX_DEVICES         (6U)
#define HDP_MAX_MEASUREMENTS    (32U)
#define HDP_MAX_HISTORY_POINTS  (7U)

/*******************************************************************************
 * Measurement types
 *******************************************************************************/
typedef enum {
    HDP_MEAS_UNKNOWN       = 0U,
    HDP_MEAS_TEMPERATURE   = 1U,
    HDP_MEAS_SPO2_HR       = 2U,
    HDP_MEAS_GLUCOSE       = 3U,
    HDP_MEAS_BLOOD_PRESSURE = 4U
} hdp_measurement_type_t;

/*******************************************************************************
 * Measurement record
 *******************************************************************************/
typedef struct {
    hdp_measurement_type_t type;
    int32_t  raw_value;
    uint32_t timestamp_sec;
    uint8_t  device_type;
    uint8_t  member_index;
    uint8_t  addr[6];
    char     device_name[32];
} hdp_measurement_t;

/*******************************************************************************
 * Unpacked measurement values
 *******************************************************************************/
typedef struct {
    uint16_t sys_mmhg;
    uint16_t dia_mmhg;
    uint16_t pulse_bpm;
} hdp_bp_values_t;

typedef struct {
    uint16_t spo2_pct;
    uint16_t hr_bpm;
} hdp_spo2_hr_values_t;

/*******************************************************************************
 * Device info
 *******************************************************************************/
typedef struct {
    bool     active;
    uint8_t  device_type;
    uint8_t  addr[6];
    char     name[32];
    uint32_t last_measurement_sec;
    uint32_t measurement_count;
} hdp_device_info_t;

/* TODO REAL-DATA-PROVIDER: Replace stubs below with real implementation.
 * Create health_data_provider.c with:
 *   - Ring buffer (HDP_MAX_MEASUREMENTS per member, circular)
 *   - hdp_store_measurement(device_type, meas_type, value, timestamp_ms)
 *     Called from health_data_bridge.c when IPC_EVT_BLE_READ_RESULT arrives
 *   - hdp_get_latest() / hdp_get_history() return real stored data
 *   - hdp_has_real_data() returns true when any BLE measurement exists
 *   - Thread safety: taskENTER_CRITICAL/EXIT (IPC task writes, LVGL task reads)
 *   - health_ui_data_adapter.c already overlays real data on sim — no UI changes
 * Add health_data_provider.c to proj_cm55/Makefile SOURCES. */

/*******************************************************************************
 * Stub API — all return "no data" so UI falls back to sim
 *******************************************************************************/

static inline void hdp_init(void) {}

static inline bool hdp_get_latest(uint8_t member_index,
                                  hdp_measurement_type_t type,
                                  hdp_measurement_t *out) {
    (void)member_index; (void)type; (void)out;
    return false;
}

static inline uint32_t hdp_get_measurement_count(uint8_t member_index) {
    (void)member_index;
    return 0U;
}

static inline uint32_t hdp_get_history(uint8_t member_index,
                                       hdp_measurement_type_t type,
                                       hdp_measurement_t *out_array,
                                       uint32_t max_count) {
    (void)member_index; (void)type; (void)out_array; (void)max_count;
    return 0U;
}

static inline void hdp_unpack_bp(int32_t raw_value, hdp_bp_values_t *out) {
    (void)raw_value;
    if (out) { out->sys_mmhg = 0; out->dia_mmhg = 0; out->pulse_bpm = 0; }
}

static inline void hdp_unpack_spo2_hr(int32_t raw_value,
                                       hdp_spo2_hr_values_t *out) {
    (void)raw_value;
    if (out) { out->spo2_pct = 0; out->hr_bpm = 0; }
}

static inline int16_t hdp_get_temp_x10(int32_t raw_value) {
    return (int16_t)raw_value;
}

static inline int16_t hdp_get_glucose_mgdl(int32_t raw_value) {
    return (int16_t)raw_value;
}

static inline bool hdp_get_device_info(uint32_t device_index,
                                       hdp_device_info_t *out) {
    (void)device_index; (void)out;
    return false;
}

static inline uint32_t hdp_get_device_count(void) { return 0U; }

static inline void hdp_set_rtc_epoch(uint32_t epoch_sec, uint32_t tick_base) {
    (void)epoch_sec; (void)tick_base;
}

static inline uint32_t hdp_get_current_epoch(void) { return 0U; }

static inline bool hdp_has_real_data(void) { return false; }

#endif /* HEALTH_DATA_PROVIDER_H */
