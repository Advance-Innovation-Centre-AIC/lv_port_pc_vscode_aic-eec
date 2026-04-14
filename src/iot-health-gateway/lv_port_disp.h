/*******************************************************************************
 * lv_port_disp.h — PC Simulator compatibility shim
 *
 * Provides MY_DISP_HOR_RES / MY_DISP_VER_RES macros that the Health UI
 * display policy header expects.  On the real board these come from the
 * GFXSS driver; here we hardcode the 4.3" viewport (832×480) to match
 * the target display for layout development.
 *
 * Switch to 1024×600 to preview the 7" layout.
 ******************************************************************************/

#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

/* Simulate 4.3" Waveshare DSI (800×480, LVGL viewport 832×480) */
#define MTB_DISPLAY_W4P3INCH_RPI

#ifdef MTB_DISPLAY_W4P3INCH_RPI
#define MY_DISP_HOR_RES (832U)
#define MY_DISP_VER_RES (480U)
#else
/* 7" Waveshare DSI (1024×600) */
#define MY_DISP_HOR_RES (1024U)
#define MY_DISP_VER_RES (600U)
#endif

#endif /* LV_PORT_DISP_H */
