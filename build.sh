#!/usr/bin/env bash
# =============================================================================
# build.sh - AIC-EEC LVGL PC Simulator Build Script
#
# Usage:
#   ./build.sh <part> <example>    Build and run
#   ./build.sh <part> <example> -b Build only (no run)
#   ./build.sh                     Build current selection and run
#   ./build.sh -c                  Clean build directory
#   ./build.sh -l                  List all examples
#
# Examples:
#   ./build.sh 1 1          Part 1, Example 1 (Hello World)
#   ./build.sh 3 5          Part 3, Example 5 (Oscilloscope UI)
#   ./build.sh 5 5          Part 5, Example 5 (WiFi Manager)
#   ./build.sh 2 3 -b       Part 2, Example 3 (build only)
# =============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
BIN_DIR="$SCRIPT_DIR/bin"
SELECTOR="$SCRIPT_DIR/src/example_selector.h"
EXECUTABLE="$BIN_DIR/main"

# Detect OS
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*) OS="windows" ;;
    Darwin*)               OS="macos"   ;;
    *)                     OS="linux"   ;;
esac

if [ "$OS" = "windows" ]; then
    EXECUTABLE="$BIN_DIR/main.exe"
fi

# --- Functions ---

print_header() {
    echo "=============================================="
    echo " AIC-EEC LVGL PC Simulator - Build Script"
    echo " Embedded Systems and IoT Development"
    echo "=============================================="
}

list_examples() {
    echo ""
    echo "Part 1: LVGL Basics + GPIO (11 examples)"
    echo "  Section I - UI Only:"
    echo "    1: Hello World            2: Button Counter"
    echo "    3: LED Control            4: Switch Toggle"
    echo "    5: GPIO Dashboard"
    echo "  Section II - Hardware Integration:"
    echo "    6: HW LED Control         7: HW Button Status"
    echo "    8: HW ADC Display         9: HW GPIO Dashboard"
    echo "   10: CapSense Mockup       11: CapSense Hardware"
    echo ""
    echo "Part 2: Sensor Visualization (11 examples)"
    echo "  Section I - Simulated Data:"
    echo "    1: Slider Bar             2: Arc Gauge"
    echo "    3: Chart Time Series      4: Scale Temperature"
    echo "    5: Sensor Dashboard       6: Chart Dashboard"
    echo "  Section II - Hardware Integration:"
    echo "    7: Real IMU Display       8: Real Sensor Dashboard"
    echo "    9: Real Arc Gauge        10: Real Scale Gauge"
    echo "   11: Real Chart Dashboard"
    echo ""
    echo "Part 3: Oscilloscope & Signal (8 examples)"
    echo "  Section I - Simulated Data:"
    echo "    1: Waveform Generator     2: Noise Generator"
    echo "    3: Audio Waveform         4: Mic Visualizer"
    echo "    5: Oscilloscope UI        6: Spectrum Analyzer"
    echo "  Section II - Integration:"
    echo "    7: Custom Panel Scope     8: HW Scope"
    echo ""
    echo "Part 4: IPC, Logging, Event Bus (9 examples)"
    echo "  Section I - Simulated:"
    echo "    1: IPC Ping Test          2: Logging System"
    echo "    3: IPC Sensor Data        4: Event Bus"
    echo "  Section II - Hardware IPC:"
    echo "    5: HW IPC LED             6: HW IPC Button"
    echo "    7: HW IPC Dashboard       8: Advanced Features"
    echo "    9: CapSense IPC"
    echo ""
    echo "Part 5: WiFi Manager & IoT (8 examples)"
    echo "  Section I - WiFi Basics:"
    echo "    1: WiFi Network List      2: WiFi Connect"
    echo "    3: TCP/IP Info            4: Hardware Info"
    echo "  Section II - Full Integration:"
    echo "    5: WiFi Manager           6: WiFi Status"
    echo "    7: IoT Dashboard          8: MQTT Preview"
    echo ""
}

update_selector() {
    local part=$1
    local example=$2

    sed -i.bak "s/^#define SELECTED_PART.*/#define SELECTED_PART       $part/" "$SELECTOR"
    sed -i.bak "s/^#define SELECTED_EXAMPLE.*/#define SELECTED_EXAMPLE    $example/" "$SELECTOR"
    rm -f "$SELECTOR.bak"

    echo " Setting: Part $part, Example $example"
}

do_cmake() {
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    if [ ! -f "Makefile" ]; then
        echo " Running cmake..."
        if [ "$OS" = "windows" ]; then
            cmake -G "MSYS Makefiles" "$SCRIPT_DIR"
        else
            cmake "$SCRIPT_DIR"
        fi
    fi
}

do_build() {
    cd "$BUILD_DIR"
    echo " Building..."
    make -j

    # Copy SDL2.dll on Windows
    if [ "$OS" = "windows" ]; then
        if [ ! -f "$BIN_DIR/SDL2.dll" ]; then
            if [ -f "/ucrt64/bin/SDL2.dll" ]; then
                cp /ucrt64/bin/SDL2.dll "$BIN_DIR/"
                echo " Copied SDL2.dll to bin/"
            else
                echo " WARNING: SDL2.dll not found at /ucrt64/bin/SDL2.dll"
                echo " Please copy it manually to bin/"
            fi
        fi
    fi
}

do_run() {
    echo ""
    echo " Running: $EXECUTABLE"
    echo "----------------------------------------------"
    cd "$SCRIPT_DIR"
    "$EXECUTABLE"
}

do_clean() {
    echo " Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    rm -rf "$BIN_DIR"
    echo " Done."
}

# --- Main ---

print_header

# Handle flags
if [ "$1" = "-c" ] || [ "$1" = "--clean" ]; then
    do_clean
    exit 0
fi

if [ "$1" = "-l" ] || [ "$1" = "--list" ]; then
    list_examples
    exit 0
fi

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo ""
    echo "Usage:"
    echo "  ./build.sh <part> <example>       Build and run"
    echo "  ./build.sh <part> <example> -b    Build only"
    echo "  ./build.sh                        Build current and run"
    echo "  ./build.sh -c                     Clean build"
    echo "  ./build.sh -l                     List examples"
    echo ""
    list_examples
    exit 0
fi

# Parse part and example
BUILD_ONLY=false

if [ -n "$1" ] && [ -n "$2" ]; then
    PART=$1
    EXAMPLE=$2

    # Validate
    if ! [[ "$PART" =~ ^[1-5]$ ]]; then
        echo " ERROR: Part must be 1-5 (got: $PART)"
        exit 1
    fi
    if ! [[ "$EXAMPLE" =~ ^[0-9]+$ ]]; then
        echo " ERROR: Example must be a number (got: $EXAMPLE)"
        exit 1
    fi

    update_selector "$PART" "$EXAMPLE"

    if [ "$3" = "-b" ] || [ "$3" = "--build-only" ]; then
        BUILD_ONLY=true
    fi
elif [ -n "$1" ]; then
    echo " ERROR: Please provide both part and example number"
    echo " Usage: ./build.sh <part> <example>"
    exit 1
fi

do_cmake
do_build

echo ""
echo " Build successful!"

if [ "$BUILD_ONLY" = false ]; then
    do_run
fi
