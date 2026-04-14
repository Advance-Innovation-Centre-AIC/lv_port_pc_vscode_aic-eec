#ifndef HEALTH_UI_DISPLAY_POLICY_H
#define HEALTH_UI_DISPLAY_POLICY_H

#include <stdint.h>

#include "lv_port_disp.h"

#if (MY_DISP_HOR_RES < MY_DISP_VER_RES)
#error "Health UI is landscape-only. Rotation/portrait is not supported."
#endif

#define HEALTH_UI_VIEWPORT_WIDTH ((uint32_t)MY_DISP_HOR_RES)
#define HEALTH_UI_VIEWPORT_HEIGHT ((uint32_t)MY_DISP_VER_RES)

/*******************************************************************************
 * Viewport-proportional scaling
 *
 * All pixel constants designed for 1024×600 (7") reference.
 * HL_SCALE_V(px) scales vertical dimensions proportionally to actual viewport.
 * HL_SCALE_H(px) scales horizontal dimensions proportionally.
 *
 *   7"  (600px): HL_SCALE_V(320) = 320
 *   4.3" (480px): HL_SCALE_V(320) = 256  (×0.8)
 ******************************************************************************/
#define HL_REF_HEIGHT (600U)
#define HL_REF_WIDTH  (1024U)

#define HL_SCALE_V(px) \
    ((int32_t)((int32_t)(px) * (int32_t)HEALTH_UI_VIEWPORT_HEIGHT / (int32_t)HL_REF_HEIGHT))
#define HL_SCALE_H(px) \
    ((int32_t)((int32_t)(px) * (int32_t)HEALTH_UI_VIEWPORT_WIDTH / (int32_t)HL_REF_WIDTH))

#endif
