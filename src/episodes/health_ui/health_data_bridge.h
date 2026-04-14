/*******************************************************************************
 * health_data_bridge.h — BLE IPC event handler → health_data_provider store
 *
 * Registers as CM55 BLE IPC callback. On IPC_EVT_BLE_READ_RESULT (0xC5) or
 * IPC_EVT_BLE_PAIRED_READ_RESULT (0xCB), unpacks the ipc_ble_read_result_t
 * and stores via hdp_store_measurement().
 ******************************************************************************/

#ifndef HEALTH_DATA_BRIDGE_H
#define HEALTH_DATA_BRIDGE_H

/** Initialise the BLE → health-data bridge.
 *  Registers the BLE IPC event handler with the CM55 IPC app. */
void health_data_bridge_init(void);

#endif /* HEALTH_DATA_BRIDGE_H */
