#include "metric_detail_catalog.h"

#include <stddef.h>

static const health_ui_metric_detail_def_t
    s_metric_defs[HEALTH_UI_METRIC_DETAIL_COUNT] = {
        {.metric = HEALTH_UI_METRIC_DETAIL_BP,
         .page = HEALTH_UI_PAGE_METRIC_BP_DETAIL,
         .tab_title = "BP Detail",
         .metric_title = "Blood Pressure"},
        {.metric = HEALTH_UI_METRIC_DETAIL_GLUCOSE,
         .page = HEALTH_UI_PAGE_METRIC_GLUCOSE_DETAIL,
         .tab_title = "Glucose Detail",
         .metric_title = "Blood Glucose"},
        {.metric = HEALTH_UI_METRIC_DETAIL_BODY_TEMP,
         .page = HEALTH_UI_PAGE_METRIC_BODY_TEMP_DETAIL,
         .tab_title = "Body Temp Detail",
         .metric_title = "Body Temp"},
        {.metric = HEALTH_UI_METRIC_DETAIL_SPO2,
         .page = HEALTH_UI_PAGE_METRIC_SPO2_DETAIL,
         .tab_title = "SpO2 Detail",
         .metric_title = "SpO2"},
        {.metric = HEALTH_UI_METRIC_DETAIL_WEIGHT,
         .page = HEALTH_UI_PAGE_METRIC_WEIGHT_DETAIL,
         .tab_title = "Weight Detail",
         .metric_title = "Weight"},
        {.metric = HEALTH_UI_METRIC_DETAIL_SLEEP,
         .page = HEALTH_UI_PAGE_METRIC_SLEEP_DETAIL,
         .tab_title = "Sleep Detail",
         .metric_title = "Sleep Duration"}};

bool health_ui_metric_detail_is_valid(health_ui_metric_detail_t metric) {
  return ((uint32_t)metric < (uint32_t)HEALTH_UI_METRIC_DETAIL_COUNT);
}

const health_ui_metric_detail_def_t *
health_ui_metric_detail_get_def(health_ui_metric_detail_t metric) {
  if (!health_ui_metric_detail_is_valid(metric)) {
    return NULL;
  }

  return &s_metric_defs[(uint32_t)metric];
}

health_ui_page_t health_ui_metric_detail_to_page(health_ui_metric_detail_t metric) {
  const health_ui_metric_detail_def_t *def = health_ui_metric_detail_get_def(metric);

  if (NULL == def) {
    return HEALTH_UI_PAGE_METRIC_BP_DETAIL;
  }

  return def->page;
}

const char *health_ui_metric_detail_get_tab_title(health_ui_metric_detail_t metric) {
  const health_ui_metric_detail_def_t *def = health_ui_metric_detail_get_def(metric);

  if ((NULL == def) || (NULL == def->tab_title)) {
    return "Metric Detail";
  }

  return def->tab_title;
}

const char *health_ui_metric_detail_get_title(health_ui_metric_detail_t metric) {
  const health_ui_metric_detail_def_t *def = health_ui_metric_detail_get_def(metric);

  if ((NULL == def) || (NULL == def->metric_title)) {
    return "Metric";
  }

  return def->metric_title;
}
