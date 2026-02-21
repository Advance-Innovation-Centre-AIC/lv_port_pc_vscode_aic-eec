# AIC-EEC Module Library (คู่มือภาษาไทย)

ไลบรารี Hardware Abstraction Layer และ UI helpers สำหรับวิชา **Embedded C for IoT**
นี่คือเวอร์ชัน **PC Simulator** — API ตรงกับเวอร์ชัน PSoC Edge E84 ทุกประการ โดยใช้ mock implementation แทนการเข้าถึงฮาร์ดแวร์จริง

> **ผู้พัฒนา:** รศ.ดร.วิรุณ ศรีบริรักษ์ คณะวิศวกรรมศาสตร์ มหาวิทยาลัยบูรพา

---

## โมดูลทั้งหมด

### Framework (โครงสร้างพื้นฐาน)

| ไฟล์ | คำอธิบาย |
|------|----------|
| `aic-eec.h/c` | UI components กลาง — footer, header, logo, theme, container |
| `aic_layout.h/c` | ตัวช่วยจัด layout แบบ Flexbox — row, column, card, gauge, progress bar |
| `aic_event.h/c` | ระบบ Event Bus แบบ publish-subscribe — sensor/button/system events |
| `aic_log.h/c` | ระบบ Logging — แบ่ง level (Error/Warn/Info/Debug/Verbose), output ทาง printf + LVGL |

### Hardware Abstraction (Mock — จำลองฮาร์ดแวร์)

| ไฟล์ | คำอธิบาย |
|------|----------|
| `gpio.h/c` | ควบคุม LED (RGB), อ่านปุ่มกด, ปรับความสว่าง PWM — เก็บค่าในหน่วยความจำ |
| `sensors.h/c` | ADC (5 channels), IMU (BMI270), อุณหภูมิ, CAPSENSE — ข้อมูลจำลอง |

### Signal Processing (ประมวลผลสัญญาณ)

| ไฟล์ | คำอธิบาย |
|------|----------|
| `ma_filter.h` | Moving average filter (header-only, ค่าเริ่มต้น window = 3 samples) |
| `tilt.h/c` | Complementary filter — คำนวณ roll/pitch จาก accelerometer + gyroscope |
| `scope.h/c` | สร้างสัญญาณ (sine, square, triangle, sawtooth), FFT |

---

## ตัวอย่างการใช้งาน

### GPIO — ควบคุม LED และปุ่มกด

```c
#include "gpio.h"

aic_gpio_init();                              // เริ่มต้นระบบ GPIO (คืนค่า bool)
aic_gpio_led_set(AIC_LED_RED, true);          // เปิด/ปิด LED
aic_gpio_led_toggle(AIC_LED_GREEN);           // สลับสถานะ LED
aic_gpio_pwm_set_brightness(AIC_LED_BLUE, 50);  // ปรับ PWM 0-100%

bool pressed = aic_gpio_button_read(AIC_BTN_USER);     // อ่านปุ่ม (มี debounce)
bool edge    = aic_gpio_button_was_pressed(AIC_BTN_USER); // ตรวจจับขอบ (กดแล้วเคลียร์)
aic_gpio_button_set_callback(AIC_BTN_USER, my_func);   // ลงทะเบียน callback
```

**LED ที่มี:** `AIC_LED_RED`, `AIC_LED_GREEN`, `AIC_LED_BLUE`
**ปุ่มที่มี:** `AIC_BTN_USER` (SW2), `AIC_BTN_USER2` (SW4)

### Sensors — เซ็นเซอร์

```c
#include "sensors.h"

aic_sensors_init();                           // เริ่มต้นเซ็นเซอร์ทั้งหมด

// ADC — อ่านค่าแอนะล็อก
uint16_t raw = aic_adc_read(AIC_ADC_CH0);    // ค่าดิบ (0-4095 ที่ 12-bit)
float volts  = aic_adc_read_voltage(AIC_ADC_CH0);  // แรงดัน (0-3.3V)
float temp   = aic_adc_read_temperature();    // อุณหภูมิภายใน (องศา C)

// IMU — ตัวตรวจจับการเคลื่อนไหว (BMI270)
float ax, ay, az;
aic_imu_read_accel(&ax, &ay, &az);           // ความเร่ง (หน่วย g)
float gx, gy, gz;
aic_imu_read_gyro(&gx, &gy, &gz);           // ความเร็วเชิงมุม (deg/s)

aic_imu_data_t data;
aic_imu_read_all(&data);                     // อ่านข้อมูล IMU ทั้งหมดพร้อมกัน
```

**ADC channels:** `AIC_ADC_CH0` (Pot), `CH1`, `CH2`, `CH3`, `AIC_ADC_TEMP`

### Layout — จัด UI ด้วย LVGL

```c
#include "aic_layout.h"

aic_apply_dark_theme(NULL);                   // ธีมมืดบนหน้าจอปัจจุบัน
lv_obj_t *row  = aic_row_create(parent);      // แถวแนวนอน (flex)
lv_obj_t *col  = aic_col_create(parent);      // คอลัมน์แนวตั้ง (flex)
lv_obj_t *card = aic_card_create(parent, "Title");  // การ์ดพร้อมหัวข้อ
lv_obj_t *val  = aic_value_display_create(parent, "Temp:");  // แสดง label + ค่า
lv_obj_t *bar  = aic_progress_bar_create(parent, "CPU");     // แถบความคืบหน้า
aic_create_footer(parent);                    // footer ลิขสิทธิ์
aic_create_header(parent, "My App");          // header พร้อมชื่อ
```

### Event Bus — ระบบ Event

