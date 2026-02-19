/*******************************************************************************
 * File Name:   part2_examples.c
 *
 * Description: Part 2 LVGL Examples - Sensor Data Visualization
 *              Part of Embedded C for IoT Course
 *
 * Learning Objectives:
 *   - Advanced LVGL widgets (Slider, Bar, Arc, Chart, Scale)
 *   - Sensor data visualization
 *   - Real-time data updates
 *   - Multi-widget dashboard design
 *
 * LVGL Version: v9.2.0
 *
 * =============================================================================
 * WIDGET SIZING GUIDELINES (for 480x320 screen)
 * =============================================================================
 *
 *   Widget Type      | Recommended Size      | Notes
 *   -----------------|-----------------------|----------------------------------
 *   Slider           | width: 200-300        | height auto
 *   Bar              | 300 x 25              | horizontal display
 *   Arc              | 140x140 to 200x200    | should be square
 *   Chart            | 400x260 max           | reduce if labels on side
 *   Scale (round)    | 200x200               | for gauge display
 *   TabView tab bar  | height: 80            | for touch-friendly tabs
 *
 * =============================================================================
 * CHART WIDGET PATTERNS
 * =============================================================================
 *
 *   Pattern 1: Line Chart for Time-Series
 *   --------------------------------------
 *   - Point count: 50 (good balance of detail and performance)
 *   - Hide points: lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR)
 *   - Line width: 2-3 pixels for visibility
 *   - Y-axis range: scale to your data (e.g., -200 to 1200 for accel*100)
 *
 *   Pattern 2: Bar Chart for Comparison
 *   ------------------------------------
 *   - Point count = number of categories
 *   - Use lv_chart_set_value_by_id() for direct update
 *   - Column padding: 30-50px for spacing
 *
 *   Pattern 3: Scatter Chart for X-Y Correlation
 *   ---------------------------------------------
 *   - Use LV_CHART_TYPE_SCATTER
 *   - Point count: 30-50 for motion trails
 *   - Use lv_chart_set_next_value2() for (x,y) pairs
 *
 * =============================================================================
 * TABVIEW PATTERNS
 * =============================================================================
 *
 *   Pattern 1: Top Tabs (default)
 *   -----------------------------
 *   - Tab bar position: LV_DIR_TOP
 *   - Tab bar size: 80 pixels (touch-friendly)
 *   - Content area: 480 x (320 - 80) = 480 x 240
 *
 *   Pattern 2: Left Tabs (for more tabs)
 *   ------------------------------------
 *   - Tab bar position: LV_DIR_LEFT
 *   - Tab bar size: 80 pixels
 *   - Content area: (480 - 80) x 320 = 400 x 320
 *
 *   IMPORTANT: Disable scrolling on content!
 *   lv_obj_remove_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
 *
 * =============================================================================
 * TIMER PATTERN FOR REAL-TIME UPDATES
 * =============================================================================
 *
 *   1. Create timer: lv_timer_t * timer = lv_timer_create(cb, period_ms, NULL);
 *   2. Update widgets in callback (runs in LVGL context, safe for UI updates)
 *   3. Period: 100ms for smooth updates, 500ms for slower data
 *   4. Delete on screen change: lv_timer_delete(timer);
 *
 * Official LVGL Documentation and Example References:
 *   - Slider:  https://docs.lvgl.io/9.2/widgets/slider.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/slider/lv_example_slider_1.c
 *   - Bar:     https://docs.lvgl.io/9.2/widgets/bar.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/bar/lv_example_bar_1.c
 *   - Arc:     https://docs.lvgl.io/9.2/widgets/arc.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/arc/lv_example_arc_1.c
 *   - Chart:   https://docs.lvgl.io/9.2/widgets/chart.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/chart/lv_example_chart_1.c
 *   - Scale:   https://docs.lvgl.io/9.2/widgets/scale.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/scale/lv_example_scale_1.c
 *   - TabView: https://docs.lvgl.io/9.2/widgets/tabview.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/tabview/lv_example_tabview_1.c
 *   - Line:    https://docs.lvgl.io/9.2/widgets/line.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/line/lv_example_line_1.c
 *
 ******************************************************************************/

#include "part2_examples.h"
#include "../aic-eec/aic-eec.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************
 * Simulation Variables
 ******************************************************************************/
static uint32_t sim_counter = 0;

/*******************************************************************************
 * Simulation Functions
 ******************************************************************************/
uint16_t simulate_adc_read(void)
{
    /* Simulate ADC with sine wave + noise */
    sim_counter++;
    float base = 2048.0f + 1500.0f * sinf((float)sim_counter * 0.05f);
    int32_t noise = (rand() % 100) - 50;
    int32_t value = (int32_t)base + noise;

    if(value < 0) value = 0;
    if(value > 4095) value = 4095;

    return (uint16_t)value;
}

void simulate_imu_accel(float *ax, float *ay, float *az)
{
    sim_counter++;
    /* Simulate accelerometer with small oscillations around gravity */
    *ax = 0.5f * sinf((float)sim_counter * 0.03f) + ((rand() % 100) - 50) * 0.01f;
    *ay = 0.5f * cosf((float)sim_counter * 0.04f) + ((rand() % 100) - 50) * 0.01f;
    *az = 9.81f + 0.3f * sinf((float)sim_counter * 0.02f);
}

void simulate_imu_gyro(float *gx, float *gy, float *gz)
{
    /* Simulate gyroscope with small rotations */
    *gx = 0.5f * sinf((float)sim_counter * 0.05f);
    *gy = 0.3f * cosf((float)sim_counter * 0.06f);
    *gz = 0.2f * sinf((float)sim_counter * 0.04f);
}

/*******************************************************************************
 * Example 1: Slider and Bar - ADC Visualization
 *
 * Official References:
 *   - Slider: https://docs.lvgl.io/9.2/widgets/slider.html
 *             https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/slider/lv_example_slider_1.c
 *   - Bar:    https://docs.lvgl.io/9.2/widgets/bar.html
 *             https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/bar/lv_example_bar_1.c
 *
 * WHY: ADC returns analog values (0-4095) that need proper visualization
 * HOW: Slider for user input, Bar for display-only output
 * CAUTION: Always map the range correctly (ADC 0-4095 to percentage 0-100)
 ******************************************************************************/
static lv_obj_t * ex1_slider;
static lv_obj_t * ex1_bar;
static lv_obj_t * ex1_raw_label;
static lv_obj_t * ex1_volt_label;
static lv_obj_t * ex1_pct_label;
static lv_timer_t * ex1_timer;

