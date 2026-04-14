/**
 * sim_main.c — Generic launcher for TESAIoT episodes/practice examples
 *
 * Initializes LVGL + SDL2 display, then calls example_main() which is
 * defined in the linked example's main_example.c.
 */

#include <stdlib.h>
#include <unistd.h>
#include "lvgl.h"
#include "hal/hal.h"
#include "tesaiot/app_interface.h"

#define DISP_HOR_RES 800
#define DISP_VER_RES 480

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    lv_init();
    sdl_hal_init(DISP_HOR_RES, DISP_VER_RES);

    /* Call the example entry point — identical to firmware contract */
    example_main(lv_screen_active());

    /* Run LVGL event loop */
    while (1) {
        uint32_t time_till_next = lv_timer_handler();
        usleep((time_till_next > 0 ? time_till_next : 5) * 1000);
    }

    return 0;
}
