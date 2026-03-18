/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   中断服务函数
  ******************************************************************************
  */
#include "main.h"
#include "stm32h7xx_it.h"
#include "lvgl.h"   /* LVGL 时基 */
#include "lcd_rgb.h" /* 提供 extern LTDC_HandleTypeDef hltdc */
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler( void );

extern LTDC_HandleTypeDef hltdc;

void NMI_Handler(void)
{
    while (1) {}
}

void HardFault_Handler(void)
{
    while (1) {}
}

void MemManage_Handler(void)
{
    while (1) {}
}

void BusFault_Handler(void)
{
    while (1) {}
}

void UsageFault_Handler(void)
{
    while (1) {}
}

/* SVC_Handler / PendSV_Handler 由 FreeRTOS port.c 通过宏重命名提供，此处不重复定义 */

void DebugMon_Handler(void) {}

/**
 * @brief SysTick 中断 (HAL_Delay 依赖)
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
    //lv_tick_inc(portTICK_PERIOD_MS);   /* LVGL 时基，自动匹配 FreeRTOS tick 周期 */
#if ( INCLUDE_xTaskGetSchedulerState == 1 )
    if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
#endif
    {
        xPortSysTickHandler();  /* FreeRTOS tick，驱动任务切换 */
    }
}

/**
 * @brief LTDC 全局中断处理（帧缓冲切换）
 */
void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}

/**
 * @brief LTDC 错误中断处理
 */
void LTDC_ER_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}
