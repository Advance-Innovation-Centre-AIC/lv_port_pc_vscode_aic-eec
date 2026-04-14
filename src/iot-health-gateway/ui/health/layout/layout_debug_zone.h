#ifndef HEALTH_LAYOUT_DEBUG_ZONE_H
#define HEALTH_LAYOUT_DEBUG_ZONE_H

#include "lvgl.h"

lv_obj_t *health_layout_debug_zone_create(lv_obj_t *parent, const char *name,
                                          lv_coord_t width, lv_coord_t height,
                                          uint32_t bg_color_hex);

#endif
