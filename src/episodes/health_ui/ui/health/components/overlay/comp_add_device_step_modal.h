#ifndef COMP_ADD_DEVICE_STEP_MODAL_H
#define COMP_ADD_DEVICE_STEP_MODAL_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*comp_add_device_step_modal_back_cb_t)(void *user_data);

void comp_add_device_step_modal_open(uint32_t device_index, uint32_t device_id,
                                     comp_add_device_step_modal_back_cb_t on_back,
                                     void *user_data);
void comp_add_device_step_modal_close(void);
bool comp_add_device_step_modal_is_open(void);

#endif
