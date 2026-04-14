#include "page_metric_detail_layout.h"

#include "../../components/header/comp_tab_menu.h"
#include "../../layout/layout_scaffold.h"
#include "../../metric_detail_catalog.h"
#include "../../health_ui_data_adapter.h"
#include "page_metric_bp_detail_layout.h"
#include "page_metric_single_detail_layout.h"
#include "page_metric_sleep_detail_layout.h"

#ifndef METRIC_DETAIL_SIDE_INSET_PX
#define METRIC_DETAIL_SIDE_INSET_PX (8)
#endif

#ifndef METRIC_DETAIL_ROW_GAP_PX
#define METRIC_DETAIL_ROW_GAP_PX (8)
#endif

typedef struct {
  health_ui_metric_detail_t metric;
  lv_obj_t *content_root;
} metric_page_ctx_t;

static metric_page_ctx_t s_metric_pages[HEALTH_UI_METRIC_DETAIL_COUNT] = {0};
static uint32_t s_selected_member_index = 0U;

static void on_back_clicked(lv_event_t *e) {
  (void)e;
  health_ui_root_set_active_page(HEALTH_UI_PAGE_USER_DETAIL, false);
}

static void rebuild_metric_page(metric_page_ctx_t *ctx) {
  health_ui_bp_metric_detail_data_t bp_detail;
  health_ui_single_metric_detail_data_t single_detail;
  health_ui_sleep_metric_detail_data_t sleep_detail;

  if ((NULL == ctx) || (NULL == ctx->content_root) ||
      (!lv_obj_is_valid(ctx->content_root))) {
    return;
  }

  lv_obj_clean(ctx->content_root);

  if (HEALTH_UI_METRIC_DETAIL_BP == ctx->metric) {
    health_ui_data_build_bp_metric_detail(s_selected_member_index, &bp_detail);
    page_metric_bp_detail_layout_build(ctx->content_root, &bp_detail);
    return;
  }

  if (HEALTH_UI_METRIC_DETAIL_SLEEP == ctx->metric) {
    health_ui_data_build_sleep_metric_detail(s_selected_member_index, &sleep_detail);
    page_metric_sleep_detail_layout_build(ctx->content_root, &sleep_detail);
    return;
  }

  health_ui_data_build_single_metric_detail(s_selected_member_index, ctx->metric,
                                                &single_detail);
  page_metric_single_detail_layout_build(ctx->content_root, &single_detail);
}

void page_metric_detail_layout_set_member_index(uint32_t member_index) {
  uint32_t i;

  s_selected_member_index = member_index;
  for (i = 0U; i < (uint32_t)HEALTH_UI_METRIC_DETAIL_COUNT; i++) {
    rebuild_metric_page(&s_metric_pages[i]);
  }
}

void page_metric_detail_layout_build(lv_obj_t *tab,
                                     health_ui_metric_detail_t metric) {
  lv_obj_t *page;
  lv_obj_t *content_root;
  comp_tab_menu_refs_t menu_refs;
  metric_page_ctx_t *ctx;

  if ((NULL == tab) || (!health_ui_metric_detail_is_valid(metric))) {
    return;
  }

  page = health_layout_scaffold_create_page(tab);
  if (NULL == page) {
    return;
  }
  lv_obj_set_scroll_dir(page, LV_DIR_NONE);
  lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

  (void)comp_tab_menu_create(page, COMP_TAB_MENU_MODE_METRIC_DETAIL, &menu_refs);
  if (NULL != menu_refs.btn_back) {
    lv_obj_add_event_cb(menu_refs.btn_back, on_back_clicked, LV_EVENT_CLICKED,
                        NULL);
  }

  content_root = lv_obj_create(page);
  lv_obj_set_size(content_root, lv_pct(100), 0);
  lv_obj_set_style_flex_grow(content_root, 1, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(content_root, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(content_root, METRIC_DETAIL_SIDE_INSET_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(content_root, METRIC_DETAIL_SIDE_INSET_PX,
                             LV_PART_MAIN);
  lv_obj_set_style_pad_top(content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(content_root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(content_root, METRIC_DETAIL_ROW_GAP_PX, LV_PART_MAIN);
  lv_obj_set_layout(content_root, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(content_root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(content_root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_clear_flag(content_root, LV_OBJ_FLAG_SCROLLABLE);

  ctx = &s_metric_pages[(uint32_t)metric];
  ctx->metric = metric;
  ctx->content_root = content_root;
  rebuild_metric_page(ctx);
}
