# AIC-EEC Module Library

Hardware Abstraction Layer and UI helpers for **Embedded Systems and IoT Development**, Department of Electrical Engineering, Faculty of Engineering, Burapha University.
This is the **PC Simulator** version — API signatures are identical to the PSoC Edge E84 embedded version, with mock implementations replacing real hardware access.

> **Author:** Assoc. Prof. Wiroon Sriborrirux (รศ.วิรุฬห์ ศรีบริรักษ์), Department of Electrical Engineering, Faculty of Engineering, Burapha University

---

## Modules

### Framework

| File | Description |
|------|-------------|
| `aic-eec.h/c` | Common UI components — footer, header, logo, theme, container |
| `aic_layout.h/c` | Flexbox-style layout helpers — row, column, card, gauge, progress bar, divider |
| `aic_event.h/c` | Publish-subscribe event bus — sensor/button/system events with callbacks |
| `aic_log.h/c` | Logging system — levels (Error/Warn/Info/Debug/Verbose), printf + LVGL output |

### Hardware Abstraction (Mock)

| File | Description |
|------|-------------|
| `gpio.h/c` | LED control (RGB), button input, PWM brightness — values stored in memory |
| `sensors.h/c` | ADC (5 channels), IMU (BMI270), temperature, CAPSENSE — simulated data |

### Signal Processing

| File | Description |
|------|-------------|
| `ma_filter.h` | Moving average filter (header-only, default window = 3 samples) |
| `tilt.h/c` | Complementary filter — roll/pitch from accelerometer + gyroscope |
| `scope.h/c` | Waveform generation (sine, square, triangle, sawtooth), FFT |

---

## Quick API Reference

### GPIO

```c
#include "gpio.h"

aic_gpio_init();                              // Initialize (returns bool)
aic_gpio_led_set(AIC_LED_RED, true);          // Turn LED on/off
aic_gpio_led_toggle(AIC_LED_GREEN);           // Toggle LED
aic_gpio_pwm_set_brightness(AIC_LED_BLUE, 50);  // PWM 0-100%

bool pressed = aic_gpio_button_read(AIC_BTN_USER);     // Read with debounce
bool edge    = aic_gpio_button_was_pressed(AIC_BTN_USER); // Edge detection
aic_gpio_button_set_callback(AIC_BTN_USER, my_func);   // Register callback
```

### Sensors

```c
#include "sensors.h"

aic_sensors_init();                           // Initialize all sensors

// ADC
uint16_t raw = aic_adc_read(AIC_ADC_CH0);    // Raw value (0-4095 @12-bit)
float volts  = aic_adc_read_voltage(AIC_ADC_CH0);  // Voltage (0-3.3V)
float temp   = aic_adc_read_temperature();    // Internal temp sensor

// IMU (BMI270)
float ax, ay, az;
aic_imu_read_accel(&ax, &ay, &az);           // Accelerometer (g)
float gx, gy, gz;
aic_imu_read_gyro(&gx, &gy, &gz);           // Gyroscope (deg/s)

aic_imu_data_t data;
aic_imu_read_all(&data);                     // All IMU data at once
```

### Layout (LVGL)

```c
#include "aic_layout.h"

aic_apply_dark_theme(NULL);                   // Dark theme on active screen
lv_obj_t *row  = aic_row_create(parent);      // Horizontal flex container
lv_obj_t *col  = aic_col_create(parent);      // Vertical flex container
lv_obj_t *card = aic_card_create(parent, "Title");  // Styled card with title
lv_obj_t *val  = aic_value_display_create(parent, "Temp:"); // Label + value
lv_obj_t *bar  = aic_progress_bar_create(parent, "CPU");    // Progress bar
aic_create_footer(parent);                    // Copyright footer
aic_create_header(parent, "My App");          // Title header
```

### Event Bus

```c
#include "aic_event.h"

aic_event_init();
aic_event_subscribe(AIC_EVENT_BUTTON_PRESS, my_callback, NULL);
aic_event_publish_button(AIC_BTN_USER, true);   // Helper for button events
aic_event_publish_adc(0, raw_value, voltage_mv); // Helper for ADC events
aic_event_process();   // Call from main loop (PC) or LVGL timer
```

### Logging

```c
#include "aic_log.h"

aic_log_init();
aic_log_set_level(AIC_LOG_DEBUG);

AIC_LOGE("Error: %s", msg);     // Error
AIC_LOGW("Warning: %d", val);   // Warning
AIC_LOGI("Info: started");      // Info
AIC_LOGD("Debug: x=%f", x);    // Debug
```

---

## PC Simulator vs PSoC Edge E84

| Aspect | PC Simulator | PSoC Edge E84 |
|--------|-------------|---------------|
| **API** | Identical | Identical |
| **GPIO** | In-memory state | Real GPIO pins |
| **ADC** | Simulated values | Real SAR ADC |
| **IMU** | Sine-wave animation | BMI270 via shared memory |
| **CAPSENSE** | Stub (returns false) | PSoC 4000T via I2C |
| **Event Bus** | Circular buffer | FreeRTOS queue |
| **Logging** | printf to terminal | printf + IPC to CM33 |

### PC-Only Functions

These functions exist only in the PC Simulator for testing:

```c
// Simulate button press from UI/keyboard
aic_gpio_sim_set_button(AIC_BTN_USER, true);

// Set simulated sensor values
aic_sensors_sim_set_adc(AIC_ADC_CH0, 75);     // Set ADC to 75%
aic_sensors_sim_tick();                         // Animate IMU data
aic_imu_set_simulated_accel(0.0f, 0.0f, 1.0f); // Set specific values
```

---

## Build (PC Simulator)

```bash
cd lv_port_pc_vscode_aic-eec
mkdir -p build && cd build
cmake ..
make -j8
./bin/main
```

---

## Color Palette (defined in aic_layout.h)

| Macro | Hex | Usage |
|-------|-----|-------|
| `AIC_COLOR_BG_DARK` | `#16213e` | Screen background |
| `AIC_COLOR_BG_CARD` | `#1f4068` | Card background |
| `AIC_COLOR_PRIMARY` | `#00d4ff` | Primary accent |
| `AIC_COLOR_SECONDARY` | `#ff6b6b` | Secondary accent |
| `AIC_COLOR_SUCCESS` | `#4ade80` | Success state |
| `AIC_COLOR_WARNING` | `#fbbf24` | Warning state |
| `AIC_COLOR_ERROR` | `#ef4444` | Error state |
| `AIC_COLOR_TEXT` | `#ffffff` | Normal text |
| `AIC_COLOR_TEXT_DIM` | `#94a3b8` | Dimmed text |

---

## License

Part of the **Embedded Systems and IoT Development** course materials.
Department of Electrical Engineering, Faculty of Engineering, Burapha University.
