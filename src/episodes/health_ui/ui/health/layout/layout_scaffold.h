#ifndef HEALTH_LAYOUT_SCAFFOLD_H
#define HEALTH_LAYOUT_SCAFFOLD_H

#include "lvgl.h"

typedef struct {
  lv_obj_t *screen;
  lv_obj_t *root;
  lv_obj_t *tabview;
} health_layout_scaffold_t;

void health_layout_scaffold_create(health_layout_scaffold_t *out_scaffold);
lv_obj_t *health_layout_scaffold_add_tab(health_layout_scaffold_t *scaffold,
                                         const char *title);
lv_obj_t *health_layout_scaffold_create_page(lv_obj_t *tab);

#endif
