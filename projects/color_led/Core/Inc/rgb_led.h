/*
 * rgb_led.h - RGB LED PWM Driver
 *
 * Software PWM implementation for flexible GPIO configuration.
 */

#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

/*
 * GPIO Configuration - Modify these to change LED pins
 *
 * Default: PB0=Blue, PB1=Green, PB2=Red
 */
#define LED_RED_PORT    GPIOB
#define LED_RED_PIN     GPIO_PIN_2
#define LED_GREEN_PORT  GPIOB
#define LED_GREEN_PIN   GPIO_PIN_1
#define LED_BLUE_PORT   GPIOB
#define LED_BLUE_PIN    GPIO_PIN_0

/* PWM frequency in Hz (affects flicker visibility) */
#define RGB_PWM_FREQ    100

/* Initialize RGB LED PWM */
void rgb_led_init(void);

/* Set RGB color (0-255 for each channel) */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

/* PWM update function - called from timer interrupt */
void rgb_led_pwm_update(void);

#endif /* RGB_LED_H */
