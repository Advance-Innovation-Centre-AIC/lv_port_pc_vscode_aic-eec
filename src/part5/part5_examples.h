/*******************************************************************************
 * File: part5_examples.h
 * Description: Part 5 WiFi Manager Examples - PC Simulator Version
 *
 * Provides simulated WiFi network data for testing the UI on PC.
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#ifndef PART5_EXAMPLES_H
#define PART5_EXAMPLES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Example 1: WiFi Network List (Simulated) */
void part5_ex1_wifi_list(void);

/* Example 2: WiFi Connect/Disconnect (Simulated) */
void part5_ex2_wifi_connect(void);

/* Example 3: TCP/IP Information (Simulated) */
void part5_ex3_tcpip_info(void);

/* Example 4: Hardware Information (Simulated) */
void part5_ex4_hardware_info(void);

/* Example 5: Full WiFi Manager (macOS Style - Simulated) */
void part5_ex5_wifi_manager(void);

/* Example 6: WiFi Status Dashboard (Simulated) */
void part5_ex6_wifi_status(void);

/* Example 7: IoT Dashboard (Simulated) */
void part5_ex7_iot_dashboard(void);

/* Example 8: MQTT Preview (UI Only) */
void part5_ex8_mqtt_preview(void);

#ifdef __cplusplus
}
#endif

#endif /* PART5_EXAMPLES_H */
