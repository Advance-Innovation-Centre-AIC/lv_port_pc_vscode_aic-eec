/*******************************************************************************
 * File Name        : health_ui_data_adapter.h
 *
 * Description      : Unified data adapter for Health UI pages. Pulls real BLE
 *                    measurement data from health_data_provider when available;
 *                    falls back to sim data otherwise. Drop-in replacement for
 *                    direct health_ui_sim_data_build_* calls.
 *
 *******************************************************************************/

#ifndef HEALTH_UI_DATA_ADAPTER_H
#define HEALTH_UI_DATA_ADAPTER_H

#include "sim/health_ui_sim_data.h"
#include "components/content/comp_user_card.h"
#include "components/content/comp_device_card.h"
#include "health_ui_root.h"

/** Build user detail page data. Uses real data if available for the member. */
void health_ui_data_build_user_detail(uint32_t member_index,
                                      health_ui_user_detail_data_t *out_data);

/** Build BP metric detail page data. */
void health_ui_data_build_bp_metric_detail(
    uint32_t member_index, health_ui_bp_metric_detail_data_t *out_data);

/** Build single metric detail (glucose/temp/spo2/weight). */
void health_ui_data_build_single_metric_detail(
    uint32_t member_index, health_ui_metric_detail_t metric,
    health_ui_single_metric_detail_data_t *out_data);

/** Build sleep metric detail. */
void health_ui_data_build_sleep_metric_detail(
    uint32_t member_index, health_ui_sleep_metric_detail_data_t *out_data);

/** Build member card for dashboard. */
void health_ui_data_build_member_card(uint32_t member_index,
                                      comp_user_card_data_t *out_card_data);

/** Build device card for dashboard. */
void health_ui_data_build_device_card(uint32_t device_index,
                                      comp_device_card_data_t *out_card_data);

/** Get device count (real + sim). */
uint32_t health_ui_data_get_device_count(void);

#endif /* HEALTH_UI_DATA_ADAPTER_H */
