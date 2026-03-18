/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : STM32H750XBH6 飞特SCS2332舵机驱动工程头文件
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* -------- 飞特舵机 UART 引脚定义 (反客FK750M5-XBH6) --------
   驱动板有独立 TX/RX，MCU 使用标准全双工 UART
   PA9  → USART1_TX (AF7)
   PA10 → USART1_RX (AF7)
   ----------------------------------------------------------- */
#define FT_UART              USART1

/* -------- LED (反客板 LED 在 PE3, Active-LOW, 可按实际修改) -------- */
#define LED_GPIO_PORT        GPIOC
#define LED_GPIO_PIN         GPIO_PIN_13

void Error_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /* __MAIN_H */
