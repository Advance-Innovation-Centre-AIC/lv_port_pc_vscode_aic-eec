#ifndef COMP_PAGER_DOTS_H
#define COMP_PAGER_DOTS_H

#include <stdint.h>

#include "lvgl.h"

#ifndef COMP_PAGER_DOT_SIZE_PX
#define COMP_PAGER_DOT_SIZE_PX (6)
#endif

#ifndef COMP_PAGER_DOT_GAP_PX
#define COMP_PAGER_DOT_GAP_PX (6)
#endif

#ifndef COMP_PAGER_DOT_ACTIVE_COLOR_HEX
#define COMP_PAGER_DOT_ACTIVE_COLOR_HEX (0x26282B)
#endif

#ifndef COMP_PAGER_DOT_INACTIVE_COLOR_HEX
#define COMP_PAGER_DOT_INACTIVE_COLOR_HEX (0xD0D5DD)
#endif

lv_obj_t *comp_pager_dots_create(lv_obj_t *parent);
void comp_pager_dots_build(lv_obj_t *dots, uint32_t dot_count,
                           uint32_t active_idx);
void comp_pager_dots_set_active(lv_obj_t *dots, uint32_t active_idx);

#endif
