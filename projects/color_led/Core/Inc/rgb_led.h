/*
 * rgb_led.h - RGB LED PWM Driver
 *
 * Hardware PWM using TIM4 (PB6, PB7, PB8)
 */

#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

/*
 * GPIO Configuration (TIM4 channels)
 *
 * PB6: TIM4_CH1 → Red
 * PB7: TIM4_CH2 → Green
 * PB8: TIM4_CH3 → Blue
 */

/* PWM frequency in Hz */
#define RGB_PWM_FREQ    1000

/* Initialize RGB LED PWM */
void rgb_led_init(void);

/* Set RGB color (0-255 for each channel) */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif /* RGB_LED_H */
