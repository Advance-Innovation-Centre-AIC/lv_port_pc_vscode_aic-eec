/*******************************************************************************
 * File Name:   gpio.c
 *
 * Description: AIC-EEC GPIO Hardware Abstraction Layer - PC Simulator Mock
 *              Implementation. All state is stored in memory arrays.
 *              API matches the PSoC Edge E84 version exactly.
 *
 ******************************************************************************/

#include "gpio.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * Static Variables (Mock State)
 ******************************************************************************/

static bool led_states[AIC_LED_COUNT];
static uint8_t led_brightness[AIC_LED_COUNT];
static bool btn_states[AIC_BTN_COUNT];
static bool btn_was_pressed[AIC_BTN_COUNT];
static aic_button_callback_t btn_callbacks[AIC_BTN_COUNT];

/*******************************************************************************
 * GPIO Initialization
 ******************************************************************************/

bool aic_gpio_init(void)
{
    memset(led_states, 0, sizeof(led_states));
    memset(led_brightness, 0, sizeof(led_brightness));
    memset(btn_states, 0, sizeof(btn_states));
    memset(btn_was_pressed, 0, sizeof(btn_was_pressed));
    memset(btn_callbacks, 0, sizeof(btn_callbacks));
    printf("[GPIO] Mock initialized\n");
    return true;
}

void aic_gpio_deinit(void)
{
    memset(led_states, 0, sizeof(led_states));
    memset(led_brightness, 0, sizeof(led_brightness));
    memset(btn_states, 0, sizeof(btn_states));
    memset(btn_was_pressed, 0, sizeof(btn_was_pressed));
    memset(btn_callbacks, 0, sizeof(btn_callbacks));
    printf("[GPIO] Mock deinitialized\n");
}

/*******************************************************************************
 * LED Control Functions
 ******************************************************************************/

void aic_gpio_led_set(aic_led_t led, bool state)
{
    if (led >= AIC_LED_COUNT) return;
    led_states[led] = state;
    led_brightness[led] = state ? 100 : 0;
    printf("[GPIO] LED %s: %s\n", aic_gpio_led_name(led), state ? "ON" : "OFF");
}

void aic_gpio_led_toggle(aic_led_t led)
{
    if (led >= AIC_LED_COUNT) return;
    led_states[led] = !led_states[led];
    led_brightness[led] = led_states[led] ? 100 : 0;
    printf("[GPIO] LED %s: %s\n", aic_gpio_led_name(led),
           led_states[led] ? "ON" : "OFF");
}

bool aic_gpio_led_get(aic_led_t led)
{
    if (led >= AIC_LED_COUNT) return false;
    return led_states[led];
}

void aic_gpio_led_all_on(void)
{
    for (int i = 0; i < AIC_LED_COUNT; i++) {
        aic_gpio_led_set((aic_led_t)i, true);
    }
}

void aic_gpio_led_all_off(void)
{
    for (int i = 0; i < AIC_LED_COUNT; i++) {
        aic_gpio_led_set((aic_led_t)i, false);
    }
}

/*******************************************************************************
 * PWM LED Control Functions
 ******************************************************************************/

bool aic_gpio_pwm_init(aic_led_t led)
{
    if (led >= AIC_LED_COUNT) return false;
    printf("[GPIO] PWM initialized for LED %s\n", aic_gpio_led_name(led));
    return true;
}

void aic_gpio_pwm_set_brightness(aic_led_t led, uint8_t brightness)
{
    if (led >= AIC_LED_COUNT) return;
    if (brightness > 100) brightness = 100;
    led_brightness[led] = brightness;
    led_states[led] = (brightness > 0);
}

uint8_t aic_gpio_pwm_get_brightness(aic_led_t led)
{
    if (led >= AIC_LED_COUNT) return 0;
    return led_brightness[led];
}

void aic_gpio_pwm_set_frequency(aic_led_t led, uint32_t freq_hz)
{
    if (led >= AIC_LED_COUNT) return;
    printf("[GPIO] PWM frequency set to %u Hz for LED %s\n",
           (unsigned int)freq_hz, aic_gpio_led_name(led));
}

void aic_gpio_pwm_deinit(aic_led_t led)
{
    if (led >= AIC_LED_COUNT) return;
    led_brightness[led] = 0;
    printf("[GPIO] PWM deinitialized for LED %s\n", aic_gpio_led_name(led));
}

/*******************************************************************************
 * Button Input Functions
 ******************************************************************************/

bool aic_gpio_button_read(aic_button_t btn)
{
    /* No real debounce on PC simulator, same as raw read */
    return aic_gpio_button_read_raw(btn);
}

bool aic_gpio_button_read_raw(aic_button_t btn)
{
    if (btn >= AIC_BTN_COUNT) return false;
    return btn_states[btn];
}

bool aic_gpio_button_set_callback(aic_button_t btn, aic_button_callback_t callback)
{
    if (btn >= AIC_BTN_COUNT) return false;
    btn_callbacks[btn] = callback;
    return true;
}

void aic_gpio_button_clear_callback(aic_button_t btn)
{
    if (btn >= AIC_BTN_COUNT) return;
    btn_callbacks[btn] = NULL;
}

bool aic_gpio_button_was_pressed(aic_button_t btn)
{
    if (btn >= AIC_BTN_COUNT) return false;
    bool was = btn_was_pressed[btn];
    btn_was_pressed[btn] = false;
    return was;
}

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/

const char* aic_gpio_led_name(aic_led_t led)
{
    switch (led) {
        case AIC_LED_RED:   return "Red";
        case AIC_LED_GREEN: return "Green";
        case AIC_LED_BLUE:  return "Blue";
        default:            return "Unknown";
    }
}

const char* aic_gpio_button_name(aic_button_t btn)
{
    switch (btn) {
        case AIC_BTN_USER:  return "User";
        case AIC_BTN_USER2: return "User2";
        default:            return "Unknown";
    }
}

void aic_gpio_led_demo(uint8_t cycles, uint32_t delay_ms)
{
    printf("[GPIO] LED demo: %u cycles, %u ms delay (simulated, no real delay)\n",
           (unsigned int)cycles, (unsigned int)delay_ms);
}

/*******************************************************************************
 * PC Simulator Only
 ******************************************************************************/

void aic_gpio_sim_set_button(aic_button_t btn, bool pressed)
{
    if (btn >= AIC_BTN_COUNT) return;
    btn_states[btn] = pressed;
    if (pressed) {
        btn_was_pressed[btn] = true;
        if (btn_callbacks[btn] != NULL) {
            btn_callbacks[btn]();
        }
    }
}
