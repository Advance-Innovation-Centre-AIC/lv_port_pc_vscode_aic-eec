#ifndef COMP_ADD_DEVICE_LIST_MODAL_H
#define COMP_ADD_DEVICE_LIST_MODAL_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*comp_add_device_list_modal_select_cb_t)(uint32_t device_index,
                                                       uint32_t device_id,
                                                       void *user_data);

void comp_add_device_list_modal_open(comp_add_device_list_modal_select_cb_t on_select,
                                     void *user_data);
void comp_add_device_list_modal_close(void);
bool comp_add_device_list_modal_is_open(void);

#endif
