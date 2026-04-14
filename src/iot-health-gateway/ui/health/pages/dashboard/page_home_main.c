#include "page_home_main.h"

#include "../../components/header/comp_tab_menu.h"
#include "../../health_ui_root.h"
#include "../../layout/layout_fonts.h"
#include "../../layout/layout_scaffold.h"

static void on_health_clicked(lv_event_t *e) {
  (void)e;
  health_ui_root_set_active_page(HEALTH_UI_PAGE_HEALTH, false);
}

static void on_home_clicked(lv_event_t *e) {
  (void)e;
  health_ui_root_set_active_page(HEALTH_UI_PAGE_HOME, false);
}

static void on_setting_clicked(lv_event_t *e) {
  (void)e;
  health_ui_root_set_active_page(HEALTH_UI_PAGE_SETTING, false);
}

void page_home_main_build(lv_obj_t *tab) {
  lv_obj_t *page;
  lv_obj_t *placeholder;
  comp_tab_menu_refs_t menu_refs;

  if (NULL == tab) {
    return;
  }

  page = health_layout_scaffold_create_page(tab);
  if (NULL == page) {
    return;
  }

  (void)comp_tab_menu_create(page, COMP_TAB_MENU_MODE_MAIN_HOME, &menu_refs);
  if (NULL != menu_refs.btn_health) {
    lv_obj_add_event_cb(menu_refs.btn_health, on_health_clicked, LV_EVENT_CLICKED,
                        NULL);
  }
  if (NULL != menu_refs.btn_home) {
    lv_obj_add_event_cb(menu_refs.btn_home, on_home_clicked, LV_EVENT_CLICKED,
                        NULL);
  }
  if (NULL != menu_refs.btn_setting) {
    lv_obj_add_event_cb(menu_refs.btn_setting, on_setting_clicked,
                        LV_EVENT_CLICKED, NULL);
  }

  placeholder = lv_label_create(page);
  lv_label_set_text(placeholder, "Home Page");
  lv_obj_set_style_text_color(placeholder, lv_color_hex(0x6B7280), LV_PART_MAIN);
  lv_obj_set_style_text_font(placeholder, HL_FONT_HEADING, LV_PART_MAIN);
  lv_obj_align(placeholder, LV_ALIGN_CENTER, 0, 0);
}
