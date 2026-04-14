#ifndef HEALTH_UI_METRIC_DETAIL_CATALOG_H
#define HEALTH_UI_METRIC_DETAIL_CATALOG_H

#include "health_ui_root.h"

#include <stdbool.h>

typedef struct {
  health_ui_metric_detail_t metric;
  health_ui_page_t page;
  const char *tab_title;
  const char *metric_title;
} health_ui_metric_detail_def_t;

bool health_ui_metric_detail_is_valid(health_ui_metric_detail_t metric);
const health_ui_metric_detail_def_t *
health_ui_metric_detail_get_def(health_ui_metric_detail_t metric);
health_ui_page_t health_ui_metric_detail_to_page(health_ui_metric_detail_t metric);
const char *health_ui_metric_detail_get_tab_title(health_ui_metric_detail_t metric);
const char *health_ui_metric_detail_get_title(health_ui_metric_detail_t metric);
void health_ui_metric_detail_catalog_apply_i18n(void);

#endif
