/*
 * main.c - LED Blink with FreeRTOS on STM32F411CE (Black Pill)
 *
 * This example creates two FreeRTOS tasks:
 * - LED Task: Blinks the onboard LED (PC13)
 * - Heartbeat Task: Demonstrates multiple task execution
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

/* Private function prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void LED_Task(void *pvParameters);
static void Heartbeat_Task(void *pvParameters);

/* Task handles */
TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t heartbeatTaskHandle = NULL;

/**
 * @brief  Application entry point
 */
int main(void)
{
    /* Reset of all peripherals, Initialize Flash interface and Systick */
    HAL_Init();

    /* Configure the system clock to 100 MHz */
    SystemClock_Config();

    /* Initialize GPIO */
    GPIO_Init();

    /* Create LED blink task */
    xTaskCreate(
        LED_Task,           /* Task function */
        "LED",              /* Task name */
        128,                /* Stack size (words) */
        NULL,               /* Parameters */
        2,                  /* Priority */
        &ledTaskHandle      /* Task handle */
    );

    /* Create heartbeat task */
    xTaskCreate(
        Heartbeat_Task,
        "Heartbeat",
        128,
        NULL,
        1,                  /* Lower priority */
        &heartbeatTaskHandle
    );

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    while (1)
    {
    }
}

/**
 * @brief  LED blink task - toggles LED every 500ms
 */
static void LED_Task(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        LED_TOGGLE();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief  Heartbeat task - demonstrates concurrent execution
 */
static void Heartbeat_Task(void *pvParameters)
{
    (void)pvParameters;
    uint32_t counter = 0;

    for (;;)
    {
        counter++;
        /* This task runs at lower priority */
        /* Could be used for status monitoring, communication, etc. */
        vTaskDelay(pdMS_TO_TICKS(2000));
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
 * @brief  GPIO Initialization - PC13 as output for LED
 */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOC clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Configure LED pin (PC13) */
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    /* Start with LED off */
    LED_OFF();
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
