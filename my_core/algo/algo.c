// #include "algo.h"
// #include "SCS.h"
// #include "stdint.h"
// #include "math.h"
// #include "flex.h"

// extern uint16_t calib_max;
// extern uint16_t calib_min;


// /*============================= 舵机控制算法 ================================================ */
// //Global Var
//   int16_t waveshare_angle;
//   int16_t  waveshare_step;
//   int16_t  Step_Res_Get;

// int16_t  Angle_Now_Update()
// {
//     Step_Res_Get = readWord(1,STEP_NOW_RES);
//     waveshare_angle =(int16_t )( ((long)Step_Res_Get * 300) / 1023);
//     return waveshare_angle;
// }


// int16_t  Step_Angle_Update(int16_t Angle_Set)
// {
//     waveshare_step = (int16_t)((long)Angle_Set  *1023/ 300); 
//     return waveshare_step;
// }

// /*============================= flex传感器算法 ================================================ */



// //限位
// int16_t MAP_Flex_To_Update(uint16_t current_adc)
// {
//     if(calib_max == calib_min)
//     {
//         return 49;  
//     }
//     else if(current_adc > calib_max)
//     {
//         current_adc = calib_max;
//     }
//     else if(current_adc < calib_min)
//     {
//         current_adc = calib_min;
//     }
//     //计算线性范围
//     int32_t target_angle = 190 - ( ((int32_t)current_adc - (int32_t)calib_min)*(190-49))/(calib_max - calib_min);

//     return (int16_t)target_angle;

// }


/*====================================4月21重构部分======================================================*/
#include "algo.h"
#include "SCS.h"
#include "stdint.h"
#include "math.h"
#include "flex.h"
#include  "Finger.h"
#include "stddef.h"
#include "stdint.h"


int16_t waveshare_angle;
int16_t  waveshare_step;
int16_t  Step_Res_Get;


int16_t  Step_Angle_Update(int16_t Angle_Set)
{
    waveshare_step = (int16_t)((long)Angle_Set  *1023/ 300); 
    return waveshare_step;
}


//判断adc变化幅值和滤波强度的绝对值 参数：两个adc值或者滤波值
uint16_t Flex_GetAbs(uint16_t a , uint16_t b)
{
    return (a >= b)?(a-b):(b-a);
}


int16_t test_abs_i16(int16_t x)
{
    return (x >= 0) ? x : -x;
}

//三点中值
uint16_t Flex_GetMiddleNum(uint16_t a, uint16_t b, uint16_t c)
{
    if(a > b)
    {
        uint16_t t = a;
        a = b;
        b = t;
    }
    if(b > c)
    {
        uint16_t t = b;
        b = c;
        c = t;
    }
    if(a > b)
    {
        uint16_t t = a;
        a = b;
        b = t;
    }
    return b;
}



//位置滤波
uint16_t Flex_UpdatePosFilter(const  Fingertask_t * cfg , uint16_t prev_adc, uint16_t raw_adc)
{
    // uint16_t delta_adc = Flex_GetAbs(prev_adc,raw_adc);

    // if(delta_adc >= cfg -> pos_fast_delta_adc)
    // {
    //     return (uint16_t)(((uint32_t)prev_adc + (uint32_t)raw_adc*3U)/4U);
    // }

    // if(delta_adc >= 6U){
    //     return (uint16_t)(((uint32_t)prev_adc * 3U + raw_adc)/4U);
    // }
    // return (uint16_t)(((uint32_t)prev_adc*7U + raw_adc)/8U);

    uint16_t hold_delta_adc = 6U;
    uint16_t delta_adc = Flex_GetAbs(prev_adc, raw_adc);

    if(cfg != NULL)
    {
        if((cfg->servo_id == 1U) ||
           (cfg->servo_id == 2U) ||
           (cfg->servo_id == 3U) ||
           (cfg->servo_id == 4U))
        {
            hold_delta_adc = 10U;
        }
    }

    if(delta_adc <= hold_delta_adc)
    {
        return prev_adc;
    }

    if(delta_adc >= cfg->pos_fast_delta_adc)
    {
        return (uint16_t)(((uint32_t)prev_adc + (uint32_t)raw_adc * 3U) / 4U);
    }

    if(delta_adc >= 8U)
    {
        return (uint16_t)(((uint32_t)prev_adc * 3U + raw_adc) / 4U);
    }

    return (uint16_t)(((uint32_t)prev_adc * 7U + raw_adc) / 8U);

}

