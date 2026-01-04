/*
 * main.h - LED Blink with FreeRTOS
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/* Exported functions prototypes */
void Error_Handler(void);

/* LED Pin Definition (PC13 for Black Pill) */
#define LED_PIN         GPIO_PIN_13
#define LED_GPIO_PORT   GPIOC

/* LED is active low on Black Pill */
#define LED_ON()        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF()       HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_TOGGLE()    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
