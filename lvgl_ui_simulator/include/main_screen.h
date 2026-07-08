#ifndef MY_SCREEN_H
#define MY_SCREEN_H

//Header
#include "lvgl.h"


typedef struct {
    //创建主控
    lv_obj_t * Main_control_scr; 
    //当前角度文本
    lv_obj_t * Angle_now_Text;
    //舵机目标角度文本
    lv_obj_t * Angle_Set_Text;
    //舵机当前速度文本
    lv_obj_t * Speed_Text;
    //舵机当前电压
    lv_obj_t * voltage_text;
    //舵机当前温度
    lv_obj_t * temp_text;
    //舵机模式ID
    lv_obj_t * mode_text;
    lv_obj_t* id_text;

}Main_scr_t;


/*                            GlobalVar                    */
extern Main_scr_t Main_scr;

void Main_control_scr_init(void);


#endif