/*******************************************************************************
 * File Name:   part3_scope_example.h
 *
 * Description: Part 3 - Custom Panel Scope Application (Simulated)
 *              Embedded C for IoT Course
 *
 * This file implements the AIC-EEC Scope application using custom panels
 * instead of TabView for maximum chart display area.
 *
 * =============================================================================
 * LVGL 9.2 Widget References
 * =============================================================================
 *   Chart:         https://docs.lvgl.io/9.2/widgets/chart.html
 *                  - Line Chart (waveform), Bar Chart (FFT)
 *   Dropdown:      https://docs.lvgl.io/9.2/widgets/dropdown.html
 *   Slider:        https://docs.lvgl.io/9.2/widgets/slider.html
 *   LED:           https://docs.lvgl.io/9.2/widgets/led.html
 *   Button:        https://docs.lvgl.io/9.2/widgets/button.html
 *   Label:         https://docs.lvgl.io/9.2/widgets/label.html
 *   Timer:         https://docs.lvgl.io/9.2/others/timer.html
 *
 * Dependencies:
 *   - aic-eec/aic-eec.h (footer, theme)
 *   - aic-eec/scope.h   (waveform, FFT, measurements)
 *
 ******************************************************************************/

#ifndef PART3_SCOPE_EXAMPLE_H
#define PART3_SCOPE_EXAMPLE_H

#include "lvgl/lvgl.h"

/*******************************************************************************
 * Example Function Declarations
 ******************************************************************************/

/**
 * @brief Example 7: Custom Panel Scope (Maximum Chart Size - Simulated)
 *
 * Complete oscilloscope application using custom panels instead of TabView
 * to maximize chart display area.
 *
 * Widgets: Button (nav), Container (panels), Chart, Dropdown, Slider, LED
 */
void part3_ex7_custom_panel_scope(void);

#endif /* PART3_SCOPE_EXAMPLE_H */
