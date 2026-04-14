#include "page_dashboard_main.h"

#include "../../components/content/comp_device_card.h"
#include "../../components/content/comp_pager_dots.h"
#include "../../components/content/comp_user_card.h"
#include "../../components/header/comp_tab_menu.h"
#include "../../components/overlay/comp_add_device_flow_modal.h"
#include "../../health_ui_root.h"
#include "../../layout/layout_scaffold.h"
#include "../../layout/layout_fonts.h"
#include "../../layout/layout_tokens.h"
#include "../../health_ui_data_adapter.h"
#include "../../health_ui_display_policy.h"

#include <limits.h>
#include <stdio.h>

/* Dashboard layout tuning knobs (override by -D compile flags if needed). */
#ifndef DASH_MEMBER_PER_PAGE
#if (MY_DISP_HOR_RES >= 1024)
#define DASH_MEMBER_PER_PAGE (4U)
#else
#define DASH_MEMBER_PER_PAGE (3U)
#endif
#endif

#ifndef DASH_MEMBER_SECTION_H_PX
#define DASH_MEMBER_SECTION_H_PX HL_SCALE_V(296)
#endif

#ifndef DASH_MEMBER_CAROUSEL_H_PX
#define DASH_MEMBER_CAROUSEL_H_PX HL_SCALE_V(256)
#endif

#ifndef DASH_MEMBER_NAME_BUF_SIZE
#define DASH_MEMBER_NAME_BUF_SIZE (64U)
#endif

#ifndef DASH_DEVICE_PER_PAGE
#define DASH_DEVICE_PER_PAGE (4U)
#endif

#ifndef DASH_DEVICE_DEFAULT_COUNT
#define DASH_DEVICE_DEFAULT_COUNT (3U)
#endif

#ifndef DASH_DEVICE_SECTION_H_PX
#define DASH_DEVICE_SECTION_H_PX HL_SCALE_V(180)
#endif

#ifndef DASH_DEVICE_HEADER_H_PX
#define DASH_DEVICE_HEADER_H_PX HL_SCALE_V(38)
#endif

#ifndef DASH_DEVICE_PANEL_H_PX
#define DASH_DEVICE_PANEL_H_PX HL_SCALE_V(108)
#endif

#ifndef DASH_DEVICE_PANEL_RADIUS_PX
#define DASH_DEVICE_PANEL_RADIUS_PX (16)
#endif

#ifndef DASH_DEVICE_PANEL_BG_COLOR_HEX
#define DASH_DEVICE_PANEL_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef DASH_DEVICE_PAGE_CARD_GAP_PX
#define DASH_DEVICE_PAGE_CARD_GAP_PX (8)
#endif

#ifndef DASH_SECTION_SIDE_INSET_PX
/* Shared side inset to align section content with menu/user-card visual baseline. */
#define DASH_SECTION_SIDE_INSET_PX (HL_SPACE_SM)
#endif

#ifndef DASH_DEVICE_PANEL_INNER_PAD_PX
/* Small inner inset keeps first/last device card off panel edges. */
#define DASH_DEVICE_PANEL_INNER_PAD_PX (HL_SPACE_SM)
#endif

#ifndef DASH_DEVICE_TITLE_TEXT_COLOR_HEX
#define DASH_DEVICE_TITLE_TEXT_COLOR_HEX (0x002329)
#endif

#ifndef DASH_DEVICE_TITLE_FONT
#define DASH_DEVICE_TITLE_FONT HL_FONT_HEADING
#endif

#ifndef DASH_DEVICE_TITLE_BUF_SIZE
#define DASH_DEVICE_TITLE_BUF_SIZE (32U)
#endif

#ifndef DASH_DEVICE_ADD_BTN_SIZE_PX
#define DASH_DEVICE_ADD_BTN_SIZE_PX (40)
#endif

#ifndef DASH_DEVICE_ADD_BTN_RADIUS_PX
#define DASH_DEVICE_ADD_BTN_RADIUS_PX (20)
#endif

