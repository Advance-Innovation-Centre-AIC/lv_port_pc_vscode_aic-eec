#ifndef PAGE_METRIC_DETAIL_LAYOUT_H
#define PAGE_METRIC_DETAIL_LAYOUT_H

#include "../../health_ui_root.h"

#include "lvgl.h"

void page_metric_detail_layout_build(lv_obj_t *tab,
                                     health_ui_metric_detail_t metric);
void page_metric_detail_layout_set_member_index(uint32_t member_index);

#endif
