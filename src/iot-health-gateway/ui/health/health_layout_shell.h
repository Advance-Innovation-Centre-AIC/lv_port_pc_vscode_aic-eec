#ifndef HEALTH_LAYOUT_SHELL_H
#define HEALTH_LAYOUT_SHELL_H

#include "health_ui_root.h"

void health_layout_shell_init(void);
void health_layout_shell_poll(void);
void health_layout_shell_on_alive_tick(unsigned long alive_sec);
void health_layout_shell_set_active_page(health_ui_page_t page, bool animate);
void health_layout_shell_set_household_summary(const char *family_name,
                                               uint32_t member_count);
void health_layout_shell_update_system_status(
    const health_ui_system_status_t *status);

#endif
