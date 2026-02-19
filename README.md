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

## Build (ครั้งแรก)

**Windows:** ต้องรันใน **MSYS2 UCRT64** terminal เท่านั้น

```bash
cd lv_port_pc_vscode_aic-eec
mkdir build && cd build
cmake -G "MSYS Makefiles" ..
make -j
```

> **สำคัญ (Windows):** หลัง build เสร็จครั้งแรก ต้อง copy `SDL2.dll` มาไว้ใน `bin/`:
> ```bash
> cp /ucrt64/bin/SDL2.dll bin/
> ```
> ถ้าไม่ทำจะเจอ error "SDL2.dll was not found" ตอนรัน

**macOS / Linux:**

```bash
cd lv_port_pc_vscode_aic-eec
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

---

## เลือกตัวอย่าง + Build ด้วย build.sh (แนะนำ)

วิธีที่ง่ายที่สุด ใช้ `build.sh`:

```bash
./build.sh 1 1     # Part 1, Example 1 (Hello World) → build + รัน
./build.sh 2 3     # Part 2, Example 3 (Chart Time Series)
./build.sh 5 5     # Part 5, Example 5 (WiFi Manager)
./build.sh 3 2 -b  # Part 3, Example 2 → build อย่างเดียว (ไม่รัน)
./build.sh         # build ค่าปัจจุบัน + รัน
./build.sh -l      # แสดงรายการตัวอย่างทั้งหมด
./build.sh -c      # ลบ build directory (clean)
./build.sh -h      # แสดงวิธีใช้
```

Script จะ:
1. ตั้งค่า `SELECTED_PART` และ `SELECTED_EXAMPLE` ใน `src/example_selector.h` ให้อัตโนมัติ
2. สร้าง build directory + cmake (ถ้ายังไม่มี)
3. Build โปรเจค
4. Copy `SDL2.dll` ให้อัตโนมัติ (Windows)
5. รันโปรแกรมทันที (ยกเว้นใช้ `-b`)

---

## เลือกตัวอย่าง (แบบ Manual)

แก้ไขไฟล์ `src/example_selector.h`:

```c
#define SELECTED_PART       1    // เลือก Part (1-5)
#define SELECTED_EXAMPLE    1    // เลือก Example
```

แล้ว build ใหม่:

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
lv_port_pc_vscode_aic-eec/
├── build.sh                   Build Script (แนะนำ)
├── src/
│   ├── example_selector.h     <-- ตั้งค่า Part + Example ที่นี่
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
├── patches/                   Patch files สำหรับ repo ต้นฉบับ LVGL
├── bin/                       Output Executables
└── build/                     Build Directory (auto-generated)
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

หรือใช้ `build.sh`:

```bash
./build.sh -c        # clean
./build.sh 1 1       # build ใหม่
```

---

## แก้ปัญหา (Troubleshooting)

### Windows: cmake "Permission denied" หรือหา compiler ไม่เจอ

ถ้าติดตั้ง cmake จาก Windows (winget/installer) ไว้ด้วย จะชนกับ cmake ของ MSYS2

**ตรวจสอบ:**
```bash
which cmake
```

ถ้าได้ `/c/Program Files/CMake/bin/cmake` แสดงว่าใช้ตัวผิด

**แก้ไข:** เพิ่ม PATH ก่อนรัน cmake
```bash
export PATH=/ucrt64/bin:$PATH
```

หรือ**ถอนการติดตั้ง cmake ตัว Windows** ออก (ผ่าน Settings > Apps)

### Windows: SDL2.dll was not found

```bash
cp /ucrt64/bin/SDL2.dll bin/
```

หรือใช้ `build.sh` ซึ่งจะ copy ให้อัตโนมัติ

### Windows: ThorVG "config.h: No such file or directory"

> **หมายเหตุ:** ปัญหานี้เกิดเฉพาะกรณีที่ไปโหลด repo ต้นฉบับจาก LVGL
> (`git clone https://github.com/lvgl/lv_port_pc_vscode.git`) มาใช้เอง
> **repo นี้ (lv_port_pc_vscode_aic-eec) แก้ไขไว้ให้แล้ว ไม่ต้องทำอะไรเพิ่ม**

ถ้าใช้ repo ต้นฉบับของ LVGL จะเจอ error นี้ตอน build บน Windows/MSYS2:

```
tvgCommon.h:29:10: fatal error: config.h: No such file or directory
```

สาเหตุ: ThorVG (ไลบรารี vector graphics ใน LVGL) ต้องการ `config.h` ที่ cmake
ควรจะ generate ให้ แต่บน MSYS2 มันไม่ทำงานถูกต้อง

**วิธีแก้:** ดูรายละเอียดทั้งหมดที่ `patches/fix-thorvg-windows.patch`
โดยสรุปคือปิด ThorVG ใน `lv_conf.h` และ `CMakeLists.txt` (ไม่กระทบการใช้งานปกติ)

### ทุก OS: Build ไม่ผ่านหลังย้ายโฟลเดอร์

ถ้าย้าย directory ของโปรเจค ต้อง clean build ใหม่เพราะ cmake cache จำ path เดิม:

```bash
rm -rf build bin
./build.sh 1 1
```

---

(C) 2026 BiiL - Embedded C for IoT, Burapha University
