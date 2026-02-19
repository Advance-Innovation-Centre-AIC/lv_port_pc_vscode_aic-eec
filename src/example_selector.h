/*******************************************************************************
 * File Name:   example_selector.h
 *
 * Description: Example Selector for PC Simulator
 *              Embedded C for IoT Course - LVGL UI Testing
 *
 * Usage:
 *   1. Set SELECTED_PART to choose the part (1, 2, 3, 4, 5)
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
 * @brief Select Part (1, 2, 3, 4, 5)
 *
 * Part 1: LVGL Basics + GPIO Concepts (UI Only)
 * Part 2: Sensor Visualization (UI Only)
 * Part 3: Oscilloscope & Signal Processing (UI Only)
 * Part 4: IPC, Logging, Event Bus (Simulated)
 * Part 5: WiFi Manager & IoT (Simulated)
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
 *
 * Part 2 Examples (UI Only):
 *   1: Slider Bar Visualization
 *   2: Arc Gauge
 *   3: Chart Time Series
 *   4: Scale Temperature
 *   5: Sensor Dashboard (TabView)
 *   6: Chart Dashboard
 *
 * Part 3 Examples (UI Only):
 *   1: Waveform Generator
 *   2: Noise Generator
 *   3: Audio Waveform Display
 *   4: Microphone Visualizer
 *   5: Oscilloscope UI
 *   6: Spectrum Analyzer
 *   7: Custom Panel Scope (Simulated)
 *
 * Part 4 Examples (IPC/Event Bus):
 *   1: IPC Ping Test
 *   2: Logging System
 *   3: IPC Sensor Data
 *   4: Event Bus Demo
 *   5: HW IPC LED Control
 *   6: HW IPC Button Events
 *   7: HW IPC Dashboard
 *   8: Advanced Features
 *   9: CapSense IPC
 *
 * Part 5 Examples (WiFi Simulated):
 *   1: WiFi Network List
 *   2: WiFi Connect/Disconnect
 *   3: TCP/IP Information
 *   4: Hardware Information
 *   5: Full WiFi Manager (macOS Style)  <-- Main Demo
 *   6: WiFi Status Dashboard
 *   7: IoT Dashboard
 *   8: MQTT Preview
 */
#define SELECTED_EXAMPLE    1

/*******************************************************************************
 * Include Part Headers
 ******************************************************************************/

#include "part1/part1_examples.h"
#include "part2/part2_examples.h"
#include "part3/part3_examples.h"
#include "part3/part3_scope_example.h"
#include "part4/part4_examples.h"
#include "part5/part5_examples.h"

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

#elif SELECTED_PART == 2
    #if SELECTED_EXAMPLE == 1
        part2_ex1_slider_bar();
    #elif SELECTED_EXAMPLE == 2
        part2_ex2_arc_gauge();
    #elif SELECTED_EXAMPLE == 3
        part2_ex3_chart_timeseries();
    #elif SELECTED_EXAMPLE == 4
        part2_ex4_scale_temperature();
    #elif SELECTED_EXAMPLE == 5
        part2_ex5_sensor_dashboard();
    #elif SELECTED_EXAMPLE == 6
        part2_ex6_chart_dashboard();
    #else
        part2_ex1_slider_bar();
    #endif

#elif SELECTED_PART == 3
    #if SELECTED_EXAMPLE == 1
        part3_ex1_waveform_generator();
    #elif SELECTED_EXAMPLE == 2
        part3_ex2_noise_generator();
    #elif SELECTED_EXAMPLE == 3
        part3_ex3_audio_waveform();
    #elif SELECTED_EXAMPLE == 4
        part3_ex4_mic_visualizer();
    #elif SELECTED_EXAMPLE == 5
        part3_ex5_oscilloscope_ui();
    #elif SELECTED_EXAMPLE == 6
        part3_ex6_spectrum_analyzer();
    #elif SELECTED_EXAMPLE == 7
        part3_ex7_custom_panel_scope();
    #else
        part3_ex1_waveform_generator();
    #endif

#elif SELECTED_PART == 4
    /* Part 4 - IPC, Logging, Event Bus (Simulated) */
    #if SELECTED_EXAMPLE == 1
        part4_ex1_ipc_ping();
    #elif SELECTED_EXAMPLE == 2
        part4_ex2_ipc_log();
    #elif SELECTED_EXAMPLE == 3
        part4_ex3_ipc_sensor();
    #elif SELECTED_EXAMPLE == 4
        part4_ex4_event_bus();
    #elif SELECTED_EXAMPLE == 5
        part4_ex5_hw_ipc_led();
    #elif SELECTED_EXAMPLE == 6
        part4_ex6_hw_ipc_button();
    #elif SELECTED_EXAMPLE == 7
        part4_ex7_hw_ipc_dashboard();
    #elif SELECTED_EXAMPLE == 8
        part4_ex8_advanced();
    #elif SELECTED_EXAMPLE == 9
        part4_ex9_capsense_ipc();
    #else
        part4_ex1_ipc_ping();
    #endif

#elif SELECTED_PART == 5
    /* Part 5 - WiFi Manager (Simulated) */
    #if SELECTED_EXAMPLE == 1
        part5_ex1_wifi_list();
    #elif SELECTED_EXAMPLE == 2
        part5_ex2_wifi_connect();
    #elif SELECTED_EXAMPLE == 3
        part5_ex3_tcpip_info();
    #elif SELECTED_EXAMPLE == 4
        part5_ex4_hardware_info();
    #elif SELECTED_EXAMPLE == 5
        part5_ex5_wifi_manager();
    #elif SELECTED_EXAMPLE == 6
        part5_ex6_wifi_status();
    #elif SELECTED_EXAMPLE == 7
        part5_ex7_iot_dashboard();
    #elif SELECTED_EXAMPLE == 8
        part5_ex8_mqtt_preview();
    #else
        part5_ex5_wifi_manager();
    #endif

#else
    /* Default: Part 1 Example 1 */
    part1_ex1_hello_world();
#endif
}

#endif /* EXAMPLE_SELECTOR_H */
