#ifndef PAGE_DASHBOARD_MAIN_H
#define PAGE_DASHBOARD_MAIN_H

#include "lvgl.h"
#include <stdint.h>

void page_dashboard_main_build(lv_obj_t *tab);
/* Updates simulated/runtime household member count and rebuilds member carousel. */
void page_dashboard_main_set_member_count(uint32_t member_count);
/* Updates simulated/runtime device count and rebuilds device section title/carousel. */
void page_dashboard_main_set_device_count(uint32_t device_count);

#endif
