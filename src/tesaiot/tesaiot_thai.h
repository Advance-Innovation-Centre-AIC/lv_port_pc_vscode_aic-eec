/*******************************************************************************
 * @file    tesaiot_thai.h
 * @brief   Thai language rendering helpers — PC simulator version
 *
 *  PC-compatible drop-in replacement for the firmware tesaiot_thai.h.
 *  Maps to the Thai fonts already compiled in the simulator (src/fonts/).
 ******************************************************************************/
#ifndef TESAIOT_THAI_H
#define TESAIOT_THAI_H

#include "lvgl.h"
#include "../fonts/lv_fonts_thai.h"   /* extern const lv_font_t lv_font_noto_thai_{14,16,20,28} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a label that renders Thai text using Noto Sans Thai font.
 * Supports Thai + English + ASCII symbols in a single string.
 *
 * @param parent  LVGL parent object
 * @param text    UTF-8 encoded string (may contain Thai characters)
 * @param size    Font size: 14, 16, 20, or 28 px (others fall back to 14)
 * @param color   Text color
 * @return        Pointer to the created label object
 */
static inline lv_obj_t *tesaiot_thai_label(lv_obj_t *parent, const char *text,
                                           int size, lv_color_t color)
{
    const lv_font_t *font;
    switch (size) {
        case 28: font = &lv_font_noto_thai_28; break;
        case 20: font = &lv_font_noto_thai_20; break;
        case 16: font = &lv_font_noto_thai_16; break;
        default: font = &lv_font_noto_thai_14; break;
    }
    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, text);
    lv_obj_set_style_text_font(lbl, font, 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    return lbl;
}

/**
 * Add a persistent "รองรับภาษาไทย" badge to the LVGL top layer.
 */
static inline void tesaiot_add_thai_support_badge(void)
{
    lv_obj_t *badge = lv_label_create(lv_layer_top());
    lv_label_set_text(badge, "\xe0\xb8\xa3\xe0\xb8\xad\xe0\xb8\x87\xe0\xb8\xa3\xe0\xb8\xb1\xe0\xb8\x9a\xe0\xb8\xa0\xe0\xb8\xb2\xe0\xb8\xa9\xe0\xb8\xb2\xe0\xb9\x84\xe0\xb8\x97\xe0\xb8\xa2");  /* รองรับภาษาไทย */
    lv_obj_set_style_text_font(badge, &lv_font_noto_thai_14, 0);
    lv_obj_set_style_text_color(badge, lv_color_hex(0xE0F7FA), 0);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0x142240), 0);
    lv_obj_set_style_bg_opa(badge, LV_OPA_80, 0);
    lv_obj_set_style_pad_top(badge, 4, 0);
    lv_obj_set_style_pad_bottom(badge, 4, 0);
    lv_obj_set_style_pad_left(badge, 10, 0);
    lv_obj_set_style_pad_right(badge, 10, 0);
    lv_obj_set_style_radius(badge, 6, 0);
    lv_obj_set_style_border_color(badge, lv_color_hex(0x00BCD4), 0);
    lv_obj_set_style_border_width(badge, 1, 0);
    lv_obj_align(badge, LV_ALIGN_TOP_RIGHT, -8, 8);
}

#ifdef __cplusplus
}
#endif

#endif /* TESAIOT_THAI_H */
