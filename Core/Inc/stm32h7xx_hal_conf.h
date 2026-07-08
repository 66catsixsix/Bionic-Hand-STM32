/**
  ******************************************************************************
  * @file    stm32h7xx_hal_conf.h
  * @brief   HAL 模块使能配置 (STM32H750XBH6 最小集)
  ******************************************************************************
  */
#ifndef __STM32H7xx_HAL_CONF_H
#define __STM32H7xx_HAL_CONF_H

/* ======================== 启用的 HAL 模块 ======================== */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_MDMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_LTDC_MODULE_ENABLED
#define HAL_SDRAM_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED

/* ======================== 晶振 / 时钟 ======================== */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE              25000000UL  /* 反客FK750M5-XBH6: 25MHz 晶振 */
#endif
#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    100U
#endif
#if !defined  (HSI_VALUE)
  #define HSI_VALUE              64000000UL
#endif
#if !defined  (CSI_VALUE)
  #define CSI_VALUE              4000000UL
#endif
#if !defined  (LSI_VALUE)
  #define LSI_VALUE              32000U
#endif
#if !defined  (LSE_VALUE)
  #define LSE_VALUE              32768U
#endif
#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    5000U
#endif
#if !defined  (EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE   12288000U
#endif

/* ======================== SysTick ======================== */
#define  TICK_INT_PRIORITY            (0x0FUL)

/* ======================== 其他可选 ======================== */
#define USE_RTOS                      0U
#define PREFETCH_ENABLE               1U
#define USE_SD_TRANSCEIVER            0U
#define USE_SPI_CRC                   0U

/* Includes -------------------------------------------------------------- */
/**
  * @brief  下述包含顺序经过严格设计，请勿随意调整。
  * 底层的 RCC/GPIO/DMA 必须先于外设（ADC/UART等）包含。
  */

#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32h7xx_hal_rcc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32h7xx_hal_gpio.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32h7xx_hal_dma.h"
  #include "stm32h7xx_hal_dma_ex.h"  // H7系列必须包含此文件
#endif

#ifdef HAL_MDMA_MODULE_ENABLED
  #include "stm32h7xx_hal_mdma.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32h7xx_hal_cortex.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
  #include "stm32h7xx_hal_adc.h"
  #include "stm32h7xx_hal_adc_ex.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32h7xx_hal_flash.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
  #include "stm32h7xx_hal_pwr.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
  #include "stm32h7xx_hal_uart.h"
#endif

#ifdef HAL_USART_MODULE_ENABLED
  #include "stm32h7xx_hal_usart.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
  #include "stm32h7xx_hal_tim.h"
#endif

#ifdef HAL_DMA2D_MODULE_ENABLED
  #include "stm32h7xx_hal_dma2d.h"
#endif

#ifdef HAL_LTDC_MODULE_ENABLED
  #include "stm32h7xx_hal_ltdc.h"
#endif

#ifdef HAL_SDRAM_MODULE_ENABLED
  #include "stm32h7xx_hal_sdram.h"
#endif

/* Assert macro ---------------------------------------------------------- */
#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t *file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif

#endif /* __STM32H7xx_HAL_CONF_H */
