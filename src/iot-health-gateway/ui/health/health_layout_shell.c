#include "health_layout_shell.h"

#include "health_ui_root.h"

void health_layout_shell_init(void) { health_ui_root_init(); }

void health_layout_shell_poll(void) { health_ui_root_poll(); }

void health_layout_shell_on_alive_tick(unsigned long alive_sec) {
  health_ui_root_on_alive_tick(alive_sec);
}

void health_layout_shell_set_active_page(health_ui_page_t page, bool animate) {
  health_ui_root_set_active_page(page, animate);
}

void health_layout_shell_set_household_summary(const char *family_name,
                                               uint32_t member_count) {
  health_ui_root_set_household_summary(family_name, member_count);
}

void health_layout_shell_update_system_status(
    const health_ui_system_status_t *status) {
  health_ui_root_update_system_status(status);
}
