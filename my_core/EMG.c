#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_adc.h"
#include "EMG.h"

ADC_HandleTypeDef hadc2;

void EMG_Init()
{
    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.NbrOfConversion = 1;
    hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;


    if(HAL_ADC_Init(&hadc2) != HAL_OK)
    {
    }

    if(HAL_ADCEx_Calibration_Start(&hadc2,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED) != HAL_OK)
    {
    }

    if(HAL_ADC_ConfigChannel(&hadc2,&sConfig) != HAL_OK)
    {  
    }

}


uint16_t EMG_ReadRaw()
{
    uint16_t value = 0;

    if(HAL_ADC_Start(&hadc2) != HAL_OK)
    {
        return UINT16_MAX;
    }

    if(HAL_ADC_PollForConversion(&hadc2,10) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc2);
        return UINT16_MAX;
    }

    value = (uint16_t)HAL_ADC_GetValue(&hadc2);
    
    HAL_ADC_Stop(&hadc2);

    return value;
}
