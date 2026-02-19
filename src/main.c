/**
 * @file main.c
 *
 * PC Simulator for AIC-EEC LVGL Examples
 * Embedded C for IoT Course - Burapha University
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

#include "hal/hal.h"
#include "example_selector.h"

/*********************
 *      DEFINES
 *********************/

/* Display size - Landscape mode for PC development */
#define DISP_HOR_RES  800
#define DISP_VER_RES  480

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  printf("==============================================\n");
  printf(" AIC-EEC LVGL PC Simulator\n");
  printf(" Embedded C for IoT Course\n");
  printf(" Burapha University\n");
  printf("==============================================\n");
  printf(" Display: %dx%d (matching PSoC Edge E84)\n", DISP_HOR_RES, DISP_VER_RES);
  printf(" Part: %d, Example: %d\n", SELECTED_PART, SELECTED_EXAMPLE);
  printf("==============================================\n\n");

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  sdl_hal_init(DISP_HOR_RES, DISP_VER_RES);

  /* Run the selected example */
  run_selected_example();

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
      sleep_time_ms = LV_DEF_REFR_PERIOD;
    }
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}


#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
