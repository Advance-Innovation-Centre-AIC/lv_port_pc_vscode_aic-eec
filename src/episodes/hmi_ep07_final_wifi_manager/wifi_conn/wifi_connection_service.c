/*******************************************************************************
 * @file    wifi_connection_service.c
 * @brief   PC simulator mock -- WiFi connection service
 *
 *          Simulates connection with a short LVGL timer delay, then reports
 *          success with demo IP "192.168.1.42".  Disconnect is instant.
 *          No real WiFi hardware is used.
 ******************************************************************************/
#include "wifi_connection_service.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lvgl.h"

/* ---- internal state ---- */
#define MOCK_CONNECT_DELAY_MS   1500U

static struct {
    bool             initialized;
    wifi_conn_state_t state;
    wifi_profile_data_t active_profile;
    bool             have_profile;
    bool             connected;
    bool             internet_ok;
    int16_t          rssi;
    char             ssid[WIFI_PROFILE_SSID_MAX_LEN + 1U];
    char             security[WIFI_PROFILE_SECURITY_MAX_LEN + 1U];
    char             ip_addr[16];
} s_ctx;

static lv_timer_t *s_connect_timer = NULL;

/* ---- helper: state text ---- */

const char *wifi_connection_service_state_text(wifi_conn_state_t state)
{
    switch (state) {
    case WIFI_CONN_STATE_IDLE:           return "Idle";
    case WIFI_CONN_STATE_CONNECTING:     return "Connecting";
    case WIFI_CONN_STATE_CONNECTED:      return "Connected";
    case WIFI_CONN_STATE_DISCONNECTING:  return "Disconnecting";
    case WIFI_CONN_STATE_RECONNECT_WAIT: return "Reconnect Wait";
    case WIFI_CONN_STATE_ERROR:          return "Error";
    default:                             return "Unknown";
    }
}

/* ---- timer callback: completes a simulated connection ---- */

static void mock_connect_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    s_ctx.state      = WIFI_CONN_STATE_CONNECTED;
    s_ctx.connected  = true;
    s_ctx.internet_ok = true;
    s_ctx.rssi       = -42;

    (void)strncpy(s_ctx.ssid, s_ctx.active_profile.ssid,
                  sizeof(s_ctx.ssid) - 1U);
    s_ctx.ssid[sizeof(s_ctx.ssid) - 1U] = '\0';

    (void)strncpy(s_ctx.security, s_ctx.active_profile.security,
                  sizeof(s_ctx.security) - 1U);
    s_ctx.security[sizeof(s_ctx.security) - 1U] = '\0';

    (void)snprintf(s_ctx.ip_addr, sizeof(s_ctx.ip_addr), "192.168.1.42");

    printf("[MOCK][WIFI_CONN] CONNECT_OK ssid=%s ip=%s\n",
           s_ctx.ssid, s_ctx.ip_addr);

    /* One-shot timer -- delete it. */
    lv_timer_delete(s_connect_timer);
    s_connect_timer = NULL;
}

/* ---- public API ---- */

bool wifi_connection_service_init(void)
{
    if (s_ctx.initialized) {
        return true;
    }

    (void)memset(&s_ctx, 0, sizeof(s_ctx));
    s_ctx.state = WIFI_CONN_STATE_IDLE;
    (void)snprintf(s_ctx.ip_addr, sizeof(s_ctx.ip_addr), "-");
    s_ctx.initialized = true;

    printf("[MOCK][WIFI_CONN] INIT_OK\n");
    return true;
}

bool wifi_connection_service_connect_profile(const wifi_profile_data_t *profile,
                                              bool user_initiated)
{
    if (!s_ctx.initialized || (profile == NULL) || (profile->ssid[0] == '\0')) {
        return false;
    }

    /* Cancel any pending connection attempt. */
    if (s_connect_timer != NULL) {
        lv_timer_delete(s_connect_timer);
        s_connect_timer = NULL;
    }

    s_ctx.active_profile = *profile;
    s_ctx.have_profile   = true;
    s_ctx.state          = WIFI_CONN_STATE_CONNECTING;
    s_ctx.connected      = false;
    s_ctx.internet_ok    = false;

    printf("[MOCK][WIFI_CONN] CONNECT_START ssid=%s user=%d\n",
           profile->ssid, (int)user_initiated);

    /* Schedule a one-shot timer to simulate connection delay. */
    s_connect_timer = lv_timer_create(mock_connect_timer_cb,
                                       MOCK_CONNECT_DELAY_MS, NULL);
    lv_timer_set_repeat_count(s_connect_timer, 1);

    return true;
}

void wifi_connection_service_disconnect(void)
{
    if (!s_ctx.initialized) {
        return;
    }

    /* Cancel any pending connection attempt. */
    if (s_connect_timer != NULL) {
        lv_timer_delete(s_connect_timer);
        s_connect_timer = NULL;
    }

    s_ctx.state       = WIFI_CONN_STATE_IDLE;
    s_ctx.connected   = false;
    s_ctx.internet_ok = false;
    s_ctx.rssi        = 0;
    (void)snprintf(s_ctx.ip_addr, sizeof(s_ctx.ip_addr), "-");

    printf("[MOCK][WIFI_CONN] DISCONNECT_OK\n");
}

void wifi_connection_service_retry_now(void)
{
    if (!s_ctx.initialized || !s_ctx.have_profile) {
        return;
    }

    printf("[MOCK][WIFI_CONN] RETRY_NOW\n");
    (void)wifi_connection_service_connect_profile(&s_ctx.active_profile, false);
}

bool wifi_connection_service_get_snapshot(wifi_connection_snapshot_t *out_snapshot)
{
    if ((out_snapshot == NULL) || !s_ctx.initialized) {
        return false;
    }

    (void)memset(out_snapshot, 0, sizeof(*out_snapshot));
    out_snapshot->initialized       = s_ctx.initialized;
    out_snapshot->state             = s_ctx.state;
    out_snapshot->connected         = s_ctx.connected;
    out_snapshot->have_profile      = s_ctx.have_profile;
    out_snapshot->internet_ok       = s_ctx.internet_ok;
    out_snapshot->rssi              = s_ctx.rssi;
    out_snapshot->reconnect_enabled = true;
    out_snapshot->last_rslt         = CY_RSLT_SUCCESS;
    out_snapshot->last_ping_rslt    = CY_RSLT_SUCCESS;

    (void)strncpy(out_snapshot->ssid, s_ctx.ssid,
                  sizeof(out_snapshot->ssid) - 1U);
    (void)strncpy(out_snapshot->security, s_ctx.security,
                  sizeof(out_snapshot->security) - 1U);
    (void)strncpy(out_snapshot->ip_addr, s_ctx.ip_addr,
                  sizeof(out_snapshot->ip_addr) - 1U);

    return true;
}
