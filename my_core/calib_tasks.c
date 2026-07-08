#include "FreeRTOS.h"
#include "algo.h"
#include "Finger.h"
#include "flex.h"
#include "calib_tasks.h"
#include "stdio.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include <stdlib.h>
#include "task.h"
#include "usart.h"
#include "main.h"
#include "flex.h"



#define  capture_MS      5000U
#define  Flex_Setting_MS  1000U
#define  FLEX_SAMPLE_MS   20U
#define  active_Finger    Finger_Middle



extern Fingertask_t Hand[];
extern UART_HandleTypeDef huart3;

//printf封装
static void test_log(const char * fmt, ...)
{
    char buf[256];
    va_list args; //定义一个va_list  获取游标
    int len;
    
   va_start(args,fmt);  //从fmt后面获取多的参数
   len = vsnprintf(buf,sizeof(buf),fmt,args);
   va_end(args);

   if(len < 0) return;
   if(len >= (int)sizeof(buf)) len = (int)sizeof(buf) - 1;

   HAL_UART_Transmit(&huart3,(uint8_t*)buf,(uint16_t)len,100);
   
}

static void printf_capture_state(const char *name,uint16_t avg,uint16_t min_val,uint16_t max_val)
{
    test_log("%s avg=%u min=%u max=%u span=%u\r\n",    name,
             (unsigned int)avg,
             (unsigned int)min_val,
             (unsigned int)max_val,
             (unsigned int)(max_val - min_val));
}


//获取ADC时间区间
static uint16_t cap_adc_avg(uint8_t scan,uint32_t capture_time_ms,uint32_t sample_ms, uint16_t * min_out, uint16_t * max_out)
{
    uint32_t sample_sum =0;
    uint32_t sample_count = 0;
    uint16_t sample_min = 4095;
    uint16_t sample_max = 0;
    //uint32_t start_time = HAL_GetTick();
    TickType_t start_time = xTaskGetTickCount();  //获取任务开始时间
    TickType_t wait_time = pdMS_TO_TICKS(capture_time_ms);
    
    while((xTaskGetTickCount() - start_time < wait_time))
    {
        uint16_t raw = get_flex_channel_adc(scan);

        if(raw < sample_min)
        {
            sample_min = raw;     //最小值
        }
        if(raw > sample_max)
        {
            sample_max = raw;   //最大值
        }
      //avg
    sample_sum +=  raw;
    //计数值
    sample_count++;
    vTaskDelay(pdMS_TO_TICKS(sample_ms));
    }

    if(sample_count == 0U)
    {
        sample_count = 1U;
    }
    if(min_out)
    {
        *min_out = sample_min;
    }
    if(max_out)
    {
        *max_out = sample_max;
    }
    //防止没有采集到count计数

    uint16_t sample_arg = sample_sum  /  sample_count;

    return (uint16_t)sample_arg;
}


uint16_t abs_diff(uint16_t a, uint16_t b)
{   
    if(a<=b)
    {
        return (uint16_t)(b - a);
    }
    return (uint16_t)(a - b);

}

extern volatile uint8_t uart3_log_busy;
/*标定层*/
void flex_calib_task(void * arg)
{
    (void)arg;
    uart3_log_busy = 1;

    //第一段打开时间测得的平均值
    uint16_t open1_avg;
    uint16_t open1_min;
    uint16_t open1_max;
    //手指闭合一段时间测得的平均值
    uint16_t close_avg;
    uint16_t close_min;
    uint16_t close_max;
    //第二段手掌打开测得的平均值
    uint16_t open2_avg;
    uint16_t open2_min;
    uint16_t open2_max;
    //张开和弯曲状态的ADC差值：测试手掌活动的动态范围
    uint16_t open1_close;
    uint16_t open2_close;
    uint16_t open1_open2;

    Fingertask_t * cfg = &Hand[active_Finger];
    vTaskDelay(pdMS_TO_TICKS(1500));


    test_log("ADC Channel:%u\r\n",(unsigned int) cfg -> adc_scan);
    test_log("Open Your Hand %lu ms\r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));

    open1_avg = cap_adc_avg(cfg->adc_scan,capture_MS,FLEX_SAMPLE_MS,&open1_min,&open1_max);
    printf_capture_state("",open1_avg,open1_min,open1_max);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));

    test_log("Close Hand... %lu ms\r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));
    close_avg = cap_adc_avg(cfg->adc_scan,capture_MS,FLEX_SAMPLE_MS,&close_min,&close_max);
    printf_capture_state("",close_avg,close_min,close_max);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));

    test_log("Open...%lu ms\r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));
    open2_avg = cap_adc_avg(cfg->adc_scan,capture_MS,FLEX_SAMPLE_MS,&open2_min,&open2_max);
    printf_capture_state("",open2_avg,open2_min,open2_max);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));

    open1_close = abs_diff(open1_avg,close_avg);
    open2_close = abs_diff(open2_avg,close_avg);
    //活动区间
    open1_open2 =abs_diff(open1_avg,open2_avg); 

    test_log("open1_close=%u open2_close=%u open1_open2=%u\r\n",
        (unsigned int)open1_close,
        (unsigned int)open2_close,
        (unsigned int)open1_open2
    );

    uint16_t open_ref;
    uint16_t close_ref;

    open_ref =(uint16_t)(((uint32_t)open1_avg + (uint32_t)open2_avg) / 2U); 
    close_ref = close_avg;

    uint8_t ok = 1;
    uint8_t useable = 1;

    if(open1_open2 < 20U)
    {
        test_log("Return to position with good\r\n");
    }
    else if(open1_open2 < 100U)
    {
        test_log("The return position is slightly off; check for hysteresis or offset\r\n");
        ok = 0;
    }
    else
    {
        test_log("The back position error is too large, and the equipment cannot function properly\r\n");
        ok = 0;
        useable = 0;
    }


    //范围
    if(open1_close < 20U || open2_close < 20U)
    {
        test_log("The range difference is too small, there is a noise problem, and the equipment cannot be used\r\n");
        ok = 0;
        useable = 0;
    }
    else if(open1_close < 80U || open2_close < 80U)
    {
        test_log("The difference is too small and easily affected by interference\r\n");
        ok = 0;
    }
    else
    {
        test_log("Dynamic range available\r\n");
    }

