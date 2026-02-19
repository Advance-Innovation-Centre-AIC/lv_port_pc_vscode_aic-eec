/*******************************************************************************
 * File Name:   gpio.c
 *
 * Description: AIC-EEC GPIO API - PC Simulator Mock Implementation
 *
 ******************************************************************************/

#include "gpio.h"
#include <stdio.h>

/*******************************************************************************
 * Static Variables (Mock State)
 ******************************************************************************/

static bool led_states[AIC_LED_COUNT] = {false, false, false};
static uint8_t led_brightness[AIC_LED_COUNT] = {0, 0, 0};
static bool btn_states[AIC_BTN_COUNT] = {false, false};

/*******************************************************************************
 * GPIO Functions
 ******************************************************************************/

void aic_gpio_init(void)
{
    printf("[GPIO Mock] Initialized\n");
}

void aic_gpio_led_set(aic_led_t led, bool state)
{
    if (led < AIC_LED_COUNT) {
        led_states[led] = state;
        led_brightness[led] = state ? 100 : 0;
    }
}

void aic_gpio_led_toggle(aic_led_t led)
{
    if (led < AIC_LED_COUNT) {
        led_states[led] = !led_states[led];
        led_brightness[led] = led_states[led] ? 100 : 0;
    }
}

bool aic_gpio_led_get(aic_led_t led)
{
    if (led < AIC_LED_COUNT) {
        return led_states[led];
    }
    return false;
}

bool aic_gpio_btn_read(aic_btn_t btn)
{
    if (btn < AIC_BTN_COUNT) {
        return btn_states[btn];
    }
    return false;
}

/*******************************************************************************
 * PWM Functions
 ******************************************************************************/

void aic_gpio_pwm_init(aic_led_t led)
{
    (void)led;
    printf("[GPIO Mock] PWM Initialized for LED%d\n", led);
}

void aic_gpio_pwm_set_brightness(aic_led_t led, uint8_t brightness)
{
    if (led < AIC_LED_COUNT) {
        if (brightness > 100) brightness = 100;
        led_brightness[led] = brightness;
        led_states[led] = (brightness > 0);
    }
}

uint8_t aic_gpio_pwm_get_brightness(aic_led_t led)
{
    if (led < AIC_LED_COUNT) {
        return led_brightness[led];
    }
    return 0;
}

/*******************************************************************************
 * Simulator-Specific Functions
 ******************************************************************************/

void aic_gpio_sim_set_button(aic_btn_t btn, bool pressed)
{
    if (btn < AIC_BTN_COUNT) {
        btn_states[btn] = pressed;
    }
}
