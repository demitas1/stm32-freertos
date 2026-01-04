/*
 * rgb_led.c - RGB LED PWM Driver
 *
 * Uses TIM2 for software PWM to allow flexible GPIO configuration.
 * Any GPIO pin can be used for R, G, B LEDs.
 */

#include "rgb_led.h"
#include "main.h"

/* PWM state */
static volatile uint8_t pwm_counter = 0;
static volatile uint8_t duty_red = 0;
static volatile uint8_t duty_green = 0;
static volatile uint8_t duty_blue = 0;

/* Timer handle */
TIM_HandleTypeDef htim2;

/**
 * @brief Initialize RGB LED GPIOs and PWM timer
 */
void rgb_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure LED pins as outputs */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    /* Red LED */
    GPIO_InitStruct.Pin = LED_RED_PIN;
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);

    /* Green LED */
    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);

    /* Blue LED */
    GPIO_InitStruct.Pin = LED_BLUE_PIN;
    HAL_GPIO_Init(LED_BLUE_PORT, &GPIO_InitStruct);

    /* Start with LEDs off */
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);

    /* Configure TIM2 for PWM timing
     * APB1 clock = 50MHz (SYSCLK/2)
     * Timer clock = 100MHz (APB1 timer clock x2 when prescaler > 1)
     *
     * For 100Hz PWM with 256 steps:
     * Update rate = 100Hz * 256 = 25600 Hz
     * 100MHz / 100 / 39 ≈ 25641 Hz
     */
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 99;    /* 100MHz / 100 = 1MHz */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 38;       /* 1MHz / 39 ≈ 25.6kHz */
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);

    /* Enable TIM2 interrupt */
    HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    /* Start timer */
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief Set RGB LED color
 * @param red   Red intensity (0-255)
 * @param green Green intensity (0-255)
 * @param blue  Blue intensity (0-255)
 */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    duty_red = red;
    duty_green = green;
    duty_blue = blue;
}

/**
 * @brief PWM update - call from TIM2 interrupt
 *
 * Implements software PWM by comparing counter with duty cycle.
 */
void rgb_led_pwm_update(void)
{
    pwm_counter++;

    /* Red LED */
    if (pwm_counter < duty_red) {
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
    }

    /* Green LED */
    if (pwm_counter < duty_green) {
        HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
    }

    /* Blue LED */
    if (pwm_counter < duty_blue) {
        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
    }
}