//
    if(useable)
    {
        cfg -> adc_open_ref = open_ref;
        cfg -> adc_close_ref = close_ref;
        cfg -> calib_ok = 1;

        if(ok)
        {
            test_log("Calibration completed, equipment can be used\r\n");
        }
        else
        {
            test_log("Calibration completed but equipment quality is average, it is recommended to recalibrate\r\n");
        }
    }
    else
    {
        test_log("Calibration failed, device cannot be used\r\n");
        cfg -> calib_ok = 0;
    }
    uart3_log_busy = 0;
    vTaskSuspend(NULL);

    }

    /*==============================================*/
typedef struct 
{
    uint16_t avg[Finger_NUM_ALL];   //第i根手指一段时间的平均值
    uint16_t min_val[Finger_NUM_ALL];//第i根手指这一段时间的最小值
    uint16_t max_val[Finger_NUM_ALL];//第i根手指这一段时间的最大值
    uint32_t count[Finger_NUM_ALL];//第i根手采集了多少次
}FlexAllCapture_t;

static uint8_t flex_enable(FInger_ID_t id)
{
    return(Hand[id].adc_scan != 255U);
}

static void flex_clear_enable_calib_ok()
{
    uint8_t i;

    for(i=0;i<Finger_NUM_ALL;i++)
    {
        if(flex_enable((FInger_ID_t)i))
        {
            Hand[i].calib_ok = 0;
        }
    }
}


static void flex_all_capture(uint32_t capture_time_ms,uint32_t sample_ms,FlexAllCapture_t * out)
{
    uint32_t sample_sum[Finger_NUM_ALL] = {0};
    TickType_t start_time;
    TickType_t wait_time;
    uint8_t i;

    for(i=0;i<Finger_NUM_ALL;i++)
    {
        out->avg[i] = 0;
        out->count[i] = 0;
        out->max_val[i] = 0;
        out->min_val[i] = 4095;
    }
    start_time = xTaskGetTickCount();
    wait_time = pdMS_TO_TICKS(capture_time_ms);

    while((xTaskGetTickCount() - start_time) < wait_time)
    {
        for(i=0;i<Finger_NUM_ALL;i++){
        if(flex_enable((FInger_ID_t)i) == 0U)
        {
            continue;
        }
        uint16_t raw = get_flex_channel_adc(Hand[i].adc_scan);

        if(raw < out->min_val[i])
        {
            out->min_val[i] = raw;
        }
        if(raw> out->max_val[i])
        {
            out->max_val[i] = raw;
        }

        sample_sum[i] += raw;
        out->count[i]++;
    } 
    vTaskDelay(pdMS_TO_TICKS(sample_ms));
}
        
        for(i=0;i<Finger_NUM_ALL;i++)
    {
        if(out->count[i] == 0)
        {
            out->avg[i] = 0;
            out->min_val[i] = 0;
            out->max_val[i] = 0;
        } 
        else
        {
            out->avg[i] = (uint16_t)(sample_sum[i]/out->count[i]);
        }

    }


}

