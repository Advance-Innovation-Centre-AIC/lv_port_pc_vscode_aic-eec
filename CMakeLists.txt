cmake_minimum_required(VERSION 3.12.4)
project(lvgl C CXX)

# Set the correct FreeRTOS port for your system (e.g., Posix for WSL)
set(FREERTOS_PORT GCC_POSIX CACHE STRING "Port for FreeRTOS on Posix environment")

set(LVGL_PRO_PROJECT_DIR
    ""
    CACHE PATH "Path to LVGL Pro Project folder")

option(USE_FREERTOS "Enable FreeRTOS" OFF) # Turn this on to enable FreeRTOS

if(USE_FREERTOS)
    message(STATUS "FreeRTOS is enabled")

    # FreeRTOS integration when USE_FREERTOS is enabled
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/config)
    target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

    # Add FreeRTOS as a subdirectory
    add_subdirectory(FreeRTOS)

    # FreeRTOS-specific include directories
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/include)
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix)
    include_directories(${PROJECT_SOURCE_DIR}/config)

    # Add FreeRTOS sources
    file(GLOB FREERTOS_SOURCES
        "${PROJECT_SOURCE_DIR}/FreeRTOS/*.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/MemMang/heap_4.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix/*.c"
    )
else()
    message(STATUS "FreeRTOS is disabled")
    set(FREERTOS_SOURCES "")  # No FreeRTOS sources if FreeRTOS is disabled
endif()

# Main include files of the project
include_directories(${PROJECT_SOURCE_DIR}/main/inc)

# AIC-EEC include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/lvgl)

# Define options for LVGL with default values (OFF)
option(LV_USE_DRAW_SDL "Use SDL draw unit" OFF)
option(LV_USE_LIBPNG "Use libpng to decode PNG" OFF)
option(LV_USE_LIBJPEG_TURBO "Use libjpeg turbo to decode JPEG" OFF)
option(LV_USE_FFMPEG "Use libffmpeg to display video using lv_ffmpeg" OFF)
option(LV_USE_FREETYPE "Use freetype library" OFF)

# Set C and C++ standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output path for the executable
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

# Find and include SDL2 library
find_package(SDL2 REQUIRED)

# Add compile definitions based on the selected options
add_compile_definitions($<$<BOOL:${LV_USE_DRAW_SDL}>:LV_USE_DRAW_SDL=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBPNG}>:LV_USE_LIBPNG=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBJPEG_TURBO}>:LV_USE_LIBJPEG_TURBO=1>)
add_compile_definitions($<$<BOOL:${LV_USE_FFMPEG}>:LV_USE_FFMPEG=1>)

# Disable ThorVG (not needed, causes config.h issues on Windows/MSYS2)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)

# Add LVGL subdirectory
add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS})

# ======================================================================
# TESAIoT — IoT Health Gateway, Episodes & Practice Examples
# Each builds a separate executable under bin/
# ======================================================================

# Common sources shared by all TESAIoT example executables
file(GLOB TESAIOT_MOCK_SOURCES "src/tesaiot/mock_sensors/*/mock_*.c")
set(TESAIOT_COMMON_SOURCES
    src/sim_main.c
    src/mouse_cursor_icon.c
    src/hal/hal.c
    src/tesaiot/sensor_bus.c
    src/tesaiot/game_common.c
    src/tesaiot/app_logo.c
    ${TESAIOT_MOCK_SOURCES}
)

# Thai fonts (shared)
file(GLOB TESAIOT_FONT_SOURCES "src/fonts/*.c")
list(APPEND TESAIOT_COMMON_SOURCES ${TESAIOT_FONT_SOURCES})

# Include paths for TESAIoT stubs + mock sensors
set(TESAIOT_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src/tesaiot
    ${PROJECT_SOURCE_DIR}/src/tesaiot/mock_sensors
    ${PROJECT_SOURCE_DIR}/src/tesaiot/mock_sensors/bmi270
    ${PROJECT_SOURCE_DIR}/src/tesaiot/mock_sensors/dps368
    ${PROJECT_SOURCE_DIR}/src/tesaiot/mock_sensors/sht4x
    ${PROJECT_SOURCE_DIR}/src/tesaiot/mock_sensors/bmm350
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/fonts
    ${PROJECT_SOURCE_DIR}/main/inc
    ${PROJECT_SOURCE_DIR}/lvgl
    ${PROJECT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
)

set(TESAIOT_LIBS lvgl lvgl::examples lvgl::demos ${SDL2_LIBRARIES})
if(NOT MSVC)
    list(APPEND TESAIOT_LIBS m pthread)
endif()

# Helper macro: add_tesaiot_example(target_name source_dir)
macro(add_tesaiot_example TARGET_NAME SOURCE_DIR)
    file(GLOB_RECURSE _EXAMPLE_SOURCES "${SOURCE_DIR}/*.c")
    # Collect all subdirectories + their parents for include paths
    file(GLOB_RECURSE _ALL_HEADERS "${SOURCE_DIR}/*.h")
    set(_EXAMPLE_INC_DIRS ${SOURCE_DIR})
    foreach(_HDR ${_ALL_HEADERS})
        get_filename_component(_DIR ${_HDR} DIRECTORY)
        get_filename_component(_PARENT ${_DIR} DIRECTORY)
        list(APPEND _EXAMPLE_INC_DIRS ${_DIR} ${_PARENT})
    endforeach()
    list(REMOVE_DUPLICATES _EXAMPLE_INC_DIRS)
    add_executable(${TARGET_NAME} ${TESAIOT_COMMON_SOURCES} ${_EXAMPLE_SOURCES})
    target_include_directories(${TARGET_NAME} PRIVATE ${TESAIOT_INCLUDE_DIRS} ${_EXAMPLE_INC_DIRS})
    target_compile_definitions(${TARGET_NAME} PRIVATE LV_CONF_INCLUDE_SIMPLE)
    target_link_libraries(${TARGET_NAME} ${TESAIOT_LIBS})
endmacro()

# --- Practice Examples (24) ---
file(GLOB PRAC_DIRS "src/practise/prac_*")
foreach(PRAC_DIR ${PRAC_DIRS})
    get_filename_component(PRAC_NAME ${PRAC_DIR} NAME)
    add_tesaiot_example(${PRAC_NAME} ${PRAC_DIR})
endforeach()

# --- Episodes (HMI + Interactive) ---
file(GLOB EP_DIRS "src/episodes/*")
foreach(EP_DIR ${EP_DIRS})
    get_filename_component(EP_NAME ${EP_DIR} NAME)
    add_tesaiot_example(${EP_NAME} ${EP_DIR})
endforeach()

# --- IoT Health Gateway (standalone) ---
add_tesaiot_example(iot-health-gateway src/iot-health-gateway)

# Apply additional compile options if the build type is Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode enabled")

    target_compile_options(lvgl PRIVATE
        -pedantic-errors
        -Wall
        -Wclobbered
        -Wdeprecated
        -Wdouble-promotion
        -Wempty-body
        -Wextra
        -Wformat-security
        -Wmaybe-uninitialized
        # -Wmissing-prototypes
        -Wpointer-arith
        -Wmultichar
        -Wno-pedantic # ignored for now, we convert functions to pointers for properties table.
        -Wreturn-type
        -Wshadow
        -Wshift-negative-value
        -Wsizeof-pointer-memaccess
        -Wtype-limits
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wfloat-conversion
        -Wstrict-aliasing
    )

if (ASAN)
    message(STATUS "AddressSanitizer enabled")

    # Add AddressSanitizer flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
else()
    message(STATUS "AddressSanitizer disabled")
endif()
endif()
