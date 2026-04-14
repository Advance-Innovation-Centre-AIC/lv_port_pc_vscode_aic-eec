/* Dummy app_logo image descriptor for PC simulator */
#include "lvgl.h"

/* Minimal 1x1 pixel placeholder — episodes that use APP_LOGO will show
   a tiny dot instead of the real logo, which is fine for UI testing. */
static const uint8_t _logo_data[] = { 0x00, 0xD4, 0xFF, 0xFF };

const lv_image_dsc_t app_logo = {
    .header = {
        .w = 1,
        .h = 1,
        .cf = LV_COLOR_FORMAT_ARGB8888,
    },
    .data_size = sizeof(_logo_data),
    .data = _logo_data,
};

/* Some episodes reference APP_LOGO (uppercase) */
const lv_image_dsc_t APP_LOGO = {
    .header = {
        .w = 1,
        .h = 1,
        .cf = LV_COLOR_FORMAT_ARGB8888,
    },
    .data_size = sizeof(_logo_data),
    .data = _logo_data,
};
