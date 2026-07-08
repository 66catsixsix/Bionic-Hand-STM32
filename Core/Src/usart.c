/**
  ******************************************************************************
  * @file    usart.c
  * @brief   USART1 全双工初始化 — 飞特SCS2332舵机驱动板
  *          引脚: PA9 TX (AF7), PA10 RX (AF7)
  *          驱动板内部自动处理方向切换，MCU使用普通TX/RX双线
  *          波特率: 1 000 000 bps
  ******************************************************************************
  */
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "SCServo.h"
#include "main.h"
#include "algo.h"
#include <string.h>

UART_HandleTypeDef  huart1;
UART_HandleTypeDef huart3;

uint8_t rx_buf[64];
uint8_t rx_index = 0;
uint8_t tx_buf[64];

int16_t angle_flag = 0;
int16_t angle_value = 0;


volatile uint8_t uart3_log_busy = 0;
/**
 * @brief USART1 初始化 (全双工，驱动板自动控制方向)
 */
void MX_USART1_UART_Init(void)
{
    huart1.Instance  = USART1;
        // 建议先用115200，便于与PC串口模块兼容，排查硬件/驱动问题
    huart1.Init.BaudRate    = 1000000;
    huart1.Init.WordLength  = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity  = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling   = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling   = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler  = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart1) != HAL_OK) {
            // 若LED快速闪烁，说明此处初始化失败，常见原因：
            // 1. 管脚PD5/PD6未连接或被占用 2. 波特率过高 3. 硬件损坏
    }

    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

void MX_USART3_UART_Init(void)
{
    huart3.Instance  = USART3;
    huart3.Init.BaudRate    = 115200;
   huart3.Init.WordLength  = UART_WORDLENGTH_8B;
   huart3.Init.StopBits = UART_STOPBITS_1;
     huart3.Init.Parity  = UART_PARITY_NONE;
     huart3.Init.Mode = UART_MODE_TX_RX;
     huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
     huart3.Init.OverSampling   = UART_OVERSAMPLING_16;
     huart3.Init.OneBitSampling   = UART_ONE_BIT_SAMPLE_DISABLE;
     huart3.Init.ClockPrescaler  = UART_PRESCALER_DIV1;
     huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

     if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
        Error_Handler();
    }

}




/**
 * @brief USART1 底层 GPIO / 时钟初始化 (MSP 回调)
 */
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (uartHandle->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* PA9 → USART1_TX, PA10 → USART1_RX
         * 驱动板有独立TX/RX，使用推挽复用模式
         */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
      else if (uartHandle->Instance == USART3)
{
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {   
        char  err_buf[64];
        int err_len = snprintf(err_buf, sizeof(err_buf), "角度超限，请重新输入\r\n");
        if(err_len > 0)
        {
            //HAL_UART_Transmit_IT(&huart3, (uint8_t *)err_buf, (uint16_t)err_len);
        }
        uint8_t ch = rx_buf[rx_index];
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
        
        if (ch == '\n' || ch == '\r')
        {
            rx_buf[rx_index] = 0; 
            int16_t angle = atoi((char*)rx_buf);
            if(angle >= 48 && angle <= 190)
            {
                angle_value = angle; // 只存角度，统一全局
                HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
                angle_flag = 1;
            }

            (void)sprintf((char*)tx_buf, "%d", angle);

            rx_index = 0; // 重新开始接收
            memset(rx_buf, 0, sizeof(rx_buf));
            HAL_UART_Receive_IT(&huart3, &rx_buf[rx_index], 1);
        }
        else
        {
            if (rx_index < sizeof(rx_buf) - 2) // 预留结尾0
                rx_index++;
            else
                rx_index = 0; // 超长保护，强制清零
        }
        // 继续接收下一个字节
        HAL_UART_Receive_IT(&huart3, &rx_buf[rx_index], 1);
    }
    //微雪50-150   
    
    //增加零点校准
    }