#ifndef DASH_DEVICE_ADD_BTN_BG_COLOR_HEX
#define DASH_DEVICE_ADD_BTN_BG_COLOR_HEX (0xF2F3F5)
#endif

#ifndef DASH_DEVICE_ADD_ICON_COLOR_HEX
#define DASH_DEVICE_ADD_ICON_COLOR_HEX (0x667085)
#endif

#ifndef DASH_DEVICE_ADD_ICON_FONT
#define DASH_DEVICE_ADD_ICON_FONT HL_FONT_ICON_LG
#endif

static lv_obj_t *s_member_carousel = NULL;
static lv_obj_t *s_member_dots = NULL;
static uint32_t s_member_count = 6U;
static uint32_t s_member_page_count = 0U;
static uint32_t s_member_dot_count = 0U;
static uint32_t s_member_active_page = 0U;
static lv_obj_t *s_device_title_label = NULL;
static lv_obj_t *s_device_carousel = NULL;
static lv_obj_t *s_device_dots = NULL;
/* Runtime count driving both header title and card generation; sourced from sim in layout phase. */
static uint32_t s_device_count = DASH_DEVICE_DEFAULT_COUNT;
static uint32_t s_device_page_count = 0U;
static uint32_t s_device_dot_count = 0U;
static uint32_t s_device_active_page = 0U;
static char s_device_title_text[DASH_DEVICE_TITLE_BUF_SIZE] = "Devices: 0";

typedef struct {
  char user_name[DASH_MEMBER_NAME_BUF_SIZE];
  /* Carry profile fields from dashboard card to user-detail header. */
  uint32_t member_index;
  uint32_t age_years;
  uint32_t avatar_bg_color_hex;
} member_card_ctx_t;

static void copy_member_profile_to_ctx(member_card_ctx_t *ctx,
                                       uint32_t member_index,
                                       const comp_user_card_data_t *member_data) {
  if ((NULL == ctx) || (NULL == member_data)) {
    return;
  }

  ctx->member_index = member_index;
  (void)snprintf(ctx->user_name, sizeof(ctx->user_name), "%s",
                 (NULL != member_data->name) ? member_data->name : "Member");
  ctx->age_years = member_data->age_years;
  ctx->avatar_bg_color_hex = member_data->avatar_bg_color_hex;
}

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

static void on_device_add_clicked(lv_event_t *e) {
  (void)e;
  comp_add_device_flow_modal_open();
}

static void style_transparent_container(lv_obj_t *obj) {
  if (NULL == obj) {
    return;
  }

  lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);
}

