#ifndef COMP_STATUS_CHIP_H
#define COMP_STATUS_CHIP_H

#include "lvgl.h"

lv_obj_t *comp_status_chip_create(lv_obj_t *parent, const char *text,
                                  uint32_t bg_color, uint32_t fg_color);

#endif