//速度滤波
uint16_t Flex_Update_vel_filter(uint16_t prev_adc,uint16_t raw_adc)
{
    uint16_t delta_adc = Flex_GetAbs(prev_adc,raw_adc);
    if(delta_adc <= 2U)
    {
        return prev_adc;
    }

    if(delta_adc <= 6U)
    {
        return (uint16_t)(((uint32_t)prev_adc * 3U + raw_adc)/4U);
    }

    return (uint16_t)(((uint32_t)prev_adc + raw_adc) / 2U);
}

uint16_t Speed_delta(const Fingertask_t * cfg,uint16_t delta_adc)
{
    if(delta_adc <= cfg ->speed_delta_1)
    {
        return cfg->speed_val_1;
    }
       if(delta_adc <= cfg ->speed_delta_2)
    {
        return cfg->speed_val_2;
    }
       if(delta_adc <= cfg ->speed_delta_3)
    {
        return cfg->speed_val_3;
    }
       if(delta_adc <= cfg ->speed_delta_4)
    {
        return cfg->speed_val_4;
    }
    return cfg->speed_val_5;
}




int16_t Flex_map_to_angle(const Fingertask_t * cfg, uint16_t adc)
{
    int16_t open_angle_deg = 49;
    int16_t close_angle_deg = 190;
    uint16_t open_snap_adc = 8;
    uint16_t close_snap_adc = 6;
    int32_t adc_value;
    int32_t open_ref;
    int32_t close_ref;
    int32_t angle;

      if(cfg == NULL) {
        return open_angle_deg;
    }

    if(cfg->calib_ok == 0U) {
        return open_angle_deg;
    }

    if(cfg->adc_open_ref == cfg->adc_close_ref) {
        return open_angle_deg;
    }

    if(cfg->servo_id == 2)
    {
        open_angle_deg = 44;
    }

    if(cfg->servo_id == 1)
    {
        open_snap_adc = 10;
        close_snap_adc = 8;
    }
    else if(cfg->servo_id == 3)
    {
        open_snap_adc = 100;
        close_snap_adc = 25;
    }

    adc_value = (int32_t)adc;
    open_ref = (int32_t)cfg->adc_open_ref;
    close_ref = (int32_t)cfg->adc_close_ref;

    if(open_ref > close_ref) {
        if(adc_value >= (open_ref - (int32_t)open_snap_adc)) {
            return open_angle_deg;
        }

        if(adc_value <= (close_ref + (int32_t)close_snap_adc)) {
            return close_angle_deg;
        }

        if(adc_value > open_ref) {
            adc_value = open_ref;
        }

        if(adc_value < close_ref) {
            adc_value = close_ref;
        }

        angle = open_angle_deg +
                ((open_ref - adc_value) *
                 (close_angle_deg - open_angle_deg)) /
                (open_ref - close_ref);
    }
    else {
        if(adc_value <= (open_ref + (int32_t)open_snap_adc)) {
            return open_angle_deg;
        }

        if(adc_value >= (close_ref - (int32_t)close_snap_adc)) {
            return close_angle_deg;
        }

        if(adc_value < open_ref) {
            adc_value = open_ref;
        }

        if(adc_value > close_ref) {
            adc_value = close_ref;
        }

        angle = open_angle_deg +
                ((adc_value - open_ref) *
                 (close_angle_deg - open_angle_deg)) /
                (close_ref - open_ref);
    }

    return (int16_t)angle;

}
