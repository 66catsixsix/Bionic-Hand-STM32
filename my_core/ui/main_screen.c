#include "stdio.h"
#include "lvgl.h"
#include "main_screen.h"
#include "feetech_motor.h"

//GlobalVar
extern volatile int16_t servo_pos_now;

//LocalVar
//主屏幕结构体
Main_scr_t Main_scr;

/*                          code                         */

//callback
//data_refresh
static void servo_data_update(lv_timer_t* timer)
{
    if(servo_pos_now != -1)
    {
        lv_label_set_text_fmt(Main_scr.Angle_now_Text,"NowAngle:%d",servo_pos_now);
    }
}
//主屏幕
void Main_control_scr_init(){

    //加载主屏幕
    Main_scr.Main_control_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(Main_scr.Main_control_scr,lv_color_hex(0x000000),0);
    /*BUG  创建文本  */
    //当前角度
    Main_scr.Angle_now_Text = lv_label_create(Main_scr.Main_control_scr);
    //当前速度
    Main_scr.Speed_Text = lv_label_create(Main_scr.Main_control_scr);
    lv_label_set_text(Main_scr.Angle_now_Text,"NowAngle:...");
    lv_label_set_text(Main_scr.Speed_Text,"NowSpeed:");
    lv_obj_set_style_text_color(Main_scr.Angle_now_Text,lv_color_hex(0xffffff),0);
    lv_obj_set_style_text_color(Main_scr.Speed_Text,lv_color_hex(0xffffff),0);
    //设置文字大小和颜色
    lv_obj_set_style_text_font(Main_scr.Angle_now_Text, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_font(Main_scr.Speed_Text, &lv_font_montserrat_26, 0);
    //设置文字位置
    lv_obj_set_pos(Main_scr.Angle_now_Text,30,50);
    lv_obj_align_to(Main_scr.Speed_Text,Main_scr.Angle_now_Text,LV_ALIGN_OUT_BOTTOM_MID,0,50);   
    lv_timer_create(servo_data_update,200,NULL);
    lv_scr_load(Main_scr.Main_control_scr);
    //NOTE 飞特舵机通讯协议
 
    

    
}

/*                           code                        */




/*电机参数返回接口,串口调速...
motor_Function_interFace{



}
*/
