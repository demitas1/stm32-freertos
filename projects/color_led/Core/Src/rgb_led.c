/*
 * rgb_led.c - RGB LED PWM Driver
 *
 * Hardware PWM using TIM4:
 *   PB6: TIM4_CH1 → Red
 *   PB7: TIM4_CH2 → Green
 *   PB8: TIM4_CH3 → Blue
 */

#include "rgb_led.h"
#include "main.h"

/* Timer handle */
static TIM_HandleTypeDef htim4;

/**
 * @brief Initialize RGB LED GPIOs and TIM4 PWM
 */
void rgb_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* Enable clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* Configure PB6, PB7, PB8 as TIM4 PWM outputs (AF2) */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure TIM4 for PWM
     * APB1 timer clock = 100MHz (APB1 = 50MHz, timer x2)
     * PWM frequency = 1kHz
     * Period = 255 (8-bit resolution)
     * Prescaler = 100MHz / (1000Hz * 256) - 1 = 390
     */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 390;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 255;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim4);

    /* Configure PWM channels */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    /* Channel 1 - Red (PB6) */
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);

    /* Channel 2 - Green (PB7) */
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);

    /* Channel 3 - Blue (PB8) */
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);

    /* Start PWM on all channels */
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

/**
 * @brief Set RGB LED color
 * @param red   Red intensity (0-255)
 * @param green Green intensity (0-255)
 * @param blue  Blue intensity (0-255)
 */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, red);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, green);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, blue);
}
