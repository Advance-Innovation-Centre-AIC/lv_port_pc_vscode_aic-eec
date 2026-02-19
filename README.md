# AIC-EEC LVGL PC Simulator

LVGL PC Simulator สำหรับวิชา **Embedded C for IoT** มหาวิทยาลัยบูรพา

จำลองหน้าจอ LVGL ขนาด 800x480 บน PC เพื่อทดสอบ UI ก่อนลงบอร์ด PSoC Edge E84

---

## ติดตั้ง Dependencies

### macOS

```bash
brew install sdl2 cmake make
```

### Windows (MSYS2)

1. ดาวน์โหลดและติดตั้ง MSYS2 จาก https://www.msys2.org/
2. เปิด **MSYS2 UCRT64** จาก Start Menu
3. รันคำสั่ง:

```bash
pacman -S --noconfirm --needed \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-SDL2 \
    git make
```

### Linux (Ubuntu/Debian)

```bash
sudo apt install build-essential cmake libsdl2-dev
```

---

## Build

**Windows:** ต้องรันใน **MSYS2 UCRT64** terminal เท่านั้น

```bash
cd lv_port_pc_vscode
mkdir build && cd build
cmake -G "MSYS Makefiles" ..
make -j
```

**macOS / Linux:**

```bash
cd lv_port_pc_vscode
mkdir build && cd build
cmake ..
make -j
```

Executable จะอยู่ที่ `bin/main` (macOS/Linux) หรือ `bin/main.exe` (Windows)

---

## รันโปรแกรม

```bash
./bin/main
```

Windows: ต้อง copy `SDL2.dll` มาไว้ใน `bin/` ด้วย (ปกติอยู่ที่ `/ucrt64/bin/SDL2.dll`)

---

## เลือกตัวอย่าง

แก้ไขไฟล์ `src/example_selector.h`:

```c
#define SELECTED_PART       1    // เลือก Part (1-5)
#define SELECTED_EXAMPLE    1    // เลือก Example
```

แก้แล้ว **build ใหม่**:

```bash
cd build
make -j
```

---

## รายการตัวอย่างทั้งหมด

### Part 1: LVGL พื้นฐาน + GPIO

| Example | ชื่อ | เรียนรู้ |
|---------|------|---------|
| 1 | Hello World | Label, Align, Style |
| 2 | Button Counter | Button, Event Callback |
| 3 | LED Control | LED Widget, Slider, Brightness |
| 4 | Switch Toggle | Switch, State Check |
| 5 | GPIO Dashboard | Layout, Multiple Widgets |

### Part 2: แสดงผลข้อมูลเซ็นเซอร์

| Example | ชื่อ | เรียนรู้ |
|---------|------|---------|
| 1 | Slider Bar | Slider, Bar, ADC Visualization |
| 2 | Arc Gauge | Arc Widget, Circular Gauge |
| 3 | Chart Time Series | Chart, Line Series, Timer |
| 4 | Scale Temperature | Scale Widget, Color Zones |
| 5 | Sensor Dashboard | TabView, IMU + ADC |
| 6 | Chart Dashboard | Bar/Area/Scatter/Line Charts |

### Part 3: Oscilloscope & Signal Processing

| Example | ชื่อ | เรียนรู้ |
|---------|------|---------|
| 1 | Waveform Generator | Square/Sine/Triangle/Sawtooth |
| 2 | Noise Generator | White Noise Display |
| 3 | Audio Waveform | I2S Audio Visualization |
| 4 | Mic Visualizer | PDM Microphone Display |
| 5 | Oscilloscope UI | Full Scope Interface |
| 6 | Spectrum Analyzer | FFT Frequency Display |
| 7 | Custom Panel Scope | Custom Panel Navigation |

### Part 4: IPC, Logging, Event Bus (Simulated)

| Example | ชื่อ | เรียนรู้ |
|---------|------|---------|
| 1 | IPC Ping Test | IPC Ping/Pong |
| 2 | Logging System | Remote Logging |
| 3 | IPC Sensor Data | Sensor Data via IPC |
| 4 | Event Bus | Event-driven Architecture |
| 5 | HW IPC LED | LED Control via IPC |
| 6 | HW IPC Button | Button Events via IPC |
| 7 | HW IPC Dashboard | Full IPC Dashboard |
| 8 | Advanced Features | Combined Features |
| 9 | CapSense IPC | CapSense via IPC |

### Part 5: WiFi Manager & IoT (Simulated)

| Example | ชื่อ | เรียนรู้ |
|---------|------|---------|
| 1 | WiFi Network List | List Widget, Network Data |
| 2 | WiFi Connect | Connect/Disconnect Flow |
| 3 | TCP/IP Info | Network Information Display |
| 4 | Hardware Info | System Information |
| 5 | WiFi Manager | Full WiFi Manager UI |
| 6 | WiFi Status | Status Dashboard |
| 7 | IoT Dashboard | IoT Overview |
| 8 | MQTT Preview | MQTT UI Preview |

---

## โครงสร้างโปรเจค

```
lv_port_pc_vscode/
├── src/
│   ├── example_selector.h    <-- ตั้งค่า Part + Example ที่นี่
│   ├── main.c
│   ├── part1/                 Part 1: LVGL พื้นฐาน
│   ├── part2/                 Part 2: เซ็นเซอร์
│   ├── part3/                 Part 3: Oscilloscope
│   ├── part4/                 Part 4: IPC/Event Bus
│   ├── part5/                 Part 5: WiFi/IoT
│   ├── aic-eec/               ไลบรารี AIC-EEC (mock)
│   └── hal/                   Hardware Abstraction (SDL)
├── lvgl/                      LVGL Library v9.2
├── lv_conf.h                  LVGL Configuration
├── CMakeLists.txt             Build Configuration
└── bin/                       Output Executables
```

---

## Rebuild หลังแก้โค้ด

ไม่ต้อง cmake ใหม่ แค่ make:

```bash
cd build
make -j
```

ถ้าแก้ CMakeLists.txt หรือเพิ่ม/ลบไฟล์ ให้ cmake ใหม่:

```bash
cd build
cmake ..
make -j
```

---

## Clean Build

```bash
rm -rf build
mkdir build && cd build
cmake ..
make -j
```

---

(C) 2026 BiiL - Embedded C for IoT, Burapha University
