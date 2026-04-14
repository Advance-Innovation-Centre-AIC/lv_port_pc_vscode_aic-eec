/*******************************************************************************
 * @file    wifi_scan_service.c
 * @brief   PC simulator mock -- WiFi scan service
 *
 *          Returns 5 hardcoded demo APs with realistic SSIDs, RSSI values,
 *          and security types.  No real hardware is touched.
 ******************************************************************************/
#include "wifi_scan_service.h"

#include <stdio.h>
#include <string.h>

/* ---- demo AP table (sorted strongest-first) ---- */
typedef struct {
    const char *ssid;
    int16_t     rssi;
    const char *security;
} mock_ap_entry_t;

static const mock_ap_entry_t s_demo_aps[] = {
    { "TESAIoT-Lab",       -38, "WPA2-AES-PSK" },
    { "Infineon-Guest",     -52, "WPA3-SAE"     },
    { "TESA-Workshop",      -61, "WPA2-AES-PSK" },
    { "IoT-Classroom",      -74, "WPA2-AES-PSK" },
    { "OpenNet",            -85, "OPEN"          },
};

#define MOCK_AP_COUNT  (sizeof(s_demo_aps) / sizeof(s_demo_aps[0]))

/* ---- public API ---- */

void wifi_scan_service_init(wifi_scan_service_t *service)
{
    if (service == NULL) {
        return;
    }

    (void)memset(service, 0, sizeof(*service));
    service->radio_ready = true;           /* always ready on PC */
    printf("[MOCK][WIFI_LIST] INIT\n");
}

cy_rslt_t wifi_scan_service_preinit(void)
{
    printf("[MOCK][WIFI_LIST] RADIO_READY (mock)\n");
    return CY_RSLT_SUCCESS;
}

bool wifi_scan_service_start(wifi_scan_service_t *service)
{
    if (service == NULL) {
        return false;
    }

    if (service->scanning) {
        return false;
    }

    /* Populate the AP list immediately with demo data. */
    service->ap_count = 0U;
    service->scan_done_pending = false;
    service->scan_error_pending = false;
    service->scanning = true;

    for (uint16_t i = 0U; (i < MOCK_AP_COUNT) && (i < WIFI_SCAN_MAX_APS); i++) {
        wifi_scan_ap_t *entry = &service->aps[service->ap_count];
        (void)memset(entry, 0, sizeof(*entry));
        (void)strncpy(entry->ssid, s_demo_aps[i].ssid, WIFI_SCAN_SSID_MAX_LEN);
        entry->ssid[WIFI_SCAN_SSID_MAX_LEN] = '\0';
        entry->rssi = s_demo_aps[i].rssi;
        (void)strncpy(entry->security, s_demo_aps[i].security, WIFI_SCAN_SECURITY_MAX_LEN);
        entry->security[WIFI_SCAN_SECURITY_MAX_LEN] = '\0';
        service->ap_count++;
    }

    /* Mark scan as complete so the next process() call picks it up. */
    service->scan_done_pending = true;

    printf("[MOCK][WIFI_LIST] SCAN_START (mock, %u APs)\n",
           (unsigned int)service->ap_count);
    return true;
}

bool wifi_scan_service_process(wifi_scan_service_t *service)
{
    if (service == NULL) {
        return false;
    }

    if (service->scan_done_pending) {
        service->scan_done_pending = false;
        service->scanning = false;
        service->scan_sequence++;
        printf("[MOCK][WIFI_LIST] SCAN_DONE count=%u seq=%u\n",
               (unsigned int)service->ap_count,
               (unsigned int)service->scan_sequence);
        return true;
    }

    if (service->scan_error_pending) {
        service->scan_error_pending = false;
        service->scanning = false;
        return true;
    }

    return false;
}

const wifi_scan_ap_t *wifi_scan_service_get_list(const wifi_scan_service_t *service,
                                                  uint16_t *count)
{
    if (count != NULL) {
        *count = (service != NULL) ? service->ap_count : 0U;
    }

    return (service != NULL) ? service->aps : NULL;
}
