/*******************************************************************************
 * File Name:   gpio.h
 *
 * Description: AIC-EEC GPIO API - PC Simulator Mock Version
 *              For LVGL UI Testing on MacOS/Windows/Linux
 *
 * Note: This is a mock implementation for PC simulation.
 *       LED/PWM values are stored in memory for UI display.
 *
 ******************************************************************************/

#ifndef AIC_GPIO_H
#define AIC_GPIO_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * LED Definitions
 ******************************************************************************/

typedef enum {
    AIC_LED_RED = 0,
    AIC_LED_GREEN,
    AIC_LED_BLUE,
    AIC_LED_COUNT
} aic_led_t;

/*******************************************************************************
 * Button Definitions
 ******************************************************************************/

typedef enum {
    AIC_BTN_USER = 0,   /**< User button 1 (SW2) */
    AIC_BTN_USER2,      /**< User button 2 (SW4) */
    AIC_BTN_COUNT
} aic_btn_t;

/*******************************************************************************
 * GPIO Functions (Mock Implementation)
 ******************************************************************************/

/**
 * @brief Initialize GPIO (no-op on PC)
 */
void aic_gpio_init(void);

/**
 * @brief Set LED state
 * @param led LED identifier
 * @param state true = ON, false = OFF
 */
void aic_gpio_led_set(aic_led_t led, bool state);

/**
 * @brief Toggle LED state
 * @param led LED identifier
 */
void aic_gpio_led_toggle(aic_led_t led);

/**
 * @brief Get LED state
 * @param led LED identifier
 * @return true = ON, false = OFF
 */
bool aic_gpio_led_get(aic_led_t led);

/**
 * @brief Read button state
 * @param btn Button identifier
 * @return true = pressed, false = released
 */
bool aic_gpio_btn_read(aic_btn_t btn);

/*******************************************************************************
 * PWM Functions (Mock Implementation)
 ******************************************************************************/

/**
 * @brief Initialize PWM for LED (no-op on PC)
 * @param led LED to use for PWM
 */
void aic_gpio_pwm_init(aic_led_t led);

/**
 * @brief Set PWM brightness
 * @param led LED identifier
 * @param brightness 0-100%
 */
void aic_gpio_pwm_set_brightness(aic_led_t led, uint8_t brightness);

/**
 * @brief Get current PWM brightness
 * @param led LED identifier
 * @return Current brightness 0-100%
 */
uint8_t aic_gpio_pwm_get_brightness(aic_led_t led);

/*******************************************************************************
 * Simulator-Specific Functions
 ******************************************************************************/

/**
 * @brief Set simulated button state (for UI control)
 * @param btn Button identifier
 * @param pressed true = pressed, false = released
 */
void aic_gpio_sim_set_button(aic_btn_t btn, bool pressed);

#endif /* AIC_GPIO_H */
