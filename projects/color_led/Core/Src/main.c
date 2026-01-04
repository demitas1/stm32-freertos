/*
 * main.c - Rainbow LED with FreeRTOS on STM32F411CE (Black Pill)
 *
 * This example creates an RGB LED rainbow effect using software PWM.
 * LED GPIO Configuration (defined in rgb_led.h):
 *   - PB0 = Blue
 *   - PB1 = Green
 *   - PB2 = Red
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rgb_led.h"
#include "hsv.h"

#define LED_PIN         GPIO_PIN_13
#define LED_GPIO_PORT   GPIOC

/* LED is active low on Black Pill */
#define LED_ON()        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF()       HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_TOGGLE()    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)

/* Private function prototypes */
static void SystemClock_Config(void);
static void Rainbow_Task(void *pvParameters);

/* Task handles */
TaskHandle_t rainbowTaskHandle = NULL;

/* Timer handle (defined in rgb_led.c) */
extern TIM_HandleTypeDef htim2;

/**
 * @brief  Application entry point
 */
int main(void)
{
    /* Reset of all peripherals, Initialize Flash interface and Systick */
    HAL_Init();

    /* Configure the system clock to 100 MHz */
    SystemClock_Config();

    /* Initialize PC13 for status LED */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    LED_OFF();

    /* Initialize RGB LED with PWM */
    rgb_led_init();

    /* Create rainbow effect task */
    xTaskCreate(
        Rainbow_Task,       /* Task function */
        "Rainbow",          /* Task name */
        256,                /* Stack size (words) */
        NULL,               /* Parameters */
        2,                  /* Priority */
        &rainbowTaskHandle  /* Task handle */
    );

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    while (1)
    {
    }
}

/**
 * @brief  Rainbow LED effect task
 *
 * Cycles through HSV color space to create a rainbow effect.
 */
static void Rainbow_Task(void *pvParameters)
{
    (void)pvParameters;

    uint16_t hue = 0;
    uint8_t r, g, b;
    uint8_t toggle_counter = 0;

    for (;;)
    {
        /* Convert HSV to RGB (full saturation and value) */
        hsv_to_rgb(hue, 255, 255, &r, &g, &b);

        /* Set LED color */
        rgb_led_set_color(r, g, b);

        /* Increment hue for rainbow effect */
        hue = (hue + 1) % 360;

        /* Toggle PC13 every ~1 second (20 * 50ms) */
        toggle_counter++;
        if (toggle_counter >= 20)
        {
            LED_TOGGLE();
            toggle_counter = 0;
        }

        /* Update every 50ms for smooth color transition */
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * @brief  System Clock Configuration
 *         HSE = 25MHz -> PLL -> SYSCLK = 100MHz
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure power supply */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Configure HSE and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;       /* HSE / 25 = 1MHz */
    RCC_OscInitStruct.PLL.PLLN = 200;      /* 1MHz * 200 = 200MHz VCO */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  /* 200MHz / 2 = 100MHz */
    RCC_OscInitStruct.PLL.PLLQ = 4;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  /* 50MHz max for APB1 */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  /* 100MHz for APB2 */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  TIM2 interrupt handler
 */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/**
 * @brief  Timer period elapsed callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        rgb_led_pwm_update();
    }
}

/**
 * @brief  Error handler
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        /* Stay here */
    }
}

/**
 * @brief  FreeRTOS malloc failed hook
 */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;);
}

/**
 * @brief  FreeRTOS stack overflow hook
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for (;;);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif
