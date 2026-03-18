/**
  ******************************************************************************
  * @file    stm32h7xx_hal_msp.c
  * @brief   HAL MSP 初始化/反初始化
  *          UART MSP 已移至 usart.c 中 (HAL_UART_MspInit / MspDeInit)
  ******************************************************************************
  */
#include "main.h"

/**
 * @brief HAL 全局 MSP 初始化 (由 HAL_Init() 调用)
 */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}
