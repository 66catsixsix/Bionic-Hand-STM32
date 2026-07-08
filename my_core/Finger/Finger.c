#include "stdio.h"
#include "Finger.h"
#include "stdint.h"
#include "SCServo.h"

Fingertask_t Hand[Finger_NUM_ALL];


void Hand_Init(void)
{
      for(int i = 0; i < Finger_NUM_ALL; i++) {
        Hand[i].target_angle = 2048;
        Hand[i].adc_scan = 255;
    //   Hand[i].calib_min = 0;
    //     Hand[i].calib_max = 4095;
        Hand[i].adc_open_ref = 0;
        Hand[i].adc_close_ref = 0;
        Hand[i].calib_ok = 0;

        // Hand[Finger_Index].adc_scan = 0;
        // Hand[Finger_Index_L].adc_scan = 255;

        // Hand[Finger_Middle].adc_scan = 255;
        // Hand[Finger_Middle_L].adc_scan = 255;

        // Hand[Finger_Ring].adc_scan = 255;
        // Hand[Finger_Ring_L].adc_scan = 255;

        // Hand[Finger_Little].adc_scan = 255;
        // Hand[Finger_Little_L].adc_scan = 255;

        // Hand[Finger_Thumb].adc_scan = 255;
        // Hand[Finger_Thumb_L].adc_scan = 255;

        Hand[Finger_Index].adc_scan = 0;
        Hand[Finger_Index_L].adc_scan = 1;

        Hand[Finger_Middle].adc_scan = 2;
        Hand[Finger_Middle_L].adc_scan = 3;

        Hand[Finger_Ring].adc_scan = 4;
        Hand[Finger_Ring_L].adc_scan = 5;

        Hand[Finger_Little].adc_scan = 6;
        Hand[Finger_Little_L].adc_scan = 7;

        Hand[Finger_Thumb].adc_scan = 8;
        Hand[Finger_Thumb_L].adc_scan = 9;

        /* PA0 quick-tune: measured after replacing extension wires. */
        Hand[Finger_Index].adc_open_ref = 2005;
        Hand[Finger_Index].adc_close_ref = 1850;
        Hand[Finger_Index].calib_ok = 1;
        Hand[Finger_Index].pos_fast_delta_adc = 16;
        Hand[Finger_Index].speed_delta_1 = 1;
        Hand[Finger_Index].speed_delta_2 = 3;
        Hand[Finger_Index].speed_delta_3 = 6;
        Hand[Finger_Index].speed_delta_4 = 10;
        Hand[Finger_Index].speed_val_1 = 180;
        Hand[Finger_Index].speed_val_2 = 320;
        Hand[Finger_Index].speed_val_3 = 700;
        Hand[Finger_Index].speed_val_4 = 1200;
        Hand[Finger_Index].speed_val_5 = 1600;

        
        Hand[Finger_Index_L].adc_open_ref = 1765;
        Hand[Finger_Index_L].adc_close_ref = 813;
        Hand[Finger_Index_L].calib_ok = 1;
        Hand[Finger_Index_L].pos_fast_delta_adc = 16;
        Hand[Finger_Index_L].speed_delta_1 = 2;
        Hand[Finger_Index_L].speed_delta_2 = 5;
        Hand[Finger_Index_L].speed_delta_3 = 10;
        Hand[Finger_Index_L].speed_delta_4 = 14;
        Hand[Finger_Index_L].speed_val_1 = 180;
        Hand[Finger_Index_L].speed_val_2 = 320;
        Hand[Finger_Index_L].speed_val_3 = 550;
        Hand[Finger_Index_L].speed_val_4 = 1200;
        Hand[Finger_Index_L].speed_val_5 = 1600;


        Hand[Finger_Middle].adc_open_ref = 2093;
        Hand[Finger_Middle].adc_close_ref = 1910;
        Hand[Finger_Middle].calib_ok = 1;
        Hand[Finger_Middle].pos_fast_delta_adc = 16;
        Hand[Finger_Middle].speed_delta_1 = 2;
        Hand[Finger_Middle].speed_delta_2 = 5;
        Hand[Finger_Middle].speed_delta_3 = 10;
        Hand[Finger_Middle].speed_delta_4 = 14;
        Hand[Finger_Middle].speed_val_1 = 180;
        Hand[Finger_Middle].speed_val_2 = 320;
        Hand[Finger_Middle].speed_val_3 = 550;
        Hand[Finger_Middle].speed_val_4 = 1200;
        Hand[Finger_Middle].speed_val_5 = 1600;

        Hand[Finger_Middle_L].adc_open_ref = 1855;
        Hand[Finger_Middle_L].adc_close_ref = 789;
        Hand[Finger_Middle_L].calib_ok = 1;
        Hand[Finger_Middle_L].pos_fast_delta_adc = 16;
        Hand[Finger_Middle_L].speed_delta_1 = 2;
        Hand[Finger_Middle_L].speed_delta_2 = 5;
        Hand[Finger_Middle_L].speed_delta_3 = 10;
        Hand[Finger_Middle_L].speed_delta_4 = 14;
        Hand[Finger_Middle_L].speed_val_1 = 180;
        Hand[Finger_Middle_L].speed_val_2 = 320;
        Hand[Finger_Middle_L].speed_val_3 = 550;
        Hand[Finger_Middle_L].speed_val_4 = 1200;
        Hand[Finger_Middle_L].speed_val_5 = 1600;


        Hand[Finger_Ring].adc_open_ref = 2340;
        Hand[Finger_Ring].adc_close_ref = 2165;
        Hand[Finger_Ring].calib_ok = 1;
        Hand[Finger_Ring].pos_fast_delta_adc = 16;
        Hand[Finger_Ring].speed_delta_1 = 4;
        Hand[Finger_Ring].speed_delta_2 = 8;
        Hand[Finger_Ring].speed_delta_3 = 10;
        Hand[Finger_Ring].speed_delta_4 = 14;
        Hand[Finger_Ring].speed_val_1 = 260;
        Hand[Finger_Ring].speed_val_2 = 420;
        Hand[Finger_Ring].speed_val_3 = 550;
        Hand[Finger_Ring].speed_val_4 = 1200;
        Hand[Finger_Ring].speed_val_5 = 1600;

        Hand[Finger_Ring_L].adc_open_ref = 1734;
        Hand[Finger_Ring_L].adc_close_ref = 931;
        Hand[Finger_Ring_L].calib_ok = 1;
        Hand[Finger_Ring_L].pos_fast_delta_adc = 16;
        Hand[Finger_Ring_L].speed_delta_1 = 2;
        Hand[Finger_Ring_L].speed_delta_2 = 5;
        Hand[Finger_Ring_L].speed_delta_3 = 10;
        Hand[Finger_Ring_L].speed_delta_4 = 14;
        Hand[Finger_Ring_L].speed_val_1 = 180;
        Hand[Finger_Ring_L].speed_val_2 = 320;
        Hand[Finger_Ring_L].speed_val_3 = 550;
        Hand[Finger_Ring_L].speed_val_4 = 1200;
        Hand[Finger_Ring_L].speed_val_5 = 1600;


        Hand[Finger_Little].adc_open_ref = 2295;
        Hand[Finger_Little].adc_close_ref = 1320;
        Hand[Finger_Little].calib_ok = 1;
        Hand[Finger_Little].pos_fast_delta_adc = 16;
        Hand[Finger_Little].speed_delta_1 = 2;
        Hand[Finger_Little].speed_delta_2 = 5;
        Hand[Finger_Little].speed_delta_3 = 10;
        Hand[Finger_Little].speed_delta_4 = 14;
        Hand[Finger_Little].speed_val_1 = 180;
        Hand[Finger_Little].speed_val_2 = 320;
        Hand[Finger_Little].speed_val_3 = 550;
        Hand[Finger_Little].speed_val_4 = 1200;
        Hand[Finger_Little].speed_val_5 = 1600;

        Hand[Finger_Little_L].adc_open_ref = 1925;
        Hand[Finger_Little_L].adc_close_ref = 880;
        Hand[Finger_Little_L].calib_ok = 1;
        Hand[Finger_Little_L].pos_fast_delta_adc = 16;
        Hand[Finger_Little_L].speed_delta_1 = 2;
        Hand[Finger_Little_L].speed_delta_2 = 5;
        Hand[Finger_Little_L].speed_delta_3 = 10;
        Hand[Finger_Little_L].speed_delta_4 = 14;
        Hand[Finger_Little_L].speed_val_1 = 180;
        Hand[Finger_Little_L].speed_val_2 = 320;
        Hand[Finger_Little_L].speed_val_3 = 550;
        Hand[Finger_Little_L].speed_val_4 = 1200;
        Hand[Finger_Little_L].speed_val_5 = 1600;


        Hand[Finger_Thumb].adc_open_ref = 1875;
        Hand[Finger_Thumb].adc_close_ref = 1280;
        Hand[Finger_Thumb].calib_ok = 1;
        Hand[Finger_Thumb].pos_fast_delta_adc = 16;
        Hand[Finger_Thumb].speed_delta_1 = 2;
        Hand[Finger_Thumb].speed_delta_2 = 5;
        Hand[Finger_Thumb].speed_delta_3 = 10;
        Hand[Finger_Thumb].speed_delta_4 = 14;
        Hand[Finger_Thumb].speed_val_1 = 180;
        Hand[Finger_Thumb].speed_val_2 = 320;
        Hand[Finger_Thumb].speed_val_3 = 550;
        Hand[Finger_Thumb].speed_val_4 = 1200;
        Hand[Finger_Thumb].speed_val_5 = 1600;

        Hand[Finger_Thumb_L].adc_open_ref = 1675;
        Hand[Finger_Thumb_L].adc_close_ref = 665;
        Hand[Finger_Thumb_L].calib_ok = 1;
        Hand[Finger_Thumb_L].pos_fast_delta_adc = 16;
        Hand[Finger_Thumb_L].speed_delta_1 = 2;
        Hand[Finger_Thumb_L].speed_delta_2 = 5;
        Hand[Finger_Thumb_L].speed_delta_3 = 10;
        Hand[Finger_Thumb_L].speed_delta_4 = 14;
        Hand[Finger_Thumb_L].speed_val_1 = 180;
        Hand[Finger_Thumb_L].speed_val_2 = 320;
        Hand[Finger_Thumb_L].speed_val_3 = 550;
        Hand[Finger_Thumb_L].speed_val_4 = 1200;
        Hand[Finger_Thumb_L].speed_val_5 = 1600;

    }

    //舵机id
    //root
    Hand[Finger_Index].servo_id = 1;
    Hand[Finger_Middle].servo_id = 2;
    Hand[Finger_Ring].servo_id = 3;
    Hand[Finger_Little].servo_id = 4;
    Hand[Finger_Thumb].servo_id = 5;

    //Small joint
    Hand[Finger_Index_L].servo_id = 6;
    Hand[Finger_Middle_L].servo_id = 7;
    Hand[Finger_Ring_L].servo_id = 8;
    Hand[Finger_Little_L].servo_id = 9;
    Hand[Finger_Thumb_L].servo_id = 10;


}

void Hand_Update_Motor(void)
{
        int i =0;
        uint8_t servo_ids[Finger_NUM_ALL];
        uint16_t target_angle[Finger_NUM_ALL];
        uint16_t speeds[Finger_NUM_ALL];

        for(;i<Finger_NUM_ALL;)  //*
        {
            servo_ids[i] = Hand[i].servo_id;
            target_angle[i] = Hand[i].target_angle;
            speeds[i] = 700;
            i++;
        }
        SyncWritePos(servo_ids,Finger_NUM_ALL,target_angle,0,speeds);
}
