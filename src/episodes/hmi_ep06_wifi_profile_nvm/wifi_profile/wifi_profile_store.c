/*******************************************************************************
 * @file    wifi_profile_store.c
 * @brief   PC simulator mock -- WiFi profile store (RAM-backed, no NVM)
 *
 *          Stores a single WiFi profile in static memory.  Supports
 *          save / load / clear with the same API as the firmware version.
 ******************************************************************************/
#include "wifi_profile_store.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static wifi_profile_data_t s_stored_profile;
static bool s_has_profile = false;

bool wifi_profile_store_load(wifi_profile_data_t *out_profile)
{
    if (out_profile == NULL) {
        return false;
    }

    if (!s_has_profile) {
        printf("[MOCK][PROFILE] LOAD_EMPTY\n");
        return false;
    }

    (void)memcpy(out_profile, &s_stored_profile, sizeof(*out_profile));
    printf("[MOCK][PROFILE] LOAD_OK ssid=%s\n", out_profile->ssid);
    return true;
}

bool wifi_profile_store_save(const wifi_profile_data_t *profile)
{
    if (profile == NULL) {
        return false;
    }

    (void)memcpy(&s_stored_profile, profile, sizeof(s_stored_profile));
    s_has_profile = true;
    printf("[MOCK][PROFILE] SAVE_OK ssid=%s auto=%d\n",
           profile->ssid, (int)profile->auto_connect);
    return true;
}

bool wifi_profile_store_clear(void)
{
    (void)memset(&s_stored_profile, 0, sizeof(s_stored_profile));
    s_has_profile = false;
    printf("[MOCK][PROFILE] CLEAR_OK\n");
    return true;
}
