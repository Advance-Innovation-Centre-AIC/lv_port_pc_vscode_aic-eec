/*******************************************************************************
 * File Name:   example_selector.h
 *
 * Description: Example Selector for PC Simulator
 *              Embedded C for IoT Course - LVGL UI Testing
 *
 * Usage:
 *   1. Set SELECTED_PART to choose the part (1)
 *   2. Set SELECTED_EXAMPLE to choose the example number
 *   3. Rebuild and run
 *
 ******************************************************************************/

#ifndef EXAMPLE_SELECTOR_H
#define EXAMPLE_SELECTOR_H

/*******************************************************************************
 * Example Selection
 ******************************************************************************/

/**
 * @brief Select Part (1)
 *
 * Part 1: LVGL Basics + GPIO Concepts (UI Only)
 */
#define SELECTED_PART       1

/**
 * @brief Select Example Number within the Part
 *
 * Part 1 Examples (UI Only):
 *   1: Hello World (Label)
 *   2: Button Counter
 *   3: LED Control with Slider
 *   4: Switch Toggle
 *   5: GPIO Dashboard
 */
#define SELECTED_EXAMPLE    1

/*******************************************************************************
 * Include Part Headers
 ******************************************************************************/

#include "part1/part1_examples.h"

/*******************************************************************************
 * Run Selected Example
 ******************************************************************************/

static inline void run_selected_example(void)
{
#if SELECTED_PART == 1
    #if SELECTED_EXAMPLE == 1
        part1_ex1_hello_world();
    #elif SELECTED_EXAMPLE == 2
        part1_ex2_button_counter();
    #elif SELECTED_EXAMPLE == 3
        part1_ex3_led_control();
    #elif SELECTED_EXAMPLE == 4
        part1_ex4_switch_toggle();
    #elif SELECTED_EXAMPLE == 5
        part1_ex5_gpio_dashboard();
    #else
        part1_ex1_hello_world();
    #endif

#else
    /* Default: Part 1 Example 1 */
    part1_ex1_hello_world();
#endif
}

#endif /* EXAMPLE_SELECTOR_H */
