#include "page_pre_auth_overview.h"

#include "../../layout/layout_debug_zone.h"
#include "../../layout/layout_scaffold.h"
#include "../../layout/layout_tokens.h"

void page_pre_auth_overview_build(lv_obj_t *tab) {
  lv_obj_t *page;
  lv_obj_t *flow_row;

  if (NULL == tab) {
    return;
  }

  page = health_layout_scaffold_create_page(tab);
  if (NULL == page) {
    return;
  }

  (void)health_layout_debug_zone_create(page, "PRE-AUTH_HEADER", lv_pct(100), 64,
                                        0xDCEBFF);

  flow_row =
      health_layout_debug_zone_create(page, "PRE-AUTH_FLOW", lv_pct(100), 220,
                                      0xE9F2FF);
  if (NULL != flow_row) {
    lv_obj_t *inner = lv_obj_create(flow_row);
    lv_obj_set_size(inner, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(inner, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(inner, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(inner, HL_SPACE_SM, LV_PART_MAIN);
    lv_obj_set_layout(inner, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(inner, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_column(inner, HL_SPACE_SM, LV_PART_MAIN);
    lv_obj_set_style_pad_row(inner, HL_SPACE_SM, LV_PART_MAIN);
    lv_obj_clear_flag(inner, LV_OBJ_FLAG_SCROLLABLE);

    (void)health_layout_debug_zone_create(inner, "SPLASH", lv_pct(48), 80,
                                          0xFFF0CC);
    (void)health_layout_debug_zone_create(inner, "LOCK_SCREEN", lv_pct(48), 80,
                                          0xE7F9E8);
    (void)health_layout_debug_zone_create(inner, "PIN_ENTRY", lv_pct(48), 80,
                                          0xFFE7D9);
    (void)health_layout_debug_zone_create(inner, "AUTH_LOADING", lv_pct(48), 80,
                                          0xEFE8FF);
  }

  (void)health_layout_debug_zone_create(page, "PRE-AUTH_FOOTER_NOTE", lv_pct(100),
                                        80, 0xF4F7FB);
}
