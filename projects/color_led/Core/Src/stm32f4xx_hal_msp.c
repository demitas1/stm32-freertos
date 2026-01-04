/*
 * stm32f4xx_hal_msp.c - HAL MSP (MCU Support Package) callbacks
 */

#include "main.h"

/**
 * @brief  Initialize the Global MSP
 */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
