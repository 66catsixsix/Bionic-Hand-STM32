#include "stdio.h"
#include "flex.h"
#include "lvgl.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "stdint.h"
#include "algo.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


//calibration
uint16_t calib_max = 0;
uint16_t calib_min = 4095;

//DMA
#define FLEX_CH_NUM 10    //test
ALIGN_32BYTES(volatile uint16_t flex_adc_values[FLEX_CH_NUM]);




void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(adcHandle->Instance==ADC1)
    {
        __HAL_RCC_ADC12_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        
       
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
         | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);

        //DMA
        hdma_adc1.Instance = DMA1_Stream1;
        hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc =DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode = DMA_CIRCULAR;
        hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
        hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        
        if(HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
        }
        
        __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);
    }
    else if(adcHandle->Instance == ADC2)
{
    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
}

void flex_init()
{
        hadc1.Instance = ADC1;
        hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
        hadc1.Init.Resolution = ADC_RESOLUTION_12B;
        hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
        hadc1.Init.ContinuousConvMode = ENABLE;
        hadc1.Init.DiscontinuousConvMode = DISABLE;
        hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc1.Init.NbrOfConversion = FLEX_CH_NUM;
        hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
        hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
        hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

        if (HAL_ADC_Init(&hadc1) != HAL_OK)
        {
        }
            if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
        {
     
    }
 
        //PA0 channel16  食指
        ADC_ChannelConfTypeDef sConfig = {0};
        sConfig.Channel = ADC_CHANNEL_16; 
        sConfig.Rank = ADC_REGULAR_RANK_1;
        //sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5; //采样时间
        //sConfig.SamplingTime = ADC_SAMPLETIME_387CYCLES_5;
        sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

       /*
        对照 按照RANK顺序填写：
        PS*   RANK数字 -1 = scan数字，scan填写多少，
        就代表该手部控制哪个手指关节
        Hand[i].target_angle = 2048; 
        Hand[i].adc_scan = 255;   
        Hand[i].calib_min = 0;       
        Hand[i].calib_max = 4095;    

        Hand[Finger_Index].adc_scan = 0;
        Hand[Finger_Middle].adc_scan = 1;
        Hand[Finger_Ring].adc_scan = 255;
        Hand[Finger_Little].adc_scan = 255;
        Hand[Finger_Thumb].adc_scan = 255;

        Hand[Finger_Index_L].adc_scan = 5;
        Hand[Finger_Middle_L].adc_scan = 255;
        Hand[Finger_Ring_L].adc_scan = 255;
        Hand[Finger_Litte_L].adc_scan = 255;
        Hand[Finger_Thumb_L].adc_scan = 255;
        */
       
    //PA1 channel17    食指小关节
    sConfig.Channel = ADC_CHANNEL_17;
    sConfig.Rank = ADC_REGULAR_RANK_2;    
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PA2 channel14    中指
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PA3 channel15      中指小关节
    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {    }

    //PA4 channel18       无名指
    sConfig.Channel = ADC_CHANNEL_18;
    sConfig.Rank = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PA5 channel19     无名指小关节
    sConfig.Channel = ADC_CHANNEL_19;
    sConfig.Rank = ADC_REGULAR_RANK_6;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PA6 channel3      小指
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_7;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PA7 channel7      小指小关节
    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_8;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PC4 channel4      拇指
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_9;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
    }

    //PC5 channel8       拇指小关节
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = ADC_REGULAR_RANK_10;
    if(HAL_ADC_ConfigChannel(&hadc1,&sConfig) != HAL_OK)
    {

    }

 

    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)flex_adc_values,FLEX_CH_NUM);

}

//阻塞轮询
uint16_t flex_read_adc(void)
{
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 10);
        uint16_t value = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return value;
}
//DMA
uint16_t get_flex_channel_adc(uint8_t ch_index)
{
    if(ch_index >=FLEX_CH_NUM)return 0;
    SCB_InvalidateDCache_by_Addr((uint32_t*)flex_adc_values,32);
    //重新读取缓存

    return flex_adc_values[ch_index];
    //index为通道结果位置
    //rank为扫描位置
}

// //单根标定程序
// void flex_calibration(void)
// {
//         uint16_t calib_compare = flex_read_adc();
//         if(calib_compare > calib_max)
//         {
//             calib_max = calib_compare;
//         }
//         if(calib_compare < calib_min)
//         {
//             calib_min = calib_compare;
//         }

// }

// #include "stdio.h"
// #include "flex.h"
// #include "lvgl.h"
// #include "stm32h7xx_hal.h"
// #include "stm32h7xx_hal_adc.h"
// #include "stdint.h"
// #include "algo.h"

// ADC_HandleTypeDef hadc1;

// // 全局变量不再需要 DMA 的数组了

// void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     if(adcHandle->Instance == ADC1)
//     {
//         __HAL_RCC_ADC12_CLK_ENABLE();
//         __HAL_RCC_GPIOA_CLK_ENABLE();
//         // PA0 -> ADC1_INP16  (1号舵机传感器)
//         // PA1 -> ADC1_INP17  (6号舵机传感器)
//         GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
//         GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//         GPIO_InitStruct.Pull = GPIO_NOPULL;
//         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//     }
// }

// void flex_init()
// {
//     hadc1.Instance = ADC1;
//     hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//     hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    
//     // 【关键：关闭 DMA 和多通道扫描，配置为单次触发模式】
//     hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;     
//     hadc1.Init.ContinuousConvMode = DISABLE;        
//     hadc1.Init.DiscontinuousConvMode = DISABLE;
//     hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//     hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//     hadc1.Init.NbrOfConversion = 1;                 
//     hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//     hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR; // 普通数据寄存器
//     hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

//     if (HAL_ADC_Init(&hadc1) != HAL_OK)
//     {
//     }
//     // 校准
// 
// }

// // 废弃旧接口（但保留防止外部报错）
// uint16_t flex_read_adc(void)
// {
//     return 0;
// }


// uint16_t get_flex_channel_adc(uint8_t ch_index)
// {
//     ADC_ChannelConfTypeDef sConfig = {0};
    
//     // 动态切换通道（非常快）
//     if(ch_index == 0) sConfig.Channel = ADC_CHANNEL_16;      // 通道 0 对应 PA0
//     else if(ch_index == 1) sConfig.Channel = ADC_CHANNEL_17; // 通道 1 对应 PA1
//     else return 0; // 无效通道直接返回 0

//     sConfig.Rank = ADC_REGULAR_RANK_1;
//     sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5; 
//     sConfig.SingleDiff = ADC_SINGLE_ENDED;
//     sConfig.OffsetNumber = ADC_OFFSET_NONE;
//     sConfig.Offset = 0;
    
//     // 配置刚刚选中的通道给 ADC
//     HAL_ADC_ConfigChannel(&hadc1, &sConfig);

//     // 触发并极速等待 (H7只需零点几微秒)
//     HAL_ADC_Start(&hadc1);
//     HAL_ADC_PollForConversion(&hadc1, 10);
//     uint16_t value = HAL_ADC_GetValue(&hadc1);
//     HAL_ADC_Stop(&hadc1);

//     return value;
// }