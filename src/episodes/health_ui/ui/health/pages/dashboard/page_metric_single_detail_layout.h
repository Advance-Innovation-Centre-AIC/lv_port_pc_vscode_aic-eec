#ifndef PAGE_METRIC_SINGLE_DETAIL_LAYOUT_H
#define PAGE_METRIC_SINGLE_DETAIL_LAYOUT_H

#include "../../sim/health_ui_sim_data.h"

#include "lvgl.h"

void page_metric_single_detail_layout_build(
    lv_obj_t *parent, const health_ui_single_metric_detail_data_t *data);

#endif