static lv_obj_t *create_carousel_page(lv_obj_t *carousel, lv_coord_t col_gap,
                                      lv_flex_align_t main_align,
                                      lv_flex_align_t cross_align) {
  lv_obj_t *page;

  if (NULL == carousel) {
    return NULL;
  }

  page = lv_obj_create(carousel);
  lv_obj_set_size(page, lv_pct(100), lv_pct(100));
  style_transparent_container(page);
  lv_obj_set_style_pad_all(page, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(page, col_gap, LV_PART_MAIN);
  lv_obj_set_layout(page, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(page, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(page, main_align, cross_align, cross_align);
  lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

  return page;
}

static void setup_horizontal_carousel(lv_obj_t *carousel, lv_event_cb_t on_scrolled) {
  if (NULL == carousel) {
    return;
  }

  style_transparent_container(carousel);
  lv_obj_set_style_pad_all(carousel, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(carousel, 0, LV_PART_MAIN);
  lv_obj_set_layout(carousel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(carousel, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(carousel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_scroll_dir(carousel, LV_DIR_HOR);
  lv_obj_set_scroll_snap_x(carousel, LV_SCROLL_SNAP_START);
  lv_obj_set_scroll_snap_y(carousel, LV_SCROLL_SNAP_NONE);
  lv_obj_set_scrollbar_mode(carousel, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_flag(carousel, LV_OBJ_FLAG_SCROLL_ONE);
  lv_obj_add_event_cb(carousel, on_scrolled, LV_EVENT_SCROLL, NULL);
  lv_obj_add_event_cb(carousel, on_scrolled, LV_EVENT_SCROLL_END, NULL);
}

static lv_coord_t calc_full_page_card_width(lv_obj_t *page, uint32_t per_page,
                                            lv_coord_t gap_px) {
  lv_coord_t content_width;
  lv_coord_t gaps_total;

  if ((NULL == page) || (per_page == 0U)) {
    return 0;
  }

  lv_obj_update_layout(page);
  content_width = lv_obj_get_content_width(page);
  if (content_width <= 0) {
    return 0;
  }

  gaps_total = (lv_coord_t)((per_page - 1U) * (uint32_t)gap_px);
  if (content_width <= gaps_total) {
    return 0;
  }

  return (content_width - gaps_total) / (lv_coord_t)per_page;
}

static void add_event_bubble_recursive(lv_obj_t *obj) {
  uint32_t i;
  uint32_t child_count;

  if (NULL == obj) {
    return;
  }

  child_count = lv_obj_get_child_count(obj);
  for (i = 0U; i < child_count; i++) {
    lv_obj_t *child = lv_obj_get_child(obj, (int32_t)i);
    if (NULL == child) {
      continue;
    }
    lv_obj_add_flag(child, LV_OBJ_FLAG_EVENT_BUBBLE);
    add_event_bubble_recursive(child);
  }
}

static void on_member_card_deleted(lv_event_t *e) {
  member_card_ctx_t *ctx = (member_card_ctx_t *)lv_event_get_user_data(e);
  if (NULL != ctx) {
    lv_free(ctx);
  }
}

static void on_member_card_clicked(lv_event_t *e) {
  member_card_ctx_t *ctx = (member_card_ctx_t *)lv_event_get_user_data(e);
  const char *user_name = "Member";
  uint32_t member_index = 0U;
  uint32_t age_years = 0U;
  uint32_t avatar_bg_color_hex = 0U;

  if ((NULL != ctx) && (ctx->user_name[0] != '\0')) {
    member_index = ctx->member_index;
    user_name = ctx->user_name;
    age_years = ctx->age_years;
    avatar_bg_color_hex = ctx->avatar_bg_color_hex;
  }

  health_ui_root_set_selected_member_index(member_index);
  health_ui_root_set_selected_user_profile(user_name, age_years,
                                           avatar_bg_color_hex);
  health_ui_root_set_active_page(HEALTH_UI_PAGE_USER_DETAIL, false);
}

static uint32_t calc_page_count(uint32_t item_count, uint32_t per_page) {
  if ((0U == per_page) || (0U == item_count)) {
    return 1U;
  }

  return (item_count + (per_page - 1U)) / per_page;
}

static void sync_dots(lv_obj_t *dots, uint32_t page_count, uint32_t active_page,
                      uint32_t *dot_count_cache,
                      uint32_t *active_page_cache) {
  if ((NULL == dots) || (NULL == dot_count_cache) || (NULL == active_page_cache)) {
    return;
  }

  if (active_page >= page_count) {
    active_page = (page_count > 0U) ? (page_count - 1U) : 0U;
  }

  if (*dot_count_cache != page_count) {
    comp_pager_dots_build(dots, page_count, active_page);
    *dot_count_cache = page_count;
    *active_page_cache = active_page;
    return;
  }

  if (*active_page_cache != active_page) {
    comp_pager_dots_set_active(dots, active_page);
    *active_page_cache = active_page;
  }
}

static uint32_t get_active_page_from_scroll(lv_obj_t *carousel,
                                            uint32_t page_count) {
  uint32_t i;
  uint32_t nearest_idx = 0U;
  lv_area_t carousel_coords;
  int32_t viewport_center_x;
  int32_t nearest_distance = INT_MAX;

  if ((NULL == carousel) || (0U == page_count)) {
    return 0U;
  }

  lv_obj_get_coords(carousel, &carousel_coords);
  viewport_center_x =
      carousel_coords.x1 + ((carousel_coords.x2 - carousel_coords.x1) / 2);

  for (i = 0U; i < page_count; i++) {
    lv_obj_t *page = lv_obj_get_child(carousel, (int32_t)i);
    lv_area_t page_coords;
    int32_t page_center_x;
    int32_t distance;
    int64_t diff;

    if (NULL == page) {
      continue;
    }

    lv_obj_get_coords(page, &page_coords);
    page_center_x = page_coords.x1 + ((page_coords.x2 - page_coords.x1) / 2);
    diff = (int64_t)page_center_x - (int64_t)viewport_center_x;
    if (diff < 0) {
      diff = -diff;
    }

    if (diff > (int64_t)INT_MAX) {
      distance = INT_MAX;
    } else {
      distance = (int32_t)diff;
    }

    if (distance < nearest_distance) {
      nearest_distance = distance;
      nearest_idx = i;
    }
  }

  return nearest_idx;
}

static void sync_member_dots(uint32_t active_page) {
  sync_dots(s_member_dots, s_member_page_count, active_page, &s_member_dot_count,
            &s_member_active_page);
}

static void sync_device_dots(uint32_t active_page) {
  sync_dots(s_device_dots, s_device_page_count, active_page, &s_device_dot_count,
            &s_device_active_page);
}

static void update_device_title(void) {
  if (NULL == s_device_title_label) {
    return;
  }

  (void)snprintf(s_device_title_text, sizeof(s_device_title_text), "Devices: %lu",
                 (unsigned long)s_device_count);
  lv_label_set_text(s_device_title_label, s_device_title_text);
}

static void rebuild_member_carousel(void) {
  uint32_t page_idx;
  uint32_t member_idx;

  if ((NULL == s_member_carousel) || (NULL == s_member_dots)) {
    return;
  }

  s_member_page_count = calc_page_count(s_member_count, DASH_MEMBER_PER_PAGE);

  /* Rebuild pages from scratch so member-count changes are deterministic. */
  lv_obj_clean(s_member_carousel);
  for (page_idx = 0U; page_idx < s_member_page_count; page_idx++) {
    uint32_t start_idx = page_idx * DASH_MEMBER_PER_PAGE;
    uint32_t end_idx = start_idx + DASH_MEMBER_PER_PAGE;
    lv_obj_t *page = create_carousel_page(
        s_member_carousel, 0, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START);

    if (end_idx > s_member_count) {
      end_idx = s_member_count;
    }
    if (NULL == page) {
      continue;
    }

    for (member_idx = start_idx; member_idx < end_idx; member_idx++) {
      comp_user_card_data_t member_data;
      lv_obj_t *card;
      member_card_ctx_t *ctx;
      /* Layout phase: feed deterministic mock data until IPC binding is enabled. */
      health_ui_data_build_member_card(member_idx, &member_data);

      card = comp_user_card_create(page, &member_data);
      if (NULL == card) {
        continue;
      }

      add_event_bubble_recursive(card);

      ctx = (member_card_ctx_t *)lv_malloc(sizeof(*ctx));
      if (NULL == ctx) {
        continue;
      }

      copy_member_profile_to_ctx(ctx, member_idx, &member_data);
      lv_obj_add_event_cb(card, on_member_card_clicked, LV_EVENT_CLICKED, ctx);
      lv_obj_add_event_cb(card, on_member_card_deleted, LV_EVENT_DELETE, ctx);
    }
  }

  lv_obj_scroll_to_x(s_member_carousel, 0, LV_ANIM_OFF);
  s_member_dot_count = 0U;
  sync_member_dots(0U);
}

static void on_member_carousel_scrolled(lv_event_t *e) {
  uint32_t active_page;
  lv_obj_t *carousel = lv_event_get_current_target(e);

  if ((NULL == carousel) || (0U == s_member_page_count)) {
    return;
  }

  active_page = get_active_page_from_scroll(carousel, s_member_page_count);
  sync_member_dots(active_page);
}

static void rebuild_device_carousel(void) {
  uint32_t page_idx;
  uint32_t device_idx;

  if ((NULL == s_device_carousel) || (NULL == s_device_dots)) {
    return;
  }

  s_device_page_count = calc_page_count(s_device_count, DASH_DEVICE_PER_PAGE);

  lv_obj_clean(s_device_carousel);
  for (page_idx = 0U; page_idx < s_device_page_count; page_idx++) {
    uint32_t start_idx = page_idx * DASH_DEVICE_PER_PAGE;
    uint32_t end_idx = start_idx + DASH_DEVICE_PER_PAGE;
    lv_coord_t card_width_px;
    lv_obj_t *page = create_carousel_page(
        s_device_carousel, DASH_DEVICE_PAGE_CARD_GAP_PX, LV_FLEX_ALIGN_START,
        LV_FLEX_ALIGN_CENTER);

    if (end_idx > s_device_count) {
      end_idx = s_device_count;
    }
    if (NULL == page) {
      continue;
    }

    /* Runtime width keeps 4 cards balanced across supported display profiles. */
    card_width_px = calc_full_page_card_width(page, DASH_DEVICE_PER_PAGE,
                                              DASH_DEVICE_PAGE_CARD_GAP_PX);

    for (device_idx = start_idx; device_idx < end_idx; device_idx++) {
      comp_device_card_data_t device_data;
      lv_obj_t *card;
      health_ui_data_build_device_card(device_idx, &device_data);
      card = comp_device_card_create(page, &device_data);
      if ((NULL != card) && (card_width_px > 0)) {
        lv_obj_set_width(card, card_width_px);
      }
    }
  }

  lv_obj_scroll_to_x(s_device_carousel, 0, LV_ANIM_OFF);
  s_device_dot_count = 0U;
  sync_device_dots(0U);
}

static void on_device_carousel_scrolled(lv_event_t *e) {
  uint32_t active_page;
  lv_obj_t *carousel = lv_event_get_current_target(e);

  if ((NULL == carousel) || (0U == s_device_page_count)) {
    return;
  }

  active_page = get_active_page_from_scroll(carousel, s_device_page_count);
  sync_device_dots(active_page);
}

static void build_device_section(lv_obj_t *parent) {
  lv_obj_t *device_section;
  lv_obj_t *header_row;
  lv_obj_t *add_btn;
  lv_obj_t *add_label;
  lv_obj_t *panel;

  /* In layout/sim mode, device section is rendered from sim inventory source. */
  s_device_count = health_ui_data_get_device_count();

  device_section = lv_obj_create(parent);
  lv_obj_set_size(device_section, lv_pct(100), DASH_DEVICE_SECTION_H_PX);
  style_transparent_container(device_section);
  lv_obj_set_style_pad_left(device_section, DASH_SECTION_SIDE_INSET_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_right(device_section, DASH_SECTION_SIDE_INSET_PX, LV_PART_MAIN);
  lv_obj_set_style_pad_top(device_section, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_bottom(device_section, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(device_section, HL_SPACE_SM, LV_PART_MAIN);
  lv_obj_set_layout(device_section, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(device_section, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(device_section, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(device_section, LV_OBJ_FLAG_SCROLLABLE);

  header_row = lv_obj_create(device_section);
  lv_obj_set_size(header_row, lv_pct(100), DASH_DEVICE_HEADER_H_PX);
  style_transparent_container(header_row);
  lv_obj_set_style_pad_all(header_row, 0, LV_PART_MAIN);
  lv_obj_set_layout(header_row, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(header_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(header_row, LV_FLEX_ALIGN_SPACE_BETWEEN,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(header_row, LV_OBJ_FLAG_SCROLLABLE);

  s_device_title_label = lv_label_create(header_row);
  lv_label_set_text(s_device_title_label, s_device_title_text);
  lv_obj_set_style_text_color(s_device_title_label,
                              lv_color_hex(DASH_DEVICE_TITLE_TEXT_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(s_device_title_label, DASH_DEVICE_TITLE_FONT,
                             LV_PART_MAIN);

  add_btn = lv_btn_create(header_row);
  lv_obj_set_size(add_btn, DASH_DEVICE_ADD_BTN_SIZE_PX, DASH_DEVICE_ADD_BTN_SIZE_PX);
  lv_obj_set_style_radius(add_btn, DASH_DEVICE_ADD_BTN_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_bg_color(add_btn, lv_color_hex(DASH_DEVICE_ADD_BTN_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(add_btn, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_border_width(add_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_width(add_btn, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(add_btn, 0, LV_PART_MAIN);
  lv_obj_clear_flag(add_btn, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(add_btn, on_device_add_clicked, LV_EVENT_CLICKED, NULL);

  add_label = lv_label_create(add_btn);
  lv_label_set_text(add_label, "+");
  lv_obj_set_style_text_color(add_label, lv_color_hex(DASH_DEVICE_ADD_ICON_COLOR_HEX),
                              LV_PART_MAIN);
  lv_obj_set_style_text_font(add_label, DASH_DEVICE_ADD_ICON_FONT, LV_PART_MAIN);
  lv_obj_center(add_label);

  panel = lv_obj_create(device_section);
  lv_obj_set_size(panel, lv_pct(100), DASH_DEVICE_PANEL_H_PX);
  lv_obj_set_style_bg_color(panel, lv_color_hex(DASH_DEVICE_PANEL_BG_COLOR_HEX),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_radius(panel, DASH_DEVICE_PANEL_RADIUS_PX, LV_PART_MAIN);
  lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
  /* Keep left/right edge alignment consistent with member-card zone. */
  lv_obj_set_style_pad_all(panel, DASH_DEVICE_PANEL_INNER_PAD_PX, LV_PART_MAIN);
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  s_device_carousel = lv_obj_create(panel);
  lv_obj_set_size(s_device_carousel, lv_pct(100), lv_pct(100));
  setup_horizontal_carousel(s_device_carousel, on_device_carousel_scrolled);

  s_device_dots = comp_pager_dots_create(device_section);
  update_device_title();
  rebuild_device_carousel();
}

void page_dashboard_main_build(lv_obj_t *tab) {
  lv_obj_t *page;
  lv_obj_t *member_section;
  comp_tab_menu_refs_t menu_refs;

  if (NULL == tab) {
    return;
  }

  page = health_layout_scaffold_create_page(tab);
  if (NULL == page) {
    return;
  }
  /* Dashboard fits within viewport — disable scroll to prevent drag. */
  lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

  (void)comp_tab_menu_create(page, COMP_TAB_MENU_MODE_MAIN_HEALTH, &menu_refs);
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

  member_section = lv_obj_create(page);
  lv_obj_set_size(member_section, lv_pct(100), DASH_MEMBER_SECTION_H_PX);
  style_transparent_container(member_section);
  lv_obj_set_style_pad_all(member_section, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(member_section, HL_SPACE_MD, LV_PART_MAIN);
  lv_obj_set_layout(member_section, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(member_section, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(member_section, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(member_section, LV_OBJ_FLAG_SCROLLABLE);

  s_member_carousel = lv_obj_create(member_section);
  lv_obj_set_size(s_member_carousel, lv_pct(100), DASH_MEMBER_CAROUSEL_H_PX);
  setup_horizontal_carousel(s_member_carousel, on_member_carousel_scrolled);

  s_member_dots = comp_pager_dots_create(member_section);

  rebuild_member_carousel();
  build_device_section(page);
}

void page_dashboard_main_set_member_count(uint32_t member_count) {
  s_member_count = member_count;
  rebuild_member_carousel();
}

void page_dashboard_main_set_device_count(uint32_t device_count) {
  uint32_t available_count = health_ui_data_get_device_count();
  s_device_count = (device_count < available_count) ? device_count : available_count;
  update_device_title();
  rebuild_device_carousel();
}
