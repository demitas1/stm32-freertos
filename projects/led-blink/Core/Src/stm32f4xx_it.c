/*
 * stm32f4xx_it.c - Interrupt handlers
 */

#include "main.h"
#include "stm32f4xx_it.h"

/**
 * @brief  NMI Handler
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief  Hard Fault Handler
 */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief  Memory Management Fault Handler
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief  Bus Fault Handler
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief  Usage Fault Handler
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/*
 * Note: SVC_Handler, PendSV_Handler, and SysTick_Handler
 * are provided by FreeRTOS port layer.
 * See FreeRTOSConfig.h for the mapping.
 */