```c
#include "aic_event.h"

aic_event_init();

// subscribe รับ event เมื่อกดปุ่ม
aic_event_subscribe(AIC_EVENT_BUTTON_PRESS, my_callback, NULL);

// publish ส่ง event
aic_event_publish_button(AIC_BTN_USER, true);    // event ปุ่มกด
aic_event_publish_adc(0, raw_value, voltage_mv); // event ADC

// ประมวลผล event queue (ต้องเรียกเป็นระยะบน PC)
aic_event_process();
```

**กลุ่ม Event ที่สำคัญ:**
- Sensor: `AIC_EVENT_IMU_UPDATE`, `AIC_EVENT_ADC_UPDATE`, `AIC_EVENT_TEMP_UPDATE`
- Input: `AIC_EVENT_BUTTON_PRESS`, `AIC_EVENT_BUTTON_RELEASE`
- System: `AIC_EVENT_IPC_CONNECTED`, `AIC_EVENT_ERROR`
- Custom: `AIC_EVENT_CUSTOM_1` ถึง `AIC_EVENT_CUSTOM_5`

### Logging — ระบบบันทึกข้อความ

```c
#include "aic_log.h"

aic_log_init();
aic_log_set_level(AIC_LOG_DEBUG);   // กำหนด level ต่ำสุดที่จะแสดง

AIC_LOGE("Error: %s", msg);        // ข้อผิดพลาด (สีแดง)
AIC_LOGW("Warning: %d", val);      // คำเตือน (สีเหลือง)
AIC_LOGI("Info: started");         // ข้อมูลทั่วไป
AIC_LOGD("Debug: x=%f", x);       // ดีบัก (สำหรับพัฒนา)
AIC_LOGV("Verbose: detail");      // รายละเอียดทั้งหมด
```

---

## เปรียบเทียบ PC Simulator กับ PSoC Edge E84

| ด้าน | PC Simulator | PSoC Edge E84 |
|------|-------------|---------------|
| **API** | เหมือนกันทุกประการ | เหมือนกันทุกประการ |
| **GPIO** | เก็บค่าในหน่วยความจำ | ควบคุม GPIO pin จริง |
| **ADC** | ค่าจำลอง | SAR ADC จริง |
| **IMU** | สร้างข้อมูล sine wave | อ่านจาก BMI270 ผ่าน shared memory |
| **CAPSENSE** | stub (คืนค่า false เสมอ) | อ่านจาก PSoC 4000T ผ่าน I2C |
| **Event Bus** | circular buffer (single-thread) | FreeRTOS queue (multi-thread) |
| **Logging** | printf ไปที่ terminal | printf + ส่ง IPC ไปยัง CM33 |

### ฟังก์ชันเฉพาะ PC Simulator

ฟังก์ชันเหล่านี้ใช้ได้เฉพาะบน PC สำหรับจำลองการทดสอบ:

```c
// จำลองการกดปุ่มจาก UI/keyboard
aic_gpio_sim_set_button(AIC_BTN_USER, true);

// ตั้งค่าเซ็นเซอร์จำลอง
aic_sensors_sim_set_adc(AIC_ADC_CH0, 75);      // ตั้ง ADC เป็น 75%
aic_sensors_sim_tick();                          // เคลื่อนไหว IMU อัตโนมัติ
aic_imu_set_simulated_accel(0.0f, 0.0f, 1.0f); // ตั้งค่าเฉพาะเจาะจง
```

---

## วิธี Build (PC Simulator)

```bash
cd lv_port_pc_vscode_aic-eec
mkdir -p build && cd build
cmake ..
make -j8
./bin/main
```

---

## ชุดสี (กำหนดใน aic_layout.h)

| Macro | สี | การใช้งาน |
|-------|-----|----------|
| `AIC_COLOR_BG_DARK` | `#16213e` | พื้นหลังหน้าจอ |
| `AIC_COLOR_BG_CARD` | `#1f4068` | พื้นหลังการ์ด |
| `AIC_COLOR_PRIMARY` | `#00d4ff` | สีหลัก |
| `AIC_COLOR_SECONDARY` | `#ff6b6b` | สีรอง |
| `AIC_COLOR_SUCCESS` | `#4ade80` | สถานะสำเร็จ |
| `AIC_COLOR_WARNING` | `#fbbf24` | สถานะเตือน |
| `AIC_COLOR_ERROR` | `#ef4444` | สถานะข้อผิดพลาด |
| `AIC_COLOR_TEXT` | `#ffffff` | ข้อความปกติ |
| `AIC_COLOR_TEXT_DIM` | `#94a3b8` | ข้อความจางๆ |

---

## การย้ายโค้ดจาก PC ไปบอร์ดจริง

เนื่องจาก API เหมือนกัน 100% การย้ายโค้ดทำได้ง่าย:

1. **คัดลอกโค้ด** จาก PC Simulator ไปใส่ในโปรเจค PSoC Edge
2. **เปลี่ยน include path** — `"lvgl/lvgl.h"` เป็น `"lvgl.h"` (ถ้าจำเป็น)
3. **ลบฟังก์ชัน sim** — ลบ `aic_gpio_sim_set_button()`, `aic_sensors_sim_tick()` ฯลฯ
4. **Build และทดสอบ** บนบอร์ดจริง

ไม่ต้องเปลี่ยนชื่อฟังก์ชัน, ชนิดข้อมูล, หรือ parameter ใดๆ ทั้งสิ้น

---

## ลิขสิทธิ์

ส่วนหนึ่งของสื่อการเรียนการสอนวิชา **Embedded C for IoT**
คณะวิศวกรรมศาสตร์ มหาวิทยาลัยบูรพา