static void flex_all_print_capture(const char * stage,const FlexAllCapture_t * out )
{   
    uint8_t i;
    for(i=0;i<Finger_NUM_ALL;i++)
    {
        if(flex_enable((FInger_ID_t)i) == 0)
        {
            continue;
        }
        
        test_log("%s ch=%u avg=%u min=%u max=%u span=%u\r\n",stage,(unsigned int)Hand[i].adc_scan,(unsigned int)out->avg[i],(unsigned int)out->min_val[i],(unsigned int)out->max_val[i],(unsigned int)(out->max_val[i] - out->min_val[i]));

    }

}

static void flex_all_result_apply(const FlexAllCapture_t *open1, const FlexAllCapture_t *close,const FlexAllCapture_t *open2)
{
    uint8_t i;

    for(i=0;i<Finger_NUM_ALL;i++)
    {
        if(flex_enable((FInger_ID_t)i) == 0U)
        {
            continue;
        }
        
        uint16_t open1_close;
        uint16_t open2_close;
        uint16_t open1_open2;
        uint16_t open_ref;
        uint16_t close_ref;
        uint8_t ok = 1;
        uint8_t useable = 1;

        open1_close = abs_diff(open1->avg[i],close->avg[i]);
        open2_close = abs_diff(open2->avg[i],close->avg[i]);
        open1_open2 = abs_diff(open1->avg[i],open2->avg[i]);

        open_ref = (uint16_t)(((uint32_t)open1->avg[i] +(uint32_t)open2->avg[i])/2U);
        close_ref = close->avg[i];


        if(open1_open2 < 20U)
    {
        test_log("ch = %u Return to position with good\r\n",(unsigned int)Hand[i].adc_scan);
    }
    else if(open1_open2 < 100U)
    {
        test_log("ch = %u  The return position is slightly off; check for hysteresis or offset\r\n",(unsigned int)Hand[i].adc_scan);
        ok = 0;
    }
    else
    {
        test_log("ch = %u The back position error is too large, and the equipment cannot function properly\r\n",(unsigned int)Hand[i].adc_scan);
        ok = 0;
        useable = 0;
    }


    //范围
    if(open1_close < 20U || open2_close < 20U)
    {
        test_log("ch = %u The range difference is too small, there is a noise problem, and the equipment cannot be used\r\n",(unsigned int)Hand[i].adc_scan);
        ok = 0;
        useable = 0;
    }
    else if(open1_close < 80U || open2_close < 80U)
    {
        test_log("ch = %u The difference is too small and easily affected by interference\r\n",(unsigned int)Hand[i].adc_scan);
        ok = 0;
    }
    else
    {
        test_log("ch = %u Dynamic range available\r\n",(unsigned int)Hand[i].adc_scan);
    }

//
    if(useable)
    {
        Hand[i].adc_open_ref = open_ref;
        Hand[i].adc_close_ref = close_ref;
        Hand[i].calib_ok = 1;
        if(ok)
        {
            test_log("ch = %u Calibration completed, equipment can be used\r\n",(unsigned int)Hand[i].adc_scan);
        }
        else
        {
            test_log("ch = %u Calibration completed but equipment quality is average, it is recommended to recalibrate\r\n",(unsigned int)Hand[i].adc_scan);
        }
    }
    else
    {
        test_log("ch = %u Calibration failed, device cannot be used\r\n",(unsigned int)Hand[i].adc_scan);
        Hand[i].calib_ok = 0;
    }
    // uart3_log_busy = 0;
    // vTaskSuspend(NULL);

    }

}


void flex_all_calib_task(void *arg)
{
    FlexAllCapture_t open1_capture;
    FlexAllCapture_t close_capture;
    FlexAllCapture_t open2_capture;

    (void)arg;
    uart3_log_busy = 1;
    flex_clear_enable_calib_ok();

    test_log("-------Flex All Calib Start------\r\n");
    vTaskDelay(pdMS_TO_TICKS(1500));

    test_log("Open your Hand %lu ms \r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));
    flex_all_capture(capture_MS,FLEX_SAMPLE_MS,&open1_capture);
    flex_all_print_capture("1 Open",&open1_capture);

    test_log("Close your Hand %lu ms\r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));
    flex_all_capture(capture_MS,FLEX_SAMPLE_MS,&close_capture);
    flex_all_print_capture("2 Close",&close_capture);

    test_log("Open your Hand again %lu ms\r\n",(unsigned long)capture_MS);
    vTaskDelay(pdMS_TO_TICKS(Flex_Setting_MS));
    flex_all_capture(capture_MS,FLEX_SAMPLE_MS,&open2_capture);
    flex_all_print_capture("3 Open",&open2_capture);

    flex_all_result_apply(&open1_capture,&close_capture,&open2_capture);

    test_log("--------Flex Calib End---------");

    uart3_log_busy = 0;
    vTaskSuspend(NULL);

}