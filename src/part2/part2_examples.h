/*******************************************************************************
 * File Name:   part2_examples.h
 *
 * Description: Header file for Part 2 LVGL examples - Sensor Data Visualization
 *              Part of Embedded C for IoT Course
 *
 * Examples:
 *   1. Slider & Bar for ADC
 *   2. Arc Widget for Gauge
 *   3. Chart for Time-Series
 *   4. Scale for Temperature
 *   5. Sensor Dashboard (IMU + ADC)
 *   6. Chart Dashboard (Bar/Area/Scatter/Line)
 *
 ******************************************************************************/

#ifndef PART2_EXAMPLES_H
#define PART2_EXAMPLES_H

#include "lvgl/lvgl.h"

/*******************************************************************************
 * Examples (1-6)
 ******************************************************************************/

/**
 * @brief Example 1: Slider and Bar - ADC Visualization
 * Learning: lv_slider_create, lv_bar_create, value mapping
 */
void part2_ex1_slider_bar(void);

/**
 * @brief Example 2: Arc Widget - Circular Gauge
 * Learning: lv_arc_create, lv_arc_set_value, rotation
 */
void part2_ex2_arc_gauge(void);

/**
 * @brief Example 3: Chart Widget - Time-Series Data
 * Learning: lv_chart_create, lv_chart_add_series, lv_chart_set_next_value
 */
void part2_ex3_chart_timeseries(void);

/**
 * @brief Example 4: Scale Widget - Temperature Gauge
 * Learning: lv_scale_create, sections, color zones
 */
void part2_ex4_scale_temperature(void);

/**
 * @brief Example 5: Sensor Dashboard - IMU + ADC Integration
 * Learning: TabView, combining multiple widgets, real-time updates
 */
void part2_ex5_sensor_dashboard(void);

/**
 * @brief Example 6: Chart Dashboard - Multiple Chart Types
 * Learning: lv_chart (Bar/Area/Scatter/Line), TabView with left tabs, IMU visualization
 */
void part2_ex6_chart_dashboard(void);

/*******************************************************************************
 * Simulation Functions (for demo without real hardware)
 ******************************************************************************/

/**
 * @brief Simulate ADC reading (0-4095)
 * @return Simulated ADC value
 */
uint16_t simulate_adc_read(void);

/**
 * @brief Simulate IMU accelerometer reading
 * @param ax Pointer to X acceleration (m/s^2)
 * @param ay Pointer to Y acceleration (m/s^2)
 * @param az Pointer to Z acceleration (m/s^2)
 */
void simulate_imu_accel(float *ax, float *ay, float *az);

/**
 * @brief Simulate IMU gyroscope reading
 * @param gx Pointer to X angular velocity (rad/s)
 * @param gy Pointer to Y angular velocity (rad/s)
 * @param gz Pointer to Z angular velocity (rad/s)
 */
void simulate_imu_gyro(float *gx, float *gy, float *gz);

#endif /* PART2_EXAMPLES_H */
