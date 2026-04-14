/*******************************************************************************
 * main_example.c — Health UI entry point for PC Simulator
 *
 * Called by sim_main.c via example_main(screen).
 * Initializes the Health UI layout shell + root with simulation data.
 ******************************************************************************/

#include "lvgl.h"
#include "ui/health/health_layout_shell.h"
#include "ui/health/health_ui_root.h"
#include "ui/health/sim/health_ui_sim_data.h"
#include "tesaiot/app_interface.h"

void example_main(lv_obj_t *parent)
{
    (void)parent;

    /* Background color matching embedded firmware */
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x003366), LV_PART_MAIN);

    health_layout_shell_init();
    health_ui_root_init();
}
