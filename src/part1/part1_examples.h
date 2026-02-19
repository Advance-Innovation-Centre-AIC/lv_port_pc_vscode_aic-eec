/*******************************************************************************
 * File Name:   part1_examples.h
 *
 * Description: Header file for Part 1 LVGL examples - GPIO Integration
 *              Part of Embedded C for IoT Course
 *
 * Examples:
 *   1. Hello World Label
 *   2. Button with Event
 *   3. LED Widget Control
 *   4. Switch for GPIO
 *   5. GPIO Dashboard
 *
 ******************************************************************************/

#ifndef PART1_EXAMPLES_H
#define PART1_EXAMPLES_H

#include "lvgl/lvgl.h"

/*******************************************************************************
 * Examples (1-5)
 ******************************************************************************/

/**
 * @brief Example 1: Hello World - Basic Label
 * Learning: lv_label_create, lv_obj_align, lv_obj_set_style_*
 */
void part1_ex1_hello_world(void);

/**
 * @brief Example 2: Button with Click Counter
 * Learning: lv_button_create, lv_obj_add_event_cb, LV_EVENT_CLICKED
 */
void part1_ex2_button_counter(void);

/**
 * @brief Example 3: LED Widget Control
 * Learning: lv_led_create, lv_led_on/off, lv_led_set_brightness
 */
void part1_ex3_led_control(void);

/**
 * @brief Example 4: Switch Widget (ON/OFF Toggle)
 * Learning: lv_switch_create, LV_STATE_CHECKED, LV_EVENT_VALUE_CHANGED
 */
void part1_ex4_switch_toggle(void);

/**
 * @brief Example 5: GPIO Dashboard (Multiple LEDs and Switches)
 * Learning: Layout, Multiple widgets interaction
 */
void part1_ex5_gpio_dashboard(void);

#endif /* PART1_EXAMPLES_H */
