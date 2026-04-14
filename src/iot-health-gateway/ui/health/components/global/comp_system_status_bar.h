#ifndef COMP_SYSTEM_STATUS_BAR_H
#define COMP_SYSTEM_STATUS_BAR_H

#include <stdbool.h>

#include "lvgl.h"

#include "health_ui_display_policy.h"
#include "../../layout/layout_fonts.h"

/* Layout tuning knobs for the top-right system status bar. */
#ifndef COMP_STATUS_BAR_RIGHT_MARGIN_PX
#define COMP_STATUS_BAR_RIGHT_MARGIN_PX (18)
#endif

#ifndef COMP_STATUS_BAR_TOP_MARGIN_PX
#define COMP_STATUS_BAR_TOP_MARGIN_PX (0)
#endif

#ifndef COMP_STATUS_BAR_HEIGHT_PX
#define COMP_STATUS_BAR_HEIGHT_PX HL_SCALE_V(32)
#endif

/* Full-width top strip behind status content (icons + time/date). */
#ifndef COMP_STATUS_BAR_SHELL_WIDTH_PCT
#define COMP_STATUS_BAR_SHELL_WIDTH_PCT (100)
#endif

/* Fixed slot widths keep icon/text anchors stable when content changes. */
#ifndef COMP_STATUS_BAR_SLOT_ICON_W_PX
#define COMP_STATUS_BAR_SLOT_ICON_W_PX (14)
#endif

#ifndef COMP_STATUS_BAR_SLOT_TIME_W_PX
#define COMP_STATUS_BAR_SLOT_TIME_W_PX (64)
#endif

#ifndef COMP_STATUS_BAR_SLOT_DATE_W_PX
#define COMP_STATUS_BAR_SLOT_DATE_W_PX (120)
#endif

/* Fine tune only date/day text draw position without moving the whole bar. */
#ifndef COMP_STATUS_BAR_DATE_TEXT_RIGHT_INSET_PX
#define COMP_STATUS_BAR_DATE_TEXT_RIGHT_INSET_PX (6)
#endif

/* Text/cache/font tuning knobs for system status bar. */
#ifndef COMP_STATUS_BAR_TEXT_CACHE_SIZE
#define COMP_STATUS_BAR_TEXT_CACHE_SIZE (24U)
#endif

#ifndef COMP_STATUS_BAR_FONT
#define COMP_STATUS_BAR_FONT HL_FONT_CAPTION
#endif

/* Backward-compatible split knobs for icon/text sizing. */
#ifndef COMP_STATUS_BAR_ICON_FONT
#define COMP_STATUS_BAR_ICON_FONT (COMP_STATUS_BAR_FONT)
#endif

#ifndef COMP_STATUS_BAR_TEXT_FONT
#define COMP_STATUS_BAR_TEXT_FONT (COMP_STATUS_BAR_FONT)
#endif

#ifndef COMP_STATUS_BAR_DEFAULT_TIME_TEXT
#define COMP_STATUS_BAR_DEFAULT_TIME_TEXT ("9:41")
#endif

#ifndef COMP_STATUS_BAR_DEFAULT_DATE_TEXT
#define COMP_STATUS_BAR_DEFAULT_DATE_TEXT ("Tue Apr 1")
#endif

/* Color knobs for system status bar. */
#ifndef COMP_STATUS_BAR_BG_COLOR_HEX
#define COMP_STATUS_BAR_BG_COLOR_HEX (0xFFFFFF)
#endif

#ifndef COMP_STATUS_BAR_TEXT_COLOR_HEX
#define COMP_STATUS_BAR_TEXT_COLOR_HEX (0x002329)
#endif

#ifndef COMP_STATUS_BAR_ICON_COLOR_HEX
#define COMP_STATUS_BAR_ICON_COLOR_HEX (0x002329)
#endif

typedef struct {
  /* valid flags allow partial updates without touching other fields */
  bool wifi_valid;
  bool wifi_connected;
  bool bt_valid;
  bool bt_connected;
  const char *time_text;
  const char *date_text;
} comp_system_status_bar_state_t;

void comp_system_status_bar_create(void);
void comp_system_status_bar_set_alive(unsigned long alive_sec);
void comp_system_status_bar_set_mock(const char *time_text, const char *date_text);
void comp_system_status_bar_set_connectivity(bool wifi_connected,
                                             bool bt_connected);
void comp_system_status_bar_update(const comp_system_status_bar_state_t *state);

#endif
