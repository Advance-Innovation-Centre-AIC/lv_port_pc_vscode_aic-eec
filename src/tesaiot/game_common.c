/**
 * game_common.c — PC simulator version (touch-only, no F310 joystick)
 */
#include "game_common.h"
#include <string.h>

/* Touch state flags — set by overlay callbacks */
static volatile uint32_t s_touch_flags = 0;
#define TOUCH_UP      (1U << 0)
#define TOUCH_DOWN    (1U << 1)
#define TOUCH_LEFT    (1U << 2)
#define TOUCH_RIGHT   (1U << 3)
#define TOUCH_ACTION  (1U << 4)
#define TOUCH_RESTART (1U << 5)

void game_input_read(game_input_state_t *state)
{
    memset(state, 0, sizeof(*state));
    state->up      = (s_touch_flags & TOUCH_UP) != 0;
    state->down    = (s_touch_flags & TOUCH_DOWN) != 0;
    state->left    = (s_touch_flags & TOUCH_LEFT) != 0;
    state->right   = (s_touch_flags & TOUCH_RIGHT) != 0;
    state->action  = (s_touch_flags & TOUCH_ACTION) != 0;
    state->restart = (s_touch_flags & TOUCH_RESTART) != 0;
}

/* ── CRT Scanline Overlay ──────────────────────────────── */

void game_add_lcd_scanlines(lv_obj_t *arena, lv_coord_t w, lv_coord_t h)
{
    for (int y = 0; y < h; y += 4) {
        lv_obj_t *line = lv_obj_create(arena);
        lv_obj_remove_style_all(line);
        lv_obj_set_size(line, w, 1);
        lv_obj_set_pos(line, 0, y);
        lv_obj_set_style_bg_opa(line, LV_OPA_10, 0);
        lv_obj_set_style_bg_color(line, lv_color_black(), 0);
        lv_obj_remove_flag(line, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    }
}

/* ── Touch Overlay Helpers ─────────────────────────────── */

static void dpad_pressed_cb(lv_event_t *e)
{
    uint32_t flag = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    s_touch_flags |= flag;
}

static void dpad_released_cb(lv_event_t *e)
{
    uint32_t flag = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    s_touch_flags &= ~flag;
}

static lv_obj_t *_make_btn(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
                           lv_coord_t w, lv_coord_t h,
                           const char *sym, uint32_t flag)
{
    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_style_opa(btn, LV_OPA_30, 0);
    lv_obj_set_style_bg_color(btn, lv_color_white(), 0);
    lv_obj_add_event_cb(btn, dpad_pressed_cb, LV_EVENT_PRESSED, (void *)(uintptr_t)flag);
    lv_obj_add_event_cb(btn, dpad_released_cb, LV_EVENT_RELEASED, (void *)(uintptr_t)flag);
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, sym);
    lv_obj_center(lbl);
    return btn;
}

void game_create_touch_dpad(lv_obj_t *parent, lv_coord_t arena_x,
                            lv_coord_t arena_y, lv_coord_t arena_h,
                            bool full_dpad)
{
    const lv_coord_t sz = 50;
    const lv_coord_t gap = 4;
    lv_coord_t cx = (arena_x - sz) / 2;
    lv_coord_t cy = arena_y + (arena_h - sz * 3 - gap * 2) / 2;

    _make_btn(parent, cx, cy,               sz, sz, LV_SYMBOL_UP,    TOUCH_UP);
    _make_btn(parent, cx, cy + sz*2 + gap*2, sz, sz, LV_SYMBOL_DOWN,  TOUCH_DOWN);

    if (full_dpad) {
        _make_btn(parent, cx - sz - gap, cy + sz + gap, sz, sz, LV_SYMBOL_LEFT,  TOUCH_LEFT);
        _make_btn(parent, cx + sz + gap, cy + sz + gap, sz, sz, LV_SYMBOL_RIGHT, TOUCH_RIGHT);
    }
}

void game_create_touch_action(lv_obj_t *parent, lv_coord_t arena_x,
                              lv_coord_t arena_w, lv_coord_t arena_y,
                              lv_coord_t arena_h)
{
    lv_coord_t btn_sz = 70;
    lv_coord_t x = arena_x + arena_w + (800 - arena_x - arena_w - btn_sz) / 2;
    lv_coord_t y = arena_y + arena_h / 2 - btn_sz / 2;
    lv_obj_t *btn = _make_btn(parent, x, y, btn_sz, btn_sz, "A", TOUCH_ACTION);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
}

void game_create_touch_restart(lv_obj_t *parent)
{
    _make_btn(parent, 730, 10, 60, 36, LV_SYMBOL_REFRESH, TOUCH_RESTART);
}

void game_touch_clear(void)
{
    s_touch_flags = 0;
}
