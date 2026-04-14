#ifndef PAGE_METRIC_SLEEP_DETAIL_LAYOUT_H
#define PAGE_METRIC_SLEEP_DETAIL_LAYOUT_H

#include "../../sim/health_ui_sim_data.h"

#include "lvgl.h"

void page_metric_sleep_detail_layout_build(
    lv_obj_t *parent, const health_ui_sleep_metric_detail_data_t *data);

#endif