#define ADC_MAX 4095
#define ADC_VREF_MV 3300

static void ex1_slider_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);

    /* Update bar */
    int32_t pct = (value * 100) / ADC_MAX;
    lv_bar_set_value(ex1_bar, pct, LV_ANIM_ON);

    /* Update labels */
    lv_label_set_text_fmt(ex1_raw_label, "Raw: %d", (int)value);
    lv_label_set_text_fmt(ex1_pct_label, "Percent: %d%%", (int)pct);

    float voltage = ((float)value / ADC_MAX) * (ADC_VREF_MV / 1000.0f);
    lv_label_set_text_fmt(ex1_volt_label, "Voltage: %.3f V", (double)voltage);
}

static void ex1_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    /* Simulate ADC reading */
    uint16_t adc_value = simulate_adc_read();
    lv_slider_set_value(ex1_slider, adc_value, LV_ANIM_ON);

    /* Trigger slider callback */
    lv_obj_send_event(ex1_slider, LV_EVENT_VALUE_CHANGED, NULL);
}

void part2_ex1_slider_bar(void)
{
    /* Background */
    lv_obj_set_style_bg_color(lv_screen_active(),
                              lv_color_hex(0x1a1a2e), LV_PART_MAIN);

    /* Title */
    lv_obj_t * title = lv_label_create(lv_screen_active());
    lv_label_set_text(title, "Part 2 - Example 1: ADC Visualization");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Slider (represents ADC value) */
    lv_obj_t * slider_label = lv_label_create(lv_screen_active());
    lv_label_set_text(slider_label, "ADC Raw Value (0-4095):");
    lv_obj_set_style_text_color(slider_label, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(slider_label, LV_ALIGN_TOP_LEFT, 20, 50);

    ex1_slider = lv_slider_create(lv_screen_active());
    lv_obj_set_width(ex1_slider, 300);
    lv_obj_align(ex1_slider, LV_ALIGN_TOP_MID, 0, 80);
    lv_slider_set_range(ex1_slider, 0, ADC_MAX);
    lv_slider_set_value(ex1_slider, 2048, LV_ANIM_OFF);
    lv_obj_add_event_cb(ex1_slider, ex1_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Bar (represents percentage) */
    lv_obj_t * bar_label = lv_label_create(lv_screen_active());
    lv_label_set_text(bar_label, "Percentage:");
    lv_obj_set_style_text_color(bar_label, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(bar_label, LV_ALIGN_TOP_LEFT, 20, 130);

    ex1_bar = lv_bar_create(lv_screen_active());
    lv_obj_set_size(ex1_bar, 300, 25);
    lv_obj_align(ex1_bar, LV_ALIGN_TOP_MID, 0, 160);
    lv_bar_set_range(ex1_bar, 0, 100);
    lv_bar_set_value(ex1_bar, 50, LV_ANIM_OFF);

    /* Value labels */
    ex1_raw_label = lv_label_create(lv_screen_active());
    lv_label_set_text(ex1_raw_label, "Raw: 2048");
    lv_obj_set_style_text_color(ex1_raw_label, lv_color_hex(0x00FF00), 0);
    lv_obj_align(ex1_raw_label, LV_ALIGN_CENTER, -100, 30);

    ex1_pct_label = lv_label_create(lv_screen_active());
    lv_label_set_text(ex1_pct_label, "Percent: 50%");
    lv_obj_set_style_text_color(ex1_pct_label, lv_color_hex(0x00FFFF), 0);
    lv_obj_align(ex1_pct_label, LV_ALIGN_CENTER, 100, 30);

    ex1_volt_label = lv_label_create(lv_screen_active());
    lv_label_set_text(ex1_volt_label, "Voltage: 1.650 V");
    lv_obj_set_style_text_color(ex1_volt_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_set_style_text_font(ex1_volt_label, &lv_font_montserrat_24, 0);
    lv_obj_align(ex1_volt_label, LV_ALIGN_CENTER, 0, 80);

    /* Auto-update timer */
    ex1_timer = lv_timer_create(ex1_timer_cb, 100, NULL);

    /* Description */
    lv_obj_t * desc = lv_label_create(lv_screen_active());
    lv_label_set_text(desc, "Learning: lv_slider, lv_bar, ADC value mapping\n"
                           "Formula: V = (raw / 4095) x 3.3V");
    lv_obj_set_style_text_color(desc, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_align(desc, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc, LV_ALIGN_BOTTOM_MID, 0, -30);

    /* Footer */
    aic_create_footer(lv_screen_active());
}

/*******************************************************************************
 * Example 2: Arc Widget - Circular Gauge
 *
 * Official Reference: https://docs.lvgl.io/9.2/widgets/arc.html
 * Example Code:       https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/arc/lv_example_arc_1.c
 *
 * WHY: Arc is ideal for displaying angles or percentages in circular form
 * HOW: Use with gyroscope, compass, or percentage gauges
 * CAUTION: Set rotation and bg_angles correctly for desired appearance
 ******************************************************************************/
static lv_obj_t * ex2_arc;
static lv_obj_t * ex2_value_label;
static lv_timer_t * ex2_timer;

static void ex2_arc_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_target(e);
    int32_t value = lv_arc_get_value(arc);
    lv_label_set_text_fmt(ex2_value_label, "%d%%", (int)value);
}

static void ex2_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    /* Simulate changing value */
    uint16_t adc = simulate_adc_read();
    int32_t pct = (adc * 100) / ADC_MAX;
    lv_arc_set_value(ex2_arc, pct);
    lv_label_set_text_fmt(ex2_value_label, "%d%%", (int)pct);
}

void part2_ex2_arc_gauge(void)
{
    /* Background */
    lv_obj_set_style_bg_color(lv_screen_active(),
                              lv_color_hex(0x0f0f23), LV_PART_MAIN);

    /* Title */
    lv_obj_t * title = lv_label_create(lv_screen_active());
    lv_label_set_text(title, "Part 2 - Example 2: Arc Gauge");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Create Arc */
    ex2_arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(ex2_arc, 200, 200);
    lv_arc_set_rotation(ex2_arc, 135);
    lv_arc_set_bg_angles(ex2_arc, 0, 270);
    lv_arc_set_range(ex2_arc, 0, 100);
    lv_arc_set_value(ex2_arc, 50);
    lv_obj_center(ex2_arc);
    lv_obj_add_event_cb(ex2_arc, ex2_arc_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /* Style the arc */
    lv_obj_set_style_arc_width(ex2_arc, 20, LV_PART_MAIN);
    lv_obj_set_style_arc_width(ex2_arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ex2_arc,
                               lv_palette_main(LV_PALETTE_CYAN),
                               LV_PART_INDICATOR);

    /* Value label in center */
    ex2_value_label = lv_label_create(lv_screen_active());
    lv_label_set_text(ex2_value_label, "50%");
    lv_obj_set_style_text_color(ex2_value_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(ex2_value_label, &lv_font_montserrat_24, 0);
    lv_obj_center(ex2_value_label);

    /* Auto-update timer */
    ex2_timer = lv_timer_create(ex2_timer_cb, 100, NULL);

    /* Description */
    lv_obj_t * desc = lv_label_create(lv_screen_active());
    lv_label_set_text(desc, "Learning: lv_arc_create, lv_arc_set_rotation\n"
                           "Use case: Gauge display, gyroscope angle");
    lv_obj_set_style_text_color(desc, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_text_align(desc, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc, LV_ALIGN_BOTTOM_MID, 0, -30);

    /* Footer */
    aic_create_footer(lv_screen_active());
}

/*******************************************************************************
 * Example 3: Chart Widget - Time-Series Data
 *
 * Official Reference: https://docs.lvgl.io/9.2/widgets/chart.html
 * Example Code:       https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/chart/lv_example_chart_1.c
 *
 * WHY: Display data trends over time (historical view)
 * HOW: Accelerometer graphs, temperature history, sensor monitoring
 * CAUTION: Too many points will cause lag - keep point_count reasonable (~50-100)
 ******************************************************************************/
static lv_obj_t * ex3_chart;
static lv_chart_series_t * ex3_ser_x;
static lv_chart_series_t * ex3_ser_y;
static lv_chart_series_t * ex3_ser_z;
static lv_obj_t * ex3_labels[3];
static lv_timer_t * ex3_timer;

static void ex3_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    float ax, ay, az;
    simulate_imu_accel(&ax, &ay, &az);

    /* Scale to int for chart (multiply by 100 for precision) */
    int32_t x_val = (int32_t)(ax * 100);
    int32_t y_val = (int32_t)(ay * 100);
    int32_t z_val = (int32_t)(az * 100);

    /* Add new points */
    lv_chart_set_next_value(ex3_chart, ex3_ser_x, x_val);
    lv_chart_set_next_value(ex3_chart, ex3_ser_y, y_val);
    lv_chart_set_next_value(ex3_chart, ex3_ser_z, z_val);

    /* Update labels */
    lv_label_set_text_fmt(ex3_labels[0], "X: %+.2f", (double)ax);
    lv_label_set_text_fmt(ex3_labels[1], "Y: %+.2f", (double)ay);
    lv_label_set_text_fmt(ex3_labels[2], "Z: %+.2f", (double)az);
}

void part2_ex3_chart_timeseries(void)
{
    /* Background */
    lv_obj_set_style_bg_color(lv_screen_active(),
                              lv_color_hex(0x16213e), LV_PART_MAIN);

    /* Title */
    lv_obj_t * title = lv_label_create(lv_screen_active());
    lv_label_set_text(title, "Part 2 - Example 3: Accelerometer Chart");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Create Chart - maximized for 480x320 screen (with space for title/labels/footer) */
    ex3_chart = lv_chart_create(lv_screen_active());
    lv_obj_set_size(ex3_chart, 440, 260);
    lv_obj_align(ex3_chart, LV_ALIGN_CENTER, -10, 10);
    lv_chart_set_type(ex3_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ex3_chart, 50);

    /* Range: -1500 to 1500 (represents -15 to +15 m/s^2 x100) */
    lv_chart_set_range(ex3_chart, LV_CHART_AXIS_PRIMARY_Y, -200, 1200);

    /* Hide points, show only lines */
    lv_obj_set_style_size(ex3_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(ex3_chart, 2, LV_PART_ITEMS);

    /* Add series */
    ex3_ser_x = lv_chart_add_series(ex3_chart,
                                    lv_palette_main(LV_PALETTE_RED),
                                    LV_CHART_AXIS_PRIMARY_Y);
    ex3_ser_y = lv_chart_add_series(ex3_chart,
                                    lv_palette_main(LV_PALETTE_GREEN),
                                    LV_CHART_AXIS_PRIMARY_Y);
    ex3_ser_z = lv_chart_add_series(ex3_chart,
                                    lv_palette_main(LV_PALETTE_BLUE),
                                    LV_CHART_AXIS_PRIMARY_Y);

    /* Value labels */
    const char *axis_names[] = {"X:", "Y:", "Z:"};
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE)
    };

    for(int i = 0; i < 3; i++) {
        ex3_labels[i] = lv_label_create(lv_screen_active());
        lv_label_set_text_fmt(ex3_labels[i], "%s 0.00", axis_names[i]);
        lv_obj_set_style_text_color(ex3_labels[i], colors[i], 0);
        lv_obj_align(ex3_labels[i], LV_ALIGN_RIGHT_MID, -20, -40 + i * 30);
    }

    /* Legend */
    lv_obj_t * legend = lv_label_create(lv_screen_active());
    lv_label_set_text(legend, "Accelerometer (m/s^2)");
    lv_obj_set_style_text_color(legend, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align_to(legend, ex3_chart, LV_ALIGN_OUT_TOP_MID, 0, -5);

    /* Timer for updates */
    ex3_timer = lv_timer_create(ex3_timer_cb, 100, NULL);

    /* Description */
    lv_obj_t * desc = lv_label_create(lv_screen_active());
    lv_label_set_text(desc, "Learning: lv_chart, lv_chart_add_series, time-series data");
    lv_obj_set_style_text_color(desc, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(desc, LV_ALIGN_BOTTOM_MID, 0, -25);

    /* Footer */
    aic_create_footer(lv_screen_active());
}

/*******************************************************************************
 * Example 4: Scale Widget - Temperature Gauge (Round Scale with Sections)
 *
 * Official Reference: https://docs.lvgl.io/9.2/widgets/scale.html
 * Example Code:       https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/scale/lv_example_scale_4.c
 *
 * WHY: Scale looks like a real measuring instrument (professional appearance)
 * HOW: Temperature display, speedometer, pressure gauge
 * CAUTION: Custom labels array MUST be static and NULL-terminated
 ******************************************************************************/
static lv_obj_t * ex4_scale;
static lv_obj_t * ex4_temp_label;
static lv_obj_t * ex4_needle_line;
static lv_timer_t * ex4_timer;

static int32_t ex4_temp_value = 50;
static int32_t ex4_needle_length = 85;  /* Needle length - almost reaching labels */

static void ex4_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    /* Simulate temperature changes */
    ex4_temp_value += (rand() % 5) - 2;  /* -2 to +2 */
    if(ex4_temp_value < 0) ex4_temp_value = 0;
    if(ex4_temp_value > 100) ex4_temp_value = 100;

    lv_label_set_text_fmt(ex4_temp_label, "%d °C", (int)ex4_temp_value);

    /* Update needle position */
    lv_scale_set_line_needle_value(ex4_scale, ex4_needle_line, ex4_needle_length, ex4_temp_value);
}

void part2_ex4_scale_temperature(void)
{
    /* Background */
    lv_obj_set_style_bg_color(lv_screen_active(),
                              lv_color_hex(0x1a1a2e), LV_PART_MAIN);

    /* Title */
    lv_obj_t * title = lv_label_create(lv_screen_active());
    lv_label_set_text(title, "Part 2 - Example 4: Temperature Gauge");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /*=========================================================================
     * Create Scale - Following lv_example_scale_4 pattern
     *=========================================================================*/
    ex4_scale = lv_scale_create(lv_screen_active());
    lv_obj_set_size(ex4_scale, 200, 200);
    lv_scale_set_label_show(ex4_scale, true);
    lv_scale_set_mode(ex4_scale, LV_SCALE_MODE_ROUND_OUTER);
    lv_obj_align(ex4_scale, LV_ALIGN_CENTER, 0, 0);

    /* Range: 0-100 degrees Celsius */
    lv_scale_set_total_tick_count(ex4_scale, 21);
    lv_scale_set_major_tick_every(ex4_scale, 5);

    /* Tick length styling */
    lv_obj_set_style_length(ex4_scale, 5, LV_PART_ITEMS);      /* Minor tick */
    lv_obj_set_style_length(ex4_scale, 10, LV_PART_INDICATOR); /* Major tick */
    lv_scale_set_range(ex4_scale, 0, 100);

    /* Custom labels for temperature */
    static const char * custom_labels[] = {"0 °C", "25 °C", "50 °C", "75 °C", "100 °C", NULL};
    lv_scale_set_text_src(ex4_scale, custom_labels);

    /*=========================================================================
     * Indicator style (Major ticks) - Blue color
     *=========================================================================*/
    static lv_style_t indicator_style;
    lv_style_init(&indicator_style);

    /* Label style properties */
    lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
    lv_style_set_text_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));

    /* Major tick properties */
    lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
    lv_style_set_width(&indicator_style, 10);       /* Tick length */
    lv_style_set_line_width(&indicator_style, 2);   /* Tick width */
    lv_obj_add_style(ex4_scale, &indicator_style, LV_PART_INDICATOR);

    /*=========================================================================
     * Minor ticks style - Lighter blue
     *=========================================================================*/
    static lv_style_t minor_ticks_style;
    lv_style_init(&minor_ticks_style);
    lv_style_set_line_color(&minor_ticks_style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
    lv_style_set_width(&minor_ticks_style, 5);      /* Tick length */
    lv_style_set_line_width(&minor_ticks_style, 2); /* Tick width */
    lv_obj_add_style(ex4_scale, &minor_ticks_style, LV_PART_ITEMS);

    /*=========================================================================
     * Main arc/line style
     *=========================================================================*/
    static lv_style_t main_line_style;
    lv_style_init(&main_line_style);
    lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
    lv_style_set_arc_width(&main_line_style, 3);
    lv_obj_add_style(ex4_scale, &main_line_style, LV_PART_MAIN);

    /*=========================================================================
     * Section: Hot zone (75-100) - Red color
     *=========================================================================*/
    lv_scale_section_t * hot_section = lv_scale_add_section(ex4_scale);
    lv_scale_section_set_range(hot_section, 75, 100);

    static lv_style_t hot_section_main_style;
    lv_style_init(&hot_section_main_style);
    lv_style_set_arc_color(&hot_section_main_style, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_arc_width(&hot_section_main_style, 3);
    lv_scale_section_set_style(hot_section, LV_PART_MAIN, &hot_section_main_style);

    static lv_style_t hot_section_indicator_style;
    lv_style_init(&hot_section_indicator_style);
    lv_style_set_line_color(&hot_section_indicator_style, lv_palette_darken(LV_PALETTE_RED, 3));
    lv_style_set_text_color(&hot_section_indicator_style, lv_palette_darken(LV_PALETTE_RED, 3));
    lv_scale_section_set_style(hot_section, LV_PART_INDICATOR, &hot_section_indicator_style);

    static lv_style_t hot_section_items_style;
    lv_style_init(&hot_section_items_style);
    lv_style_set_line_color(&hot_section_items_style, lv_palette_main(LV_PALETTE_RED));
    lv_scale_section_set_style(hot_section, LV_PART_ITEMS, &hot_section_items_style);

    /*=========================================================================
     * Needle Line - Shows current value position
     *=========================================================================*/
    ex4_needle_line = lv_line_create(ex4_scale);

    /* Needle style - Orange/Yellow color for visibility */
    static lv_style_t needle_style;
    lv_style_init(&needle_style);
    lv_style_set_line_width(&needle_style, 2);  /* Thinner needle */
    lv_style_set_line_color(&needle_style, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_line_rounded(&needle_style, true);
    lv_obj_add_style(ex4_needle_line, &needle_style, 0);

    /* Set initial needle position */
    lv_scale_set_line_needle_value(ex4_scale, ex4_needle_line, ex4_needle_length, ex4_temp_value);

    /*=========================================================================
     * Temperature value label (center)
     *=========================================================================*/
    ex4_temp_label = lv_label_create(lv_screen_active());
    lv_label_set_text(ex4_temp_label, "50 °C");
    lv_obj_set_style_text_color(ex4_temp_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(ex4_temp_label, &lv_font_montserrat_24, 0);
    lv_obj_align(ex4_temp_label, LV_ALIGN_CENTER, 0, 0);

    /* Timer for simulated updates */
    ex4_timer = lv_timer_create(ex4_timer_cb, 500, NULL);

    /* Description */
    lv_obj_t * desc = lv_label_create(lv_screen_active());
    lv_label_set_text(desc, "Learning: lv_scale_4, custom_labels, section styling");
    lv_obj_set_style_text_color(desc, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(desc, LV_ALIGN_BOTTOM_MID, 0, -25);

    /* Footer */
    aic_create_footer(lv_screen_active());
}

/*******************************************************************************
 * Example 5: Sensor Dashboard
 *
 * Official References:
 *   - TabView: https://docs.lvgl.io/9.2/widgets/tabview.html
 *              https://github.com/lvgl/lvgl/blob/release/v9.2/examples/widgets/tabview/lv_example_tabview_1.c
 *   - Bar:     https://docs.lvgl.io/9.2/widgets/bar.html
 *   - Chart:   https://docs.lvgl.io/9.2/widgets/chart.html
 *   - Arc:     https://docs.lvgl.io/9.2/widgets/arc.html
 *
 * WHY: Combine multiple sensors into a single unified UI
 * HOW: TabView separates pages, organized layout for different data types
 * CAUTION: Updates must be done from LVGL context only (use timers, not ISR)
 ******************************************************************************/

/* Tab content objects */
static lv_obj_t * dash_adc_bar;
static lv_obj_t * dash_adc_label;
static lv_obj_t * dash_accel_chart;
static lv_chart_series_t * dash_accel_ser[3];
static lv_obj_t * dash_accel_labels[3];
static lv_obj_t * dash_gyro_arcs[3];
static lv_obj_t * dash_gyro_labels[3];
static lv_timer_t * dash_timer;

static void dash_timer_cb(lv_timer_t * timer)
{
    (void)timer;

    /* Update ADC */
    uint16_t adc = simulate_adc_read();
    int32_t pct = (adc * 100) / ADC_MAX;
    lv_bar_set_value(dash_adc_bar, pct, LV_ANIM_ON);
    float volt = ((float)adc / ADC_MAX) * 3.3f;
    lv_label_set_text_fmt(dash_adc_label, "ADC: %d (%.2fV)", adc, (double)volt);

    /* Update Accelerometer */
    float ax, ay, az;
    simulate_imu_accel(&ax, &ay, &az);
    lv_chart_set_next_value(dash_accel_chart, dash_accel_ser[0], (int32_t)(ax * 100));
    lv_chart_set_next_value(dash_accel_chart, dash_accel_ser[1], (int32_t)(ay * 100));
    lv_chart_set_next_value(dash_accel_chart, dash_accel_ser[2], (int32_t)(az * 100));
    lv_label_set_text_fmt(dash_accel_labels[0], "X:%+.1f", (double)ax);
    lv_label_set_text_fmt(dash_accel_labels[1], "Y:%+.1f", (double)ay);
    lv_label_set_text_fmt(dash_accel_labels[2], "Z:%+.1f", (double)az);

    /* Update Gyroscope */
    float gx, gy, gz;
    simulate_imu_gyro(&gx, &gy, &gz);
    int32_t gx_pct = (int32_t)((gx + 1.0f) * 50);  /* Map -1..1 to 0..100 */
    int32_t gy_pct = (int32_t)((gy + 1.0f) * 50);
    int32_t gz_pct = (int32_t)((gz + 1.0f) * 50);
    lv_arc_set_value(dash_gyro_arcs[0], gx_pct);
    lv_arc_set_value(dash_gyro_arcs[1], gy_pct);
    lv_arc_set_value(dash_gyro_arcs[2], gz_pct);
    lv_label_set_text_fmt(dash_gyro_labels[0], "%.2f", (double)gx);
    lv_label_set_text_fmt(dash_gyro_labels[1], "%.2f", (double)gy);
    lv_label_set_text_fmt(dash_gyro_labels[2], "%.2f", (double)gz);
}

static void create_adc_tab(lv_obj_t * parent)
{
    /* Content area: 480x270 (320 - 50 tab bar) */

    /* Title */
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "ADC Monitor");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 40);

    /* Bar - full width minus margins */
    dash_adc_bar = lv_bar_create(parent);
    lv_obj_set_size(dash_adc_bar, 450, 60);  /* Almost full width */
    lv_obj_align(dash_adc_bar, LV_ALIGN_CENTER, 0, 10);
    lv_bar_set_range(dash_adc_bar, 0, 100);

    /* Value label */
    dash_adc_label = lv_label_create(parent);
    lv_label_set_text(dash_adc_label, "ADC: 0 (0.00V)");
    lv_obj_set_style_text_font(dash_adc_label, &lv_font_montserrat_24, 0);
    lv_obj_align(dash_adc_label, LV_ALIGN_CENTER, 0, 80);
}

static void create_accel_tab(lv_obj_t * parent)
{
    /* Content area: 480x270 */

    /* Title */
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Accelerometer (m/s^2)");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);

    /* Chart - maximized for full tab */
    dash_accel_chart = lv_chart_create(parent);
    lv_obj_set_size(dash_accel_chart, 400, 210);  /* Full width, tall */
    lv_obj_align(dash_accel_chart, LV_ALIGN_CENTER, -25, 20);
    lv_chart_set_type(dash_accel_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(dash_accel_chart, 50);
    lv_chart_set_range(dash_accel_chart, LV_CHART_AXIS_PRIMARY_Y, -200, 1200);
    lv_obj_set_style_size(dash_accel_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(dash_accel_chart, 3, LV_PART_ITEMS);

    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE)
    };

    for(int i = 0; i < 3; i++) {
        dash_accel_ser[i] = lv_chart_add_series(dash_accel_chart, colors[i],
                                                LV_CHART_AXIS_PRIMARY_Y);
        dash_accel_labels[i] = lv_label_create(parent);
        lv_label_set_text(dash_accel_labels[i], "0.0");
        lv_obj_set_style_text_color(dash_accel_labels[i], colors[i], 0);
        lv_obj_set_style_text_font(dash_accel_labels[i], &lv_font_montserrat_24, 0);
        lv_obj_align(dash_accel_labels[i], LV_ALIGN_RIGHT_MID, -5, -70 + i * 55);
    }
}

static void create_gyro_tab(lv_obj_t * parent)
{
    /* Content area: 480x270 */

    /* Title */
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Gyroscope (rad/s)");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);

    const char *axis_names[] = {"Roll", "Pitch", "Yaw"};
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE)
    };

    for(int i = 0; i < 3; i++) {
        /* Large arcs filling the space */
        dash_gyro_arcs[i] = lv_arc_create(parent);
        lv_obj_set_size(dash_gyro_arcs[i], 140, 140);  /* Large arcs */
        lv_obj_align(dash_gyro_arcs[i], LV_ALIGN_CENTER, (i - 1) * 155, 0);
        lv_arc_set_range(dash_gyro_arcs[i], 0, 100);
        lv_arc_set_value(dash_gyro_arcs[i], 50);
        lv_arc_set_bg_angles(dash_gyro_arcs[i], 135, 45);
        lv_obj_set_style_arc_color(dash_gyro_arcs[i], colors[i], LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(dash_gyro_arcs[i], 20, LV_PART_MAIN);
        lv_obj_set_style_arc_width(dash_gyro_arcs[i], 20, LV_PART_INDICATOR);

        /* Remove knob */
        lv_obj_remove_style(dash_gyro_arcs[i], NULL, LV_PART_KNOB);

        /* Axis label above arc */
        lv_obj_t * axis_label = lv_label_create(parent);
        lv_label_set_text(axis_label, axis_names[i]);
        lv_obj_set_style_text_color(axis_label, colors[i], 0);
        lv_obj_set_style_text_font(axis_label, &lv_font_montserrat_24, 0);
        lv_obj_align_to(axis_label, dash_gyro_arcs[i], LV_ALIGN_OUT_TOP_MID, 0, -5);

        /* Value label below arc */
        dash_gyro_labels[i] = lv_label_create(parent);
        lv_label_set_text(dash_gyro_labels[i], "0.00");
        lv_obj_set_style_text_color(dash_gyro_labels[i], colors[i], 0);
        lv_obj_set_style_text_font(dash_gyro_labels[i], &lv_font_montserrat_24, 0);
        lv_obj_align_to(dash_gyro_labels[i], dash_gyro_arcs[i],
                        LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }
}

void part2_ex5_sensor_dashboard(void)
{
    /* Simple TabView - following lv_example_tabview_1 pattern */

    /* Create TabView */
    lv_obj_t * tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
    lv_tabview_set_tab_bar_size(tabview, 80);

    /* Set larger font for tab buttons to match larger tab bar */
    lv_obj_t * tab_btns = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_TOP, LV_PART_ITEMS | LV_STATE_CHECKED);
 //   lv_obj_set_style_text_font(tab_btns, LV_FONT_MONTSERRAT_34, LV_PART_ITEMS);

    /* Create tabs first - like official example */
    lv_obj_t * tab_adc = lv_tabview_add_tab(tabview, "ADC");
    lv_obj_t * tab_accel = lv_tabview_add_tab(tabview, "Accel");
    lv_obj_t * tab_gyro = lv_tabview_add_tab(tabview, "Gyro");

    /* Style tab content backgrounds - white */
    lv_obj_set_style_bg_color(tab_adc, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(tab_adc, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab_accel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(tab_accel, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab_gyro, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(tab_gyro, LV_OPA_COVER, 0);

    /* Populate tabs */
    create_adc_tab(tab_adc);
    create_accel_tab(tab_accel);
    create_gyro_tab(tab_gyro);

    /* CRITICAL: Disable scrolling on content - from lv_example_tabview_2 */
    lv_obj_remove_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);

    /* Timer for updates */
    dash_timer = lv_timer_create(dash_timer_cb, 100, NULL);

    /* Footer overlay - created AFTER TabView so it appears on top */
    aic_create_footer(lv_screen_active());
}

/*******************************************************************************
 * Example 6: Chart Dashboard - Multiple Chart Types for Sensor Visualization
 *
 * Official Reference: https://docs.lvgl.io/9.2/widgets/chart.html
 * TabView Reference:  https://docs.lvgl.io/9.2/widgets/tabview.html
 *
 * Chart Types Demonstrated:
 *   - Bar Chart:     Compare multiple axis values side by side
 *   - Area Chart:    Show magnitude trends with filled area
 *   - Scatter Chart: Visualize X vs Y motion patterns
 *   - Line Chart:    Classic time-series data visualization
 *
 * WHY: Real dashboard applications need different chart types for different data
 * HOW: TabView with tabs on LEFT, each tab contains a different chart type
 * CAUTION: Scatter chart requires LV_CHART_TYPE_SCATTER, points as (x,y) pairs
 ******************************************************************************/

/* Chart Dashboard - static variables */
#define EX6_CHART_POINTS    50
#define EX6_SCATTER_POINTS  30

/* Tab 1: Bar Chart - shows current X, Y, Z values */
static lv_obj_t * ex6_bar_chart;
static lv_chart_series_t * ex6_bar_ser;
static lv_obj_t * ex6_bar_labels[3];

/* Tab 2: Area Chart - shows acceleration magnitude over time */
static lv_obj_t * ex6_area_chart;
static lv_chart_series_t * ex6_area_ser;
static lv_obj_t * ex6_area_label;

/* Tab 3: Scatter Chart - shows X vs Y motion pattern */
static lv_obj_t * ex6_scatter_chart;
static lv_chart_series_t * ex6_scatter_ser;
static lv_obj_t * ex6_scatter_label;

/* Tab 4: Line Chart - shows gyroscope time series */
static lv_obj_t * ex6_line_chart;
static lv_chart_series_t * ex6_line_ser[3];
static lv_obj_t * ex6_line_labels[3];

static lv_timer_t * ex6_chart_timer;

/*-----------------------------------------------------------------------------
 * Timer callback - updates all charts with simulated IMU data
 *---------------------------------------------------------------------------*/
static void ex6_chart_timer_cb(lv_timer_t * timer)
{
    (void)timer;

    /* Get simulated sensor data */
    float ax, ay, az;
    float gx, gy, gz;
    simulate_imu_accel(&ax, &ay, &az);
    simulate_imu_gyro(&gx, &gy, &gz);

    /*=========================================================================
     * Update Tab 1: Bar Chart - X, Y, Z axis values
     *=========================================================================*/
    /* Scale accelerometer values to 0-100 range for bar display */
    int32_t bar_x = (int32_t)((ax + 2.0f) * 25.0f);  /* -2 to +2 -> 0 to 100 */
    int32_t bar_y = (int32_t)((ay + 2.0f) * 25.0f);
    int32_t bar_z = (int32_t)((az / 10.0f) * 10.0f); /* 0 to ~10 -> 0 to ~10 */

    if(bar_x < 0) bar_x = 0; if(bar_x > 100) bar_x = 100;
    if(bar_y < 0) bar_y = 0; if(bar_y > 100) bar_y = 100;
    if(bar_z < 0) bar_z = 0; if(bar_z > 100) bar_z = 100;

    /* Update bar chart data */
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 0, bar_x);
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 1, bar_y);
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 2, bar_z);
    lv_chart_refresh(ex6_bar_chart);

    /* Update bar labels */
    lv_label_set_text_fmt(ex6_bar_labels[0], "X: %.2f", (double)ax);
    lv_label_set_text_fmt(ex6_bar_labels[1], "Y: %.2f", (double)ay);
    lv_label_set_text_fmt(ex6_bar_labels[2], "Z: %.2f", (double)az);

    /*=========================================================================
     * Update Tab 2: Area Chart - Acceleration magnitude
     *=========================================================================*/
    float magnitude = sqrtf(ax*ax + ay*ay + az*az);
    int32_t mag_scaled = (int32_t)(magnitude * 10.0f);  /* Scale for display */
    if(mag_scaled > 150) mag_scaled = 150;

    lv_chart_set_next_value(ex6_area_chart, ex6_area_ser, mag_scaled);
    lv_label_set_text_fmt(ex6_area_label, "Magnitude: %.2f m/s²", (double)magnitude);

    /*=========================================================================
     * Update Tab 3: Scatter Chart - X vs Y motion pattern
     *=========================================================================*/
    /* Scale X, Y to chart coordinate range */
    int32_t scatter_x = (int32_t)((ax + 2.0f) * 50.0f);  /* -2 to +2 -> 0 to 200 */
    int32_t scatter_y = (int32_t)((ay + 2.0f) * 50.0f);

    lv_chart_set_next_value2(ex6_scatter_chart, ex6_scatter_ser, scatter_x, scatter_y);
    lv_label_set_text_fmt(ex6_scatter_label, "X: %.2f  Y: %.2f", (double)ax, (double)ay);

    /*=========================================================================
     * Update Tab 4: Line Chart - Gyroscope data (Roll, Pitch, Yaw)
     *=========================================================================*/
    /* Scale gyro values to chart range (typically -0.5 to +0.5 rad/s) */
    int32_t gx_scaled = (int32_t)((gx + 1.0f) * 50.0f);  /* -1 to +1 -> 0 to 100 */
    int32_t gy_scaled = (int32_t)((gy + 1.0f) * 50.0f);
    int32_t gz_scaled = (int32_t)((gz + 1.0f) * 50.0f);

    lv_chart_set_next_value(ex6_line_chart, ex6_line_ser[0], gx_scaled);
    lv_chart_set_next_value(ex6_line_chart, ex6_line_ser[1], gy_scaled);
    lv_chart_set_next_value(ex6_line_chart, ex6_line_ser[2], gz_scaled);

    lv_label_set_text_fmt(ex6_line_labels[0], "Roll: %.2f", (double)gx);
    lv_label_set_text_fmt(ex6_line_labels[1], "Pitch: %.2f", (double)gy);
    lv_label_set_text_fmt(ex6_line_labels[2], "Yaw: %.2f", (double)gz);
}

/*-----------------------------------------------------------------------------
 * Tab 1: Bar Chart - IMU Axis Comparison
 *---------------------------------------------------------------------------*/
static void create_bar_chart_tab(lv_obj_t * parent)
{
    /* Title */
    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Bar Chart: Accel X/Y/Z");
    lv_obj_set_style_text_font(title, LV_FONT_DEFAULT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    /* Create Bar Chart - MAXIMIZED to fill content area (400x320) */
    ex6_bar_chart = lv_chart_create(parent);
    lv_obj_set_size(ex6_bar_chart, 390, 280);
    lv_obj_align(ex6_bar_chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(ex6_bar_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_point_count(ex6_bar_chart, 3);  /* X, Y, Z */
    lv_chart_set_range(ex6_bar_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Add series with color gradient effect */
    ex6_bar_ser = lv_chart_add_series(ex6_bar_chart,
                                       lv_palette_main(LV_PALETTE_GREEN),
                                       LV_CHART_AXIS_PRIMARY_Y);

    /* Set initial values */
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 0, 50);
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 1, 50);
    lv_chart_set_value_by_id(ex6_bar_chart, ex6_bar_ser, 2, 98);

    /* Style the bars - wider spacing for larger chart */
    lv_obj_set_style_pad_column(ex6_bar_chart, 50, 0);

    /* Value labels below chart */
    const char *axis_names[] = {"X", "Y", "Z"};
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE)
    };

    for(int i = 0; i < 3; i++) {
        ex6_bar_labels[i] = lv_label_create(parent);
        lv_label_set_text_fmt(ex6_bar_labels[i], "%s: 0.00", axis_names[i]);
        lv_obj_set_style_text_color(ex6_bar_labels[i], colors[i], 0);
        lv_obj_align(ex6_bar_labels[i], LV_ALIGN_BOTTOM_LEFT, 30 + i * 120, -5);
    }
}

/*-----------------------------------------------------------------------------
 * Tab 2: Area Chart - Acceleration Magnitude Over Time
 *---------------------------------------------------------------------------*/
static void create_area_chart_tab(lv_obj_t * parent)
{
    /* Title */
    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Area: Accel Magnitude");
    lv_obj_set_style_text_font(title, LV_FONT_DEFAULT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    /* Create Area Chart - MAXIMIZED to fill content area */
    ex6_area_chart = lv_chart_create(parent);
    lv_obj_set_size(ex6_area_chart, 390, 285);
    lv_obj_align(ex6_area_chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(ex6_area_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ex6_area_chart, EX6_CHART_POINTS);
    lv_chart_set_range(ex6_area_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 150);

    /* Add series with area fill */
    ex6_area_ser = lv_chart_add_series(ex6_area_chart,
                                        lv_palette_main(LV_PALETTE_RED),
                                        LV_CHART_AXIS_PRIMARY_Y);

    /* Enable area fill - gradient from series color to transparent */
    lv_obj_set_style_bg_opa(ex6_area_chart, LV_OPA_50, LV_PART_ITEMS);

    /* Hide points, show only line - thicker line for larger chart */
    lv_obj_set_style_size(ex6_area_chart, 4, 4, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(ex6_area_chart, 3, LV_PART_ITEMS);

    /* Add horizontal reference lines */
    lv_chart_set_div_line_count(ex6_area_chart, 5, 8);

    /* Value label */
    ex6_area_label = lv_label_create(parent);
    lv_label_set_text(ex6_area_label, "Magnitude: 0.00 m/s²");
    lv_obj_set_style_text_color(ex6_area_label, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(ex6_area_label, LV_ALIGN_BOTTOM_MID, 0, -5);
}

/*-----------------------------------------------------------------------------
 * Tab 3: Scatter Chart - X vs Y Motion Pattern
 *---------------------------------------------------------------------------*/
static void create_scatter_chart_tab(lv_obj_t * parent)
{
    /* Title */
    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Scatter: X vs Y Motion");
    lv_obj_set_style_text_font(title, LV_FONT_DEFAULT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    /* Create Scatter Chart - MAXIMIZED (square-ish for scatter) */
    ex6_scatter_chart = lv_chart_create(parent);
    lv_obj_set_size(ex6_scatter_chart, 290, 280);
    lv_obj_align(ex6_scatter_chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(ex6_scatter_chart, LV_CHART_TYPE_SCATTER);
    lv_chart_set_point_count(ex6_scatter_chart, EX6_SCATTER_POINTS);

    /* X and Y ranges for scatter plot */
    lv_chart_set_range(ex6_scatter_chart, LV_CHART_AXIS_PRIMARY_X, 0, 200);
    lv_chart_set_range(ex6_scatter_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 200);

    /* Add scatter series */
    ex6_scatter_ser = lv_chart_add_series(ex6_scatter_chart,
                                           lv_palette_main(LV_PALETTE_BLUE),
                                           LV_CHART_AXIS_PRIMARY_Y);

    /* Style scatter points - larger points for bigger chart */
    lv_obj_set_style_size(ex6_scatter_chart, 10, 10, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(ex6_scatter_chart, LV_OPA_70, LV_PART_INDICATOR);

    /* Grid lines */
    lv_chart_set_div_line_count(ex6_scatter_chart, 5, 5);

    /* Value label */
    ex6_scatter_label = lv_label_create(parent);
    lv_label_set_text(ex6_scatter_label, "X: 0.00  Y: 0.00");
    lv_obj_set_style_text_color(ex6_scatter_label, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_align(ex6_scatter_label, LV_ALIGN_BOTTOM_MID, 0, -5);
}

/*-----------------------------------------------------------------------------
 * Tab 4: Line Chart - Gyroscope Time Series (Roll, Pitch, Yaw)
 *---------------------------------------------------------------------------*/
static void create_line_chart_tab(lv_obj_t * parent)
{
    /* Title */
    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, "Line: Gyro R/P/Y");
    lv_obj_set_style_text_font(title, LV_FONT_DEFAULT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    /* Create Line Chart - MAXIMIZED to fill content area */
    ex6_line_chart = lv_chart_create(parent);
    lv_obj_set_size(ex6_line_chart, 390, 275);
    lv_obj_align(ex6_line_chart, LV_ALIGN_CENTER, 0, -5);
    lv_chart_set_type(ex6_line_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ex6_line_chart, EX6_CHART_POINTS);
    lv_chart_set_range(ex6_line_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* Hide points for cleaner look - thicker lines for larger chart */
    lv_obj_set_style_size(ex6_line_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(ex6_line_chart, 3, LV_PART_ITEMS);

    /* Grid lines */
    lv_chart_set_div_line_count(ex6_line_chart, 5, 5);

    /* Add 3 series for Roll, Pitch, Yaw */
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE)
    };
    const char *names[] = {"Roll", "Pitch", "Yaw"};

    for(int i = 0; i < 3; i++) {
        ex6_line_ser[i] = lv_chart_add_series(ex6_line_chart, colors[i],
                                               LV_CHART_AXIS_PRIMARY_Y);

        ex6_line_labels[i] = lv_label_create(parent);
        lv_label_set_text_fmt(ex6_line_labels[i], "%s: 0.00", names[i]);
        lv_obj_set_style_text_color(ex6_line_labels[i], colors[i], 0);
        lv_obj_align(ex6_line_labels[i], LV_ALIGN_BOTTOM_LEFT, 20 + i * 125, -5);
    }
}

/*-----------------------------------------------------------------------------
 * Main Function: Chart Dashboard with 4 Tabs
 *---------------------------------------------------------------------------*/
void part2_ex6_chart_dashboard(void)
{
    /*=========================================================================
     * Create TabView with tabs on LEFT (like lv_example_tabview_2)
     *=========================================================================*/
    lv_obj_t * tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, 80);

    /* Style the TabView background */
    lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_RED, 2), 0);

    /* Style tab buttons */
    lv_obj_t * tab_btns = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

    /*=========================================================================
     * Create 4 tabs for different chart types
     *=========================================================================*/
    lv_obj_t * tab_bar = lv_tabview_add_tab(tabview, "Bar");
    lv_obj_t * tab_area = lv_tabview_add_tab(tabview, "Area");
    lv_obj_t * tab_scatter = lv_tabview_add_tab(tabview, "Scatter");
    lv_obj_t * tab_line = lv_tabview_add_tab(tabview, "Line");

    /* Style tab content backgrounds */
    lv_obj_set_style_bg_color(tab_bar, lv_color_hex(0xF5F5F5), 0);
    lv_obj_set_style_bg_opa(tab_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab_area, lv_color_hex(0xF5F5F5), 0);
    lv_obj_set_style_bg_opa(tab_area, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab_scatter, lv_color_hex(0xF5F5F5), 0);
    lv_obj_set_style_bg_opa(tab_scatter, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tab_line, lv_color_hex(0xF5F5F5), 0);
    lv_obj_set_style_bg_opa(tab_line, LV_OPA_COVER, 0);

    /*=========================================================================
     * Populate each tab with its chart
     *=========================================================================*/
    create_bar_chart_tab(tab_bar);
    create_area_chart_tab(tab_area);
    create_scatter_chart_tab(tab_scatter);
    create_line_chart_tab(tab_line);

    /* Disable scrolling on content */
    lv_obj_remove_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);

    /*=========================================================================
     * Timer for real-time updates
     *=========================================================================*/
    ex6_chart_timer = lv_timer_create(ex6_chart_timer_cb, 100, NULL);

    /*=========================================================================
     * Footer overlay
     *=========================================================================*/
    aic_create_footer(lv_screen_active());
}
