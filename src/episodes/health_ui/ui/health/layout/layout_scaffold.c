#include "layout_scaffold.h"

#include "../components/global/comp_system_status_bar.h"
#include "layout_tokens.h"

void health_layout_scaffold_create(health_layout_scaffold_t *out_scaffold) {
  lv_obj_t *screen;
  lv_obj_t *root;
  lv_obj_t *tabview;
  lv_obj_t *tabview_content;

  if (NULL == out_scaffold) {
    return;
  }

  /* Minimum edge guard (2px) prevents sub-pixel clipping on 4.3" panel. */
  #define HL_EDGE_GUARD_PX 1

  screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(screen, lv_color_hex(HL_COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(screen, HL_EDGE_GUARD_PX, LV_PART_MAIN);

  root = lv_obj_create(screen);
  lv_obj_set_size(root, lv_pct(100), lv_pct(100));
  lv_obj_set_style_bg_color(root, lv_color_hex(HL_COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(root, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(root, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(root, HL_EDGE_GUARD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(root, HL_EDGE_GUARD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(root, HL_EDGE_GUARD_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(
      root, COMP_STATUS_BAR_TOP_MARGIN_PX + COMP_STATUS_BAR_HEIGHT_PX,
      LV_PART_MAIN);
  lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);

  tabview = lv_tabview_create(root);
  lv_obj_set_size(tabview, lv_pct(100), lv_pct(100));
  lv_obj_set_style_bg_color(tabview, lv_color_hex(HL_COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(tabview, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(tabview, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(tabview, 0, LV_PART_MAIN);
  lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
  lv_tabview_set_tab_bar_size(tabview, 0);
  lv_obj_add_flag(lv_tabview_get_tab_bar(tabview), LV_OBJ_FLAG_HIDDEN);
  tabview_content = lv_tabview_get_content(tabview);
  if (NULL != tabview_content) {
    /* Keep tab changes controlled by top menu buttons; disable swipe gesture. */
    lv_obj_set_scroll_dir(tabview_content, LV_DIR_NONE);
    lv_obj_clear_flag(tabview_content, LV_OBJ_FLAG_SCROLLABLE);
  }

  out_scaffold->screen = screen;
  out_scaffold->root = root;
  out_scaffold->tabview = tabview;
}

lv_obj_t *health_layout_scaffold_add_tab(health_layout_scaffold_t *scaffold,
                                         const char *title) {
  lv_obj_t *tab;

  if ((NULL == scaffold) || (NULL == scaffold->tabview)) {
    return NULL;
  }

  tab = lv_tabview_add_tab(scaffold->tabview, title);
  lv_obj_set_style_bg_color(tab, lv_color_hex(HL_COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(tab, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(tab, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_left(tab, HL_SPACE_LG, LV_PART_MAIN);
  lv_obj_set_style_pad_right(tab, HL_SPACE_LG, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(tab, HL_SPACE_LG, LV_PART_MAIN);
  lv_obj_set_style_pad_top(tab, 0, LV_PART_MAIN);

  return tab;
}

lv_obj_t *health_layout_scaffold_create_page(lv_obj_t *tab) {
  lv_obj_t *page;

  if (NULL == tab) {
    return NULL;
  }

  page = lv_obj_create(tab);
  lv_obj_set_size(page, lv_pct(100), lv_pct(100));
  lv_obj_set_style_bg_color(page, lv_color_hex(HL_COLOR_BG), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(page, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(page, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(page, HL_EDGE_GUARD_PX, LV_PART_MAIN);
  lv_obj_set_layout(page, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(page, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_scroll_dir(page, LV_DIR_VER);

  return page;
}
