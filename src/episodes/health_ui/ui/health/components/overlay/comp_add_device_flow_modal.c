#include "comp_add_device_flow_modal.h"

#include "comp_add_device_list_modal.h"
#include "comp_add_device_step_modal.h"

typedef struct {
  uint32_t selected_device_index;
  uint32_t selected_device_id;
} add_device_flow_ctx_t;

static add_device_flow_ctx_t s_flow_ctx = {
    .selected_device_index = 0U,
    .selected_device_id = 0U,
};

static void on_list_device_selected(uint32_t device_index, uint32_t device_id,
                                    void *user_data);
static void on_step_back_to_list(void *user_data);

static void on_list_device_selected(uint32_t device_index, uint32_t device_id,
                                    void *user_data) {
  (void)user_data;

  s_flow_ctx.selected_device_index = device_index;
  s_flow_ctx.selected_device_id = device_id;

  /* Switch to step modal only after list modal was deleted. */
  comp_add_device_step_modal_open(s_flow_ctx.selected_device_index,
                                  s_flow_ctx.selected_device_id,
                                  on_step_back_to_list, NULL);
}

static void on_step_back_to_list(void *user_data) {
  (void)user_data;
  comp_add_device_list_modal_open(on_list_device_selected, NULL);
}

void comp_add_device_flow_modal_open(void) {
  if (comp_add_device_step_modal_is_open()) {
    return;
  }
  if (comp_add_device_list_modal_is_open()) {
    return;
  }

  /* TODO BLE-SCAN-TRIGGER: Start real BLE scan before showing device list.
   * #include "cm55_ipc_pipe.h"
   * #include "ipc_communication.h"
   * cm55_ipc_pipe_push_request(IPC_CMD_BLE_SCAN_START, NULL, 0);
   * This sends scan command to CM33 via IPC. Results arrive in
   * health_data_bridge.c → ble_scan_cache → shown in list_modal. */

  comp_add_device_list_modal_open(on_list_device_selected, NULL);
}

void comp_add_device_flow_modal_close(void) {
  comp_add_device_step_modal_close();
  comp_add_device_list_modal_close();
}

bool comp_add_device_flow_modal_is_open(void) {
  return (comp_add_device_list_modal_is_open() || comp_add_device_step_modal_is_open());
}
