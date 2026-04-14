/**
 * usb_hid_joystick.h — PC simulator stub (no USB HID on desktop)
 */
#ifndef USB_HID_JOYSTICK_H
#define USB_HID_JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool connected;
    uint16_t sequence;
    uint8_t report[8];
} joystick_state_t;

static inline void usb_hid_joystick_init(void) {}
static inline bool usb_hid_joystick_request_init(void) { return false; }
static inline const joystick_state_t *usb_hid_joystick_get_state(void)
{
    static joystick_state_t dummy = {0};
    return &dummy;
}
static inline bool usb_hid_joystick_is_connected(void) { return false; }

#endif
