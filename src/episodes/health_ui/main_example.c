/*******************************************************************************
 * main_example.c — Health UI entry point for PC Simulator
 *
 * Called by sim_main.c via example_main(screen).
 * health_layout_shell_init() internally calls health_ui_root_init().
 ******************************************************************************/

#include "lvgl.h"
#include <stdio.h>
#include "ui/health/health_layout_shell.h"

void example_main(lv_obj_t *parent)
{
    (void)parent;

    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x003366), LV_PART_MAIN);

    fprintf(stderr, "[SIM] Starting Health UI...\n");
    health_layout_shell_init();
    fprintf(stderr, "[SIM] Health UI ready\n");
}
