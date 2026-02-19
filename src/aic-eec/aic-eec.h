/*******************************************************************************
 * File Name:   aic-eec.h
 *
 * Description: AIC-EEC Common UI Components
 *              Embedded Systems Engineering, Faculty of Engineering,
 *              Burapha University
 *
 *  Author:     Assoc. Prof. Wiroon Sriborrirux (wiroon@eng.buu.ac.th)
 * 
 * Components:
 *   - Footer/Footnote
 *   - Logo (placeholder for future)
 *   - Top note/Header (placeholder for future)
 *   - Video controls (placeholder for future)
 *   - Menu functions (placeholder for future)
 *
 * Usage:
 *   #include "aic-eec.h"
 *   aic_create_footer(lv_screen_active());
 *
 ******************************************************************************/

#ifndef AIC_EEC_H
#define AIC_EEC_H

#include "lvgl/lvgl.h"

/*******************************************************************************
 * Configuration
 ******************************************************************************/

/* Copyright text - modify as needed */
#define AIC_COPYRIGHT_TEXT   "(C) 2026 AIC-EEC.com (Embedded Systems Engineering, Burapha University)"
#define AIC_COPYRIGHT_COLOR  0x666666

/* Header text - modify as needed */
#define AIC_HEADER_TEXT      "Embedded C for IoT Course"
#define AIC_HEADER_COLOR     0xFFFFFF

/* Default positions */
#define AIC_FOOTER_Y_OFFSET  (-5)
#define AIC_HEADER_Y_OFFSET  (5)

/*******************************************************************************
 * Footer/Footnote Functions
 ******************************************************************************/

/**
 * @brief Create a footer label with copyright text
 * @param parent Parent object (usually lv_screen_active())
 * @return Pointer to the created label object
 */
lv_obj_t * aic_create_footer(lv_obj_t * parent);

/**
 * @brief Create a footer with custom text
 * @param parent Parent object
 * @param text Custom text to display
 * @param color Text color (hex)
 * @return Pointer to the created label object
 */
lv_obj_t * aic_create_footer_custom(lv_obj_t * parent, const char * text, uint32_t color);

/*******************************************************************************
 * Header/Top Note Functions
 ******************************************************************************/

/**
 * @brief Create a header label with course name
 * @param parent Parent object
 * @return Pointer to the created label object
 */
lv_obj_t * aic_create_header(lv_obj_t * parent);

/**
 * @brief Create a header with custom text
 * @param parent Parent object
 * @param text Custom text to display
 * @param color Text color (hex)
 * @return Pointer to the created label object
 */
lv_obj_t * aic_create_header_custom(lv_obj_t * parent, const char * text, uint32_t color);

/*******************************************************************************
 * Logo Functions (Placeholder for future implementation)
 ******************************************************************************/

/**
 * @brief Create university logo image
 * @param parent Parent object
 * @param align Alignment (e.g., LV_ALIGN_TOP_LEFT)
 * @param x_ofs X offset
 * @param y_ofs Y offset
 * @return Pointer to the created image object (NULL if not implemented)
 *
 * @note Requires logo image to be converted and included
 */
lv_obj_t * aic_create_logo(lv_obj_t * parent, lv_align_t align, int32_t x_ofs, int32_t y_ofs);

/*******************************************************************************
 * Menu Functions (Placeholder for future implementation)
 ******************************************************************************/

/**
 * @brief Create a navigation menu
 * @param parent Parent object
 * @return Pointer to the created menu object (NULL if not implemented)
 */
lv_obj_t * aic_create_menu(lv_obj_t * parent);

/*******************************************************************************
 * Video/Media Functions (Placeholder for future implementation)
 ******************************************************************************/

/**
 * @brief Create video playback controls
 * @param parent Parent object
 * @return Pointer to the container with controls (NULL if not implemented)
 */
lv_obj_t * aic_create_video_controls(lv_obj_t * parent);

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/

/**
 * @brief Apply AIC-EEC theme/style to screen
 * @param screen Screen object to style
 * @param bg_color Background color (hex)
 */
void aic_apply_theme(lv_obj_t * screen, uint32_t bg_color);

/**
 * @brief Create a styled container with border
 * @param parent Parent object
 * @param width Container width
 * @param height Container height
 * @return Pointer to the created container
 */
lv_obj_t * aic_create_container(lv_obj_t * parent, int32_t width, int32_t height);

#endif /* AIC_EEC_H */
