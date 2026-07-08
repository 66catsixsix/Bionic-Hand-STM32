#ifndef  FINGER_MANAGER
#define  FINGER_MANAGER

#include "stdint.h"

typedef enum
{
    Finger_Thumb = 0,
    Finger_Index,
    Finger_Middle,
    Finger_Ring,
    Finger_Little,

    Finger_Index_L,     //小关节配合flex 2.2  L:小关节
    Finger_Middle_L,
    Finger_Ring_L,
    Finger_Little_L,
    Finger_Thumb_L,
    Finger_NUM_ALL
}FInger_ID_t;

typedef struct Finger_Flex
{
    uint8_t servo_id;
    uint16_t adc_scan;
    uint16_t flex_raw_val;
    int16_t target_angle;
    uint16_t calib_min;
    uint16_t calib_max;
    int16_t lvgl_servo_current;
    uint16_t adc_open_ref;
    uint16_t adc_close_ref;
    uint16_t calib_ok;
    uint16_t pos_fast_delta_adc;
    uint16_t speed_delta_1;
    uint16_t speed_delta_2;
    uint16_t speed_delta_3;
    uint16_t speed_delta_4;
    uint16_t speed_val_1;
    uint16_t speed_val_2;
    uint16_t speed_val_3;
    uint16_t speed_val_4;
    uint16_t speed_val_5;

} Fingertask_t;

void Hand_Init(void);
void Hand_Update_Motor(void);

#endif