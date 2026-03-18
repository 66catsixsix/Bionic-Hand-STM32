/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   GPIO 初始化 (LED 调试指示灯)
  *          反客FK750M5-XBH6: LED 在 PE3, Active-LOW
  *          如你的板子不同, 修改 main.h 中的 LED_GPIO_PORT/LED_GPIO_PIN
  ******************************************************************************
  */
#include "gpio.h"

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* LED GPIO 时钟使能 (PC13) */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* LED 初始化为高电平 (灭灯, Active-LOW) */
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);

    /* PE3 推挽输出 */
    GPIO_InitStruct.Pin   = LED_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}
