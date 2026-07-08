#ifndef   MY_ALGL_H
#define  MY_ALGL_H

#include <stdint.h>
#include "Finger.h"
//defined
#define  STEP_NOW_RES          0x38
#define  SPEED_NOW_RES       0x3A


//Var
// int16_t  Angle_Now_Update();
 int16_t  Step_Angle_Update(int16_t Angle_Set);
// int16_t MAP_Flex_To_Update(uint16_t current_adc);
uint16_t Flex_GetMiddleNum(uint16_t a, uint16_t b, uint16_t c);
uint16_t Flex_GetAbs(uint16_t a , uint16_t b);
uint16_t Flex_UpdatePosFilter(const  Fingertask_t * cfg , uint16_t prev_adc, uint16_t raw_adc);
uint16_t Speed_delta(const Fingertask_t * cfg,uint16_t delta_adc);
int16_t Flex_map_to_angle(const Fingertask_t * cfg, uint16_t adc);
int16_t test_abs_i16(int16_t x);
uint16_t Flex_Update_vel_filter(uint16_t prev_adc, uint16_t raw_adc);




#endif
