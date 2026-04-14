/*******************************************************************************
 * layout_i18n.c — Health UI internationalization runtime
 *
 * Parallel const arrays in ROM. Runtime pointer switch — O(1), zero-copy.
 ******************************************************************************/

#include "layout_i18n.h"
#include "lvgl.h"
#include "lv_fonts_thai.h"
#include <stddef.h>

/* String tables (included as static const arrays) */
#include "layout_i18n_en.inc"
#include "layout_i18n_th.inc"

/* Active string table pointer (4 bytes RAM) */
static const char *const *s_active = s_strings_en;
static health_ui_locale_t s_locale = HEALTH_UI_LOCALE_EN;

/*******************************************************************************
 * Font fallback: Montserrat → NotoSansThai
 *
 * Built-in LVGL fonts are const (in flash/ROM) — cannot modify fallback field.
 * Instead, create mutable RAM copies with fallback chain set up.
 * These shadow fonts are used as HL_FONT_* in layout_fonts.h.
 ******************************************************************************/

/* Mutable font copies in RAM (~40 bytes each × 7 = 280 bytes) */
static lv_font_t s_font_12_thai;
static lv_font_t s_font_14_thai;
static lv_font_t s_font_16_thai;
static lv_font_t s_font_20_thai;
static lv_font_t s_font_24_thai;
static lv_font_t s_font_28_thai;
static lv_font_t s_font_36_thai;

/* Public pointers for layout_fonts.h to reference */
const lv_font_t *hl_font_12 = &lv_font_montserrat_12;
const lv_font_t *hl_font_14 = &lv_font_montserrat_14;
const lv_font_t *hl_font_16 = &lv_font_montserrat_16;
const lv_font_t *hl_font_20 = &lv_font_montserrat_20;
const lv_font_t *hl_font_24 = &lv_font_montserrat_24;
const lv_font_t *hl_font_28 = &lv_font_montserrat_28;
const lv_font_t *hl_font_36 = &lv_font_montserrat_36;

static void setup_thai_font_fallback(void) {
    /* Copy Montserrat structs to RAM, then set fallback to NotoSansThai */
    s_font_12_thai = lv_font_montserrat_12;
    s_font_12_thai.fallback = &lv_font_noto_thai_14;
    hl_font_12 = &s_font_12_thai;

    s_font_14_thai = lv_font_montserrat_14;
    s_font_14_thai.fallback = &lv_font_noto_thai_14;
    hl_font_14 = &s_font_14_thai;

    s_font_16_thai = lv_font_montserrat_16;
    s_font_16_thai.fallback = &lv_font_noto_thai_16;
    hl_font_16 = &s_font_16_thai;

    s_font_20_thai = lv_font_montserrat_20;
    s_font_20_thai.fallback = &lv_font_noto_thai_20;
    hl_font_20 = &s_font_20_thai;

    s_font_24_thai = lv_font_montserrat_24;
    s_font_24_thai.fallback = &lv_font_noto_thai_24;
    hl_font_24 = &s_font_24_thai;

    s_font_28_thai = lv_font_montserrat_28;
    s_font_28_thai.fallback = &lv_font_noto_thai_28;
    hl_font_28 = &s_font_28_thai;

    s_font_36_thai = lv_font_montserrat_36;
    s_font_36_thai.fallback = &lv_font_noto_thai_28;
    hl_font_36 = &s_font_36_thai;
}

void health_ui_i18n_init(health_ui_locale_t locale) {
    setup_thai_font_fallback();
    health_ui_i18n_set_locale(locale);
}

void health_ui_i18n_set_locale(health_ui_locale_t locale) {
    s_locale = locale;
    s_active = (locale == HEALTH_UI_LOCALE_TH) ? s_strings_th : s_strings_en;
}

health_ui_locale_t health_ui_i18n_get_locale(void) {
    return s_locale;
}

const char *health_ui_i18n_get(health_ui_str_t id) {
    if ((uint32_t)id >= (uint32_t)STR_COUNT) {
        return "?";
    }
    const char *str = s_active[id];
    return (str != NULL) ? str : "?";
}
