#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "SCServo.h"
#include "sdram.h"
#include "lcd_rgb.h"
#include "touch_800x480.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_conf.h"
#include "lv_demo_benchmark.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_motor.h"
#include "feetech_motor.h"
#include "stdlib.h"
#include "FreeRtos_task.h"
#include "flex.h"
#include "Finger.h"
#include "algo.h"
#include "stdio.h"
#include "EMG.h"
#if ENABLE_DHT11_TASK
#include "dht11.h"
#endif

#define LVGL_ONLY_TEST_MODE 0
#define TOUCH_UART_DEBUG 0
/* Set to 0 to disable runtime debug prints from Finger_All_Control_TASK. */
#define FINGER_ALL_CTRL_DEBUG 0
#define FINGER_ALL_CTRL_DEBUG_PERIOD_MS 500U
/* Lightweight two-joint debug while the full hand control task is running. */
#define FINGER_PAIR_DEBUG 0
#define FINGER_PAIR_DEBUG_PERIOD_MS 100U

uint16_t flex_read_adc(void); 
uint16_t get_flex_channel_adc(uint8_t ch_index);
void flex_task(void* arg);
void flex_calibration(void);
int16_t MAP_Flex_To_Update(uint16_t current_adc);

//GlobalVar
int flex_flag = 0;
extern volatile int16_t servo_pos_now;
extern volatile int16_t angle_now;
extern int16_t  Step_Angle_Update(int16_t Angle_Set);
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern uint16_t calib_max;
extern uint16_t calib_min;
extern uint8_t rx_buf[];
extern uint8_t rx_index ;
extern uint8_t tx_buf[];
extern  volatile int16_t angle_flag;  
extern  volatile int16_t  angle_value;
extern servo_mode_t servo_mode;
extern uint8_t touch_ok;
extern Fingertask_t Hand[];
//lvgl
extern volatile uint16_t g_auto_force;
extern volatile uint16_t g_auto_cycles;
extern int16_t Angle_Now_Update();

static volatile hand_app_mode_t g_hand_app_mode = HAND_APP_MODE_IDLE;
static emg_metrics_t g_emg_metrics;
volatile uint8_t g_dht_temp = 0U;
volatile uint8_t g_dht_hum = 0U;
volatile uint8_t g_dht_ok = 0U;
volatile uint8_t g_dht_fail = 0U;

void HandApp_SetMode(hand_app_mode_t mode)
{
    if(mode > HAND_APP_MODE_CALIB)
    {
        mode = HAND_APP_MODE_IDLE;
    }

    g_hand_app_mode = mode;
}

hand_app_mode_t HandApp_GetMode(void)
{
    return g_hand_app_mode;
}

uint8_t HandApp_IsMode(hand_app_mode_t mode)
{
    return (g_hand_app_mode == mode) ? 1U : 0U;
}

#if LVGL_ONLY_TEST_MODE
static lv_obj_t * s_test_tick_label;
static lv_obj_t * s_test_touch_label;
static lv_obj_t * s_test_raw_label;
static lv_obj_t * s_test_status_label;
static lv_obj_t * s_test_touch_dot;
static uint32_t s_test_tick_count;
static volatile uint32_t s_touch_task_loop_count;
static volatile uint32_t s_touch_scan_count;
static volatile uint32_t s_touch_hit_count;
static volatile uint16_t s_touch_last_x;
static volatile uint16_t s_touch_last_y;
static volatile uint8_t s_touch_last_flag;
static volatile TickType_t s_touch_last_tick;

static void lvgl_test_tick_cb(lv_timer_t * timer)
{
    TickType_t now_tick;
    TickType_t last_touch_tick;

    LV_UNUSED(timer);
    s_test_tick_count++;
    lv_label_set_text_fmt(s_test_tick_label, "LVGL tick: %lu", (unsigned long)s_test_tick_count);
    lv_label_set_text_fmt(s_test_status_label,
                          "touch_ok=%u  loops=%lu",
                          (unsigned int)touch_ok,
                          (unsigned long)s_touch_task_loop_count);
    lv_label_set_text_fmt(s_test_raw_label,
                          "raw flag=%u  scans=%lu  hits=%lu",
                          (unsigned int)s_touch_last_flag,
                          (unsigned long)s_touch_scan_count,
                          (unsigned long)s_touch_hit_count);

    now_tick = xTaskGetTickCount();
    last_touch_tick = s_touch_last_tick;
    if((now_tick - last_touch_tick) <= pdMS_TO_TICKS(500)) {
        lv_label_set_text_fmt(s_test_touch_label, "Touch: X=%u Y=%u",
                              (unsigned int)s_touch_last_x,
                              (unsigned int)s_touch_last_y);
        lv_obj_clear_flag(s_test_touch_dot, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(s_test_touch_dot,
                       (lv_coord_t)s_touch_last_x - 12,
                       (lv_coord_t)s_touch_last_y - 12);
    }
    else {
        lv_label_set_text(s_test_touch_label, "Touch: waiting");
        lv_obj_add_flag(s_test_touch_dot, LV_OBJ_FLAG_HIDDEN);
    }
}

static void lvgl_test_btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_label_set_text(s_test_touch_label, "Button clicked");
    }
}

static void lvgl_test_ui_init(void)
{
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x101820), 0);

    s_test_tick_label = lv_label_create(scr);
    lv_obj_set_style_text_color(s_test_tick_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(s_test_tick_label, &lv_font_montserrat_26, 0);
    lv_label_set_text(s_test_tick_label, "LVGL tick: 0");
    lv_obj_align(s_test_tick_label, LV_ALIGN_TOP_MID, 0, 40);

    s_test_touch_label = lv_label_create(scr);
    lv_obj_set_style_text_color(s_test_touch_label, lv_color_hex(0x7DFFB3), 0);
    lv_obj_set_style_text_font(s_test_touch_label, &lv_font_montserrat_22, 0);
    lv_label_set_text(s_test_touch_label, "Touch: waiting");
    lv_obj_align(s_test_touch_label, LV_ALIGN_TOP_MID, 0, 100);

    s_test_raw_label = lv_label_create(scr);
    lv_obj_set_style_text_color(s_test_raw_label, lv_color_hex(0x89B4FA), 0);
    lv_label_set_text(s_test_raw_label, "raw flag=0  scans=0  hits=0");
    lv_obj_align(s_test_raw_label, LV_ALIGN_TOP_MID, 0, 145);

    s_test_status_label = lv_label_create(scr);
    lv_obj_set_style_text_color(s_test_status_label, lv_color_hex(0xFFB86C), 0);
    lv_label_set_text(s_test_status_label, "touch_ok=0  loops=0");
    lv_obj_align(s_test_status_label, LV_ALIGN_TOP_MID, 0, 175);

    lv_obj_t * btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 220, 80);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(btn, lvgl_test_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Touch Me");
    lv_obj_center(btn_label);

    lv_obj_t * hint = lv_label_create(scr);
    lv_obj_set_style_text_color(hint, lv_color_hex(0xF7D774), 0);
    lv_label_set_text(hint, "Only LVGL display/touch test is running");
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, -40);

    s_test_touch_dot = lv_obj_create(scr);
    lv_obj_remove_style_all(s_test_touch_dot);
    lv_obj_set_size(s_test_touch_dot, 24, 24);
    lv_obj_set_style_radius(s_test_touch_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(s_test_touch_dot, LV_OPA_80, 0);
    lv_obj_set_style_bg_color(s_test_touch_dot, lv_color_hex(0xFF6B6B), 0);
    lv_obj_add_flag(s_test_touch_dot, LV_OBJ_FLAG_HIDDEN);

    lv_scr_load(scr);
    lv_timer_create(lvgl_test_tick_cb, 30, NULL);
}
#endif



void task_lvgl(void *arg)
{
    uint32_t delay_ms;

    (void)arg;
#if LVGL_ONLY_TEST_MODE
    lvgl_test_ui_init();
#else
    lvgl_motor();
#endif
    for (;;) {
#if LVGL_ONLY_TEST_MODE
        s_touch_task_loop_count++;
        if (touch_ok) {
            Touch_Scan();
            s_touch_scan_count++;
            s_touch_last_flag = touchInfo.flag;
            if (touchInfo.flag == 1) {
                s_touch_hit_count++;
                s_touch_last_x = touchInfo.x[0];
                s_touch_last_y = touchInfo.y[0];
                s_touch_last_tick = xTaskGetTickCount();
            }
        }
#endif
        delay_ms = lv_task_handler(); 
        if(delay_ms < 1U) {
            delay_ms = 1U;
        }
        else if(delay_ms > 20U) {
            delay_ms = 20U;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void task_touch(void *arg)
{
    (void)arg;
    char log_buf[64];
#if !LVGL_ONLY_TEST_MODE && TOUCH_UART_DEBUG
    sprintf(log_buf, "Touch Init Result: %d\n", touch_ok);
    HAL_UART_Transmit(&huart3, (uint8_t*)log_buf, strlen(log_buf), 100);
#endif

    for (;;) {
        if (touch_ok) {
            Touch_Scan();
#if !LVGL_ONLY_TEST_MODE && TOUCH_UART_DEBUG
            if (touchInfo.flag == 1) {
                sprintf(log_buf, "Touch X:%d Y:%d\n", touchInfo.x[0], touchInfo.y[0]);
                HAL_UART_Transmit(&huart3, (uint8_t*)log_buf, strlen(log_buf), 100);
            }
#endif
        }
        vTaskDelay(pdMS_TO_TICKS(20)); 
    }
}

 void task_servo(void *arg)
{
    (void)arg;

    vTaskDelay(pdMS_TO_TICKS(2000));

    for (;;) {
            //  WritePos(8, Step_Angle_Update(120), 0, 700);
            //     vTaskDelay(pdMS_TO_TICKS(300));
            //     WritePos(8, Step_Angle_Update(160), 0, 700);
            if(angle_flag)
            {
                char test_buf[32];
                sprintf(test_buf, "实际接收角度:%d\n", angle_value);
                WritePos(0xFE,  Step_Angle_Update(angle_value), 0, 700); //广播发送步长统一角度安装
                HAL_UART_Transmit_IT(&huart3, (uint8_t*)test_buf, strlen(test_buf));
                angle_flag = 0; 
            }
            vTaskDelay(pdMS_TO_TICKS(100));  //重要延迟！！！
    }

}

void  task_usart3(void * arg)
{
    uint8_t motor_tx_buf[8];
    uint8_t  ch;
    int16_t angle = 0;
    for(;;)
    {
         if((HAL_UART_Receive_IT(&huart3,&ch,1) == HAL_OK))
         {
                if(ch == '\n' || ch == '\r')
                {
                    int len = sprintf((char*)motor_tx_buf, "%d", angle);
                    HAL_UART_Transmit(&huart3, motor_tx_buf, len, 100);
                }
         }
         vTaskDelay(pdMS_TO_TICKS(100));
    }
        vTaskDelay(pdMS_TO_TICKS(10));  //重要延迟！！！
}

void id_change_task(void * arg)
{       //新舵机校准,输入需要更改的id
    change_motor_id(11);
    vTaskDelete(NULL);
}

/*===========================================================flex============================================================*/
// //单根控制功能
// void flex_task(void* arg)
// {
//     (void)arg;
//     char flex_buf[128];
//     vTaskDelay(pdMS_TO_TICKS(22000));//校准延时
//     for (;;) {
//         int16_t current_servo_angle = Angle_Now_Update();
//         int16_t target_angle = MAP_Flex_To_Update(adc_value);
//         g_auto_cycles = current_servo_angle; //
//         sprintf(pdMS_TO_TICKS(22000));
//     while(1){
//             for(int i =0; i<Finger_NUM_ALL; i++)
//         {   
//             uint16_flex_buf, "ADC: %d, Target Angle: %d\n", adc_value, target_angle);
//         HAL_UART_Transmit(&huart3, (uint8_t*)flex_buf, strlen(flex_buf),100);
//         WritePos(1,  Step_Angle_Update(target_angle), 0, 700); 
//         vTaskDelay(pdMS_TO_TICKS(50)); 
// }
// }

//
// void Finger_Map_Update(void*arg )
// {
//     (void)arg;
//     Hand_Init();
//     vTaskDelay(pdMS_TO_TICKS(22000));
//     while(1){
//             for(int i =0; i<Finger_NUM_ALL; i++)
//         {   
//             uint16_t adc_value = get_flex_channel_adc(Hand[i].adc_channel);
//             if(i == Finger_Index)
//             {
//                 adc_value = flex_read_adc();
//             }
//             else
//             {
//                 adc_value = 2048;
//             }
//         }

//             Hand_Update_Motor();
//     }
// }

//旧版单根手指轮询标定程序
// void flex_calibration_task(void * arg) 
// {
//     (void)arg;
//     uint32_t start_time = HAL_GetTick();
//     char calib_buf[128];
//     for(int i = 0;i<Finger_NUM_ALL;i++)
//     {
//         if(Hand[i].adc_channel !=255)
//         {
//             Hand[i].calib_max = 0;
//             Hand[i].calib_min = 4095;
//         }
//     }
//     HAL_UART_Transmit_IT(&huart3,(uint8_t*)"Calibration Start!!\n",23);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     HAL_UART_WritePos(6, Step_Angle_Update(120), 0, 700);
// HAL_Delay(1000);
// WritePos(6, Step_Angle_Update(160), 0, 700);
// Transmit_IT(&huart3,(uint8_t*)"Please flex the sensor to the maximum and minimum positions  for 20 seconds...\n",92);
//     HAL_UART_Transmit_IT(&huart3,(uint8_t*)"Calibration wait...\n",23);
//     vTaskDelay(pdMS_TO_TICKS(1000));
    
//     while(HAL_GetTick() - start_time < 20000)
//     {
//         flex_calibration();
//         HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//         vTaskDelay(pdMS_TO_TICKS(50));
//     }
//     sprintf((char*)calib_buf,"Max: %d\n Min: %d\n",calib_max,calib_min);
//     HAL_UART_Transmit_IT(&huart3, (uint8_t*)calib_buf, strlen((char*)calib_buf));
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     HAL_UART_Transmit_IT(&huart3, (uint8_t*)"Calibration Complete\n", 21);

//     //calib finish
//     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     for(;;)
//     {
//         vTaskSuspend(NULL);
//     } 
// }



void Finger_Map_Update(void*arg )
{
    (void)arg;
    //Hand_Init();
    for(int i = 0;i<Finger_NUM_ALL;i++)
    {
        if(Hand[i].adc_scan == 255)Hand[i].target_angle = Step_Angle_Update(49);
    }
    vTaskDelay(pdMS_TO_TICKS(22000));
    //char flex_buf[128];
  char buf[128];

    while(1){
            for(int i =0; i<Finger_NUM_ALL; i++)
            {
                if(Hand[i].adc_scan != 255)
                {
                    /*DMA*/
                    uint16_t adc_value = get_flex_channel_adc(Hand[i].adc_scan);
                     long mapped_angle;
                    /*线性弯曲平滑电阻值*/
                    mapped_angle = 190 - (long)(adc_value - Hand[i].calib_min)*(190-49)/ (Hand[i].calib_max - Hand[i].calib_min);               
                    /*死区限位*/ 
                    if(mapped_angle < 49) mapped_angle = 49;
                    if(mapped_angle > 190) mapped_angle = 190;

                    sprintf(buf,
                            "0:%4d 1:%4d 2:%4d 3:%4d 4:%4d 5:%4d 6:%4d 7:%4d 8:%4d 9:%4d\r\n",
                            get_flex_channel_adc(0),
                            get_flex_channel_adc(1),
                            get_flex_channel_adc(2),
                            get_flex_channel_adc(3),
                            get_flex_channel_adc(4),
                            get_flex_channel_adc(5),
                            get_flex_channel_adc(6),
                            get_flex_channel_adc(7),
                            get_flex_channel_adc(8),
                            get_flex_channel_adc(9));

                    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 100);
                    

                    Hand[i].target_angle = Step_Angle_Update((int16_t)mapped_angle);
                }
            }
            Hand_Update_Motor();
            vTaskDelay(pdMS_TO_TICKS(20));
    }
}
 
void flex_calibration_task(void * arg)    //标定程序
{
    (void)arg;
    
    uint32_t start_time = HAL_GetTick();
    char calib_buf[128];
    for(int i = 0;i<Finger_NUM_ALL;i++)
    {
        if(Hand[i].adc_scan !=255)
        {
            Hand[i].calib_max = 0;
            Hand[i].calib_min = 4095;
        }
    }
    HAL_UART_Transmit_IT(&huart3,(uint8_t*)"Calibration Start!!\n",20);
    vTaskDelay(pdMS_TO_TICKS(1000));
    HAL_UART_Transmit_IT(&huart3,(uint8_t*)"Please Move Your FInger to Calibration get min and max\n",55);
    HAL_UART_Transmit_IT(&huart3,(uint8_t*)"Calibration wait...\n",20);
    vTaskDelay(pdMS_TO_TICKS(1000));

    while(HAL_GetTick() - start_time < 20000)
    {
            for(int i =0 ; i<Finger_NUM_ALL ; i++)
            {
                if(Hand[i].adc_scan != 255)  //限位
                {
                    uint16_t now_val = get_flex_channel_adc(Hand[i].adc_scan);
                    if(now_val > Hand[i].calib_max)Hand[i].calib_max = now_val;
                    if(now_val < Hand[i].calib_min)Hand[i].calib_min = now_val;
                }
            }
    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    vTaskDelay(pdMS_TO_TICKS(20));    
    }

    sprintf(calib_buf,"ID1:MAX:%d,MIN:%d | L:ID6:MAX:%d MIN:%d",
                  Hand[Finger_Index].calib_max,Hand[Finger_Index].calib_min,
                  Hand[Finger_Index_L].calib_max,Hand[Finger_Index_L].calib_min);

    HAL_UART_Transmit_IT(&huart3,(uint8_t*)calib_buf,strlen(calib_buf));
    vTaskDelay(pdMS_TO_TICKS(1000));
    HAL_UART_Transmit_IT(&huart3, (uint8_t*)"Calibration Complete\n", 21);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
    vTaskSuspend(NULL);

}



//舵机id整体姿势测试
void id_total_task(void * arg)
{
    (void)arg;

    uint8_t servo_ids[10] = {1,2,3,4,5,6,7,8,9,10};
    uint16_t target_steps[10];
    uint16_t speeds[10];
    int i;

    vTaskDelay(pdMS_TO_TICKS(2000));

   for(;;)
    {
        for(i = 0; i < 10; i++)
        {
            target_steps[i] = Step_Angle_Update(50);
            speeds[i] = 600;
        }  
        SyncWritePos(servo_ids,10,target_steps,0,speeds);
        vTaskDelay(pdMS_TO_TICKS(1000));      


        target_steps[0] = Step_Angle_Update(90);
        target_steps[1] = Step_Angle_Update(90);
        target_steps[2] = Step_Angle_Update(100);
        target_steps[3] = Step_Angle_Update(100);
        target_steps[4] = Step_Angle_Update(90);
        target_steps[5] = Step_Angle_Update(90);
        target_steps[6] = Step_Angle_Update(90);
        target_steps[7] = Step_Angle_Update(100);
        target_steps[8] = Step_Angle_Update(100);
        target_steps[9] = Step_Angle_Update(90);

         for(i = 0; i < 10; i++)
        {
            speeds[i] = 700;
        }
        SyncWritePos(servo_ids, 10, target_steps, 0, speeds);
        vTaskDelay(pdMS_TO_TICKS(1000));


        target_steps[0] = Step_Angle_Update(150);
        target_steps[1] = Step_Angle_Update(150);
        target_steps[2] = Step_Angle_Update(160);
        target_steps[3] = Step_Angle_Update(160);
        target_steps[4] = Step_Angle_Update(140);
        target_steps[5] = Step_Angle_Update(150);
        target_steps[6] = Step_Angle_Update(150);
        target_steps[7] = Step_Angle_Update(160);
        target_steps[8] = Step_Angle_Update(160);
        target_steps[9] = Step_Angle_Update(140);

        for(i = 0; i < 10; i++)
        {
            speeds[i] = 800;
        }
        SyncWritePos(servo_ids, 10, target_steps, 0, speeds);
        vTaskDelay(pdMS_TO_TICKS(1500));
        
    }
    vTaskDelay(pdMS_TO_TICKS(10));
}

void Finger_Index_Root_TASK(void * arg)
{
    (void)arg;

    Fingertask_t * cfg = &Hand[Finger_Index];

    uint16_t raw_adc;
    uint16_t raw_adc_1;
    uint16_t raw_adc_2;
    uint16_t raw_med_adc;

    uint16_t filt_pos_adc;
    uint16_t filt_vel_adc;
    uint16_t last_vel_adc;

    uint16_t delta_adc;
    uint16_t servo_speed;
    
    int16_t angle_deg;
    int16_t last_sent_angle = -1000;
    int16_t send_deadband_deg;
    
    const uint16_t loop_ms = 10U;
    const int16_t  send_deadband_base = 5;

    vTaskDelay(pdMS_TO_TICKS(1500));

    if(cfg->adc_scan == 255U)
    {
        vTaskSuspend(NULL);
    }

    raw_adc = get_flex_channel_adc(cfg->adc_scan);
    filt_pos_adc = raw_adc;
    filt_vel_adc = raw_adc;
    last_vel_adc = raw_adc;

    raw_adc_1 = raw_adc;
    raw_adc_2 = raw_adc;
    raw_med_adc = raw_adc;

    servo_speed = cfg->speed_val_1;

    for(;;)
    {
        if(cfg->calib_ok == 0U)
        {
            vTaskDelay(pdMS_TO_TICKS(loop_ms));
            continue;
        }

        raw_adc = get_flex_channel_adc(cfg->adc_scan);

        //三点中值
        raw_med_adc = Flex_GetMiddleNum(raw_adc,raw_adc_1,raw_adc_2);
        raw_adc_2 = raw_adc_1;
        raw_adc_1 = raw_adc;
      

        //位置滤波
        filt_pos_adc = Flex_UpdatePosFilter(cfg,filt_pos_adc,raw_med_adc);
        
        //速度滤波
        filt_vel_adc = Flex_Update_vel_filter(filt_vel_adc,raw_med_adc);

        //动作幅度
        delta_adc = Flex_GetAbs(filt_vel_adc,last_vel_adc);
        last_vel_adc = filt_vel_adc;

        //幅度调整速度
        servo_speed = Speed_delta(cfg,delta_adc);

        //位置映射角度
        angle_deg = Flex_map_to_angle(cfg,filt_pos_adc);

        //死区增益
        send_deadband_deg = send_deadband_base;
        if(delta_adc >= cfg -> speed_delta_4)
        {
            send_deadband_deg = 2;
        }
        else if(delta_adc >= cfg->speed_delta_2)
        {
            send_deadband_deg = 3;
        }

        //角度积分增加
        if((last_sent_angle < 0 || (Flex_GetAbs(angle_deg,last_sent_angle) >= (uint16_t)send_deadband_deg)))
        {
            WritePos(cfg->servo_id,(uint16_t)Step_Angle_Update(angle_deg),0,servo_speed);

            last_sent_angle = angle_deg;
        }
        vTaskDelay(pdMS_TO_TICKS(loop_ms));

    }

}

typedef struct
{
    uint16_t raw_adc_1;
    uint16_t raw_adc_2;
    uint16_t raw_adc;
    uint16_t raw_med_adc;
    uint16_t filt_pos_adc;
    uint16_t filt_vel_adc;
    uint16_t last_vel_adc;
    uint16_t delta_adc;
    uint16_t servo_speed;
    int16_t angle_deg;
    int16_t last_sent_angle;
    uint8_t initialized;
} FingerControlState_t;

static uint16_t angle_to_percent(int16_t angle_deg)
{
    if(angle_deg <= 49) return 0U;
    if(angle_deg >= 190) return 100U;
    return (uint16_t)(((uint32_t)(angle_deg - 49) * 100U) / 141U);
}

static uint8_t Finger_Control_IsEnabled(const Fingertask_t *cfg)
{
    if(cfg == NULL)
    {
        return 0U;
    }

    return (cfg->adc_scan != 255U) ? 1U : 0U;
}

static uint16_t Finger_Control_AngleAbs(int16_t a, int16_t b)
{
    int16_t diff = a - b;

    if(diff < 0)
    {
        diff = (int16_t)(-diff);
    }

    return (uint16_t)diff;
}

static uint8_t Finger_Control_NeedsDriftHold(const Fingertask_t *cfg)
{
    if(cfg == NULL)
    {
        return 0U;
    }

    return ((cfg->servo_id == 1U) ||
            (cfg->servo_id == 2U) ||
            (cfg->servo_id == 3U) ||
            (cfg->servo_id == 4U)) ? 1U : 0U;
}

static void Finger_Control_InitState(FingerControlState_t *state, const Fingertask_t *cfg)
{
    uint16_t raw_adc;

    if((state == NULL) || (Finger_Control_IsEnabled(cfg) == 0U))
    {
        return;
    }

    raw_adc = get_flex_channel_adc(cfg->adc_scan);
    state->raw_adc_1 = raw_adc;
    state->raw_adc_2 = raw_adc;
    state->raw_adc = raw_adc;
    state->raw_med_adc = raw_adc;
    state->filt_pos_adc = raw_adc;
    state->filt_vel_adc = raw_adc;
    state->last_vel_adc = raw_adc;
    state->delta_adc = 0U;
    state->servo_speed = 0U;
    state->angle_deg = 49;
    state->last_sent_angle = -1000;
    state->initialized = 1U;
}

static void Finger_Control_UpdateOne(Fingertask_t *cfg,
                                     FingerControlState_t *state,
                                     int16_t send_deadband_base,
                                     uint8_t allow_servo_write)
{
    uint16_t raw_adc;
    uint16_t raw_med_adc;
    uint16_t delta_adc;
    uint16_t servo_speed;
    uint16_t target_step;
    int16_t angle_deg;
    int16_t prev_angle_deg;
    int16_t send_deadband_deg;

    if((cfg == NULL) || (state == NULL))
    {
        return;
    }

    if(Finger_Control_IsEnabled(cfg) == 0U)
    {
        state->initialized = 0U;
        return;
    }

    if(state->initialized == 0U)
    {
        Finger_Control_InitState(state, cfg);
    }

    if(cfg->calib_ok == 0U)
    {
        return;
    }
    raw_adc = get_flex_channel_adc(cfg->adc_scan);
    raw_med_adc = Flex_GetMiddleNum(raw_adc, state->raw_adc_1, state->raw_adc_2);
    state->raw_adc_2 = state->raw_adc_1;
    state->raw_adc_1 = raw_adc;
    state->raw_adc = raw_adc;
    state->raw_med_adc = raw_med_adc;

    state->filt_pos_adc = Flex_UpdatePosFilter(cfg, state->filt_pos_adc, raw_med_adc);
    state->filt_vel_adc = Flex_Update_vel_filter(state->filt_vel_adc, raw_med_adc);

    delta_adc = Flex_GetAbs(state->filt_vel_adc, state->last_vel_adc);
    state->last_vel_adc = state->filt_vel_adc;
    state->delta_adc = delta_adc;

    servo_speed = Speed_delta(cfg, delta_adc);
    prev_angle_deg = state->angle_deg;
    angle_deg = Flex_map_to_angle(cfg, state->filt_pos_adc);

    if((prev_angle_deg >= 0) &&
       (Finger_Control_NeedsDriftHold(cfg) != 0U))
    {
        uint16_t angle_change = Finger_Control_AngleAbs(angle_deg, prev_angle_deg);
        uint16_t close_hold_adc = cfg->adc_close_ref + 70U;

        if((cfg->servo_id == 1U) &&
           (prev_angle_deg >= 170) &&
           (angle_deg < prev_angle_deg) &&
           ((state->filt_pos_adc <= close_hold_adc) || (angle_change <= 80U)))
        {
            angle_deg = prev_angle_deg;
        }
        else if((cfg->servo_id == 2U) &&
                (prev_angle_deg >= 170) &&
                (angle_deg < prev_angle_deg) &&
                (delta_adc <= cfg->speed_delta_1) &&
                (angle_change <= 8U))
        {
            angle_deg = prev_angle_deg;
        }
        else if((cfg->servo_id == 3U) &&
                (delta_adc <= cfg->speed_delta_1) &&
                (angle_change <= 8U))
        {
            angle_deg = prev_angle_deg;
        }
        else if((cfg->servo_id == 4U) &&
                (prev_angle_deg >= 170) &&
                (angle_deg < prev_angle_deg) &&
                (state->filt_pos_adc <= close_hold_adc))
        {
            angle_deg = prev_angle_deg;
        }
    }

    state->servo_speed = servo_speed;
    state->angle_deg = angle_deg;
    target_step = (uint16_t)Step_Angle_Update(angle_deg);
    cfg->target_angle = (int16_t)target_step;

    if(allow_servo_write == 0U)
    {
        state->last_sent_angle = -1000;
        return;
    }

    send_deadband_deg = send_deadband_base;
    if(cfg->servo_id == 3U)
    {
        send_deadband_deg = 12;
    }

    if(delta_adc >= cfg->speed_delta_4)
    {
        send_deadband_deg = (cfg->servo_id == 3U) ? 5 : 2;
    }
    else if(delta_adc >= cfg->speed_delta_2)
    {
        send_deadband_deg = (cfg->servo_id == 3U) ? 8 : 5;
    }

    if((state->last_sent_angle < 0) ||
       (Finger_Control_AngleAbs(angle_deg, state->last_sent_angle) >= (uint16_t)send_deadband_deg))
    {
        WritePos(cfg->servo_id, target_step, 0, servo_speed);
        state->last_sent_angle = angle_deg;
    }
}

#if FINGER_ALL_CTRL_DEBUG
static void Finger_Control_DebugDump(const FingerControlState_t *finger_state)
{
    char buf[192];
    int len;
    uint8_t i;

    for(i = Finger_Thumb; i < Finger_NUM_ALL; i++)
    {
        const Fingertask_t *cfg = &Hand[i];
        const FingerControlState_t *state = &finger_state[i];

        if(Finger_Control_IsEnabled(cfg) == 0U)
        {
            continue;
        }

        len = snprintf(buf, sizeof(buf),
                       "FDBG idx=%u id=%u ch=%u cal=%u raw=%u med=%u pos=%u ang=%d spd=%u d=%u\r\n",
                       (unsigned int)i,
                       (unsigned int)cfg->servo_id,
                       (unsigned int)cfg->adc_scan,
                       (unsigned int)cfg->calib_ok,
                       (unsigned int)state->raw_adc,
                       (unsigned int)state->raw_med_adc,
                       (unsigned int)state->filt_pos_adc,
                       (int)state->angle_deg,
                       (unsigned int)state->servo_speed,
                       (unsigned int)state->delta_adc);
        if(len > 0)
        {
            if(len > (int)sizeof(buf))
            {
                len = (int)sizeof(buf);
            }
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, (uint16_t)len, 100);
        }
    }
}
#endif

#if FINGER_PAIR_DEBUG
static void Finger_Pair_DebugDumpOne(const char *tag,
                                     const FingerControlState_t *finger_state,
                                     uint8_t root_index,
                                     uint8_t tip_index)
{
    char buf[160];
    int len;
    const Fingertask_t *root_cfg = &Hand[root_index];
    const Fingertask_t *tip_cfg = &Hand[tip_index];
    const FingerControlState_t *root = &finger_state[root_index];
    const FingerControlState_t *tip = &finger_state[tip_index];

    if((Finger_Control_IsEnabled(root_cfg) == 0U) &&
       (Finger_Control_IsEnabled(tip_cfg) == 0U))
    {
        return;
    }

    len = snprintf(buf, sizeof(buf),
                   "%s ch%u id%u r0=%u p0=%u a0=%d d0=%u | ch%u id%u r1=%u p1=%u a1=%d d1=%u\r\n",
                   tag,
                   (unsigned int)root_cfg->adc_scan,
                   (unsigned int)root_cfg->servo_id,
                   (unsigned int)root->raw_adc,
                   (unsigned int)root->filt_pos_adc,
                   (int)root->angle_deg,
                   (unsigned int)root->delta_adc,
                   (unsigned int)tip_cfg->adc_scan,
                   (unsigned int)tip_cfg->servo_id,
                   (unsigned int)tip->raw_adc,
                   (unsigned int)tip->filt_pos_adc,
                   (int)tip->angle_deg,
                   (unsigned int)tip->delta_adc);
    if(len > 0)
    {
        if(len >= (int)sizeof(buf))
        {
            len = (int)sizeof(buf) - 1;
        }
        HAL_UART_Transmit(&huart3, (uint8_t *)buf, (uint16_t)len, 100);
    }
}

static void Finger_Pair_DebugDump(const FingerControlState_t *finger_state)
{
    Finger_Pair_DebugDumpOne("RING", finger_state, Finger_Ring, Finger_Ring_L);
    Finger_Pair_DebugDumpOne("MID", finger_state, Finger_Middle, Finger_Middle_L);
}
#endif

void Finger_All_Control_TASK(void * arg)
{
    const uint16_t loop_ms = 15U;
    const int16_t send_deadband_base = 3;
    FingerControlState_t finger_state[Finger_NUM_ALL] = {0};
    uint8_t enabled_count = 0U;
    uint8_t i;
    hand_app_mode_t mode;
    uint8_t allow_sensor_update;
    uint8_t allow_servo_write;
#if FINGER_ALL_CTRL_DEBUG
    TickType_t last_debug_tick = 0U;
#endif
#if FINGER_PAIR_DEBUG
    TickType_t last_pair_debug_tick = 0U;
#endif

    (void)arg;

    vTaskDelay(pdMS_TO_TICKS(1500));

    for(i = Finger_Thumb; i < Finger_NUM_ALL ; i++)
    {
        if(Finger_Control_IsEnabled(&Hand[i]) != 0U)
        {
            Finger_Control_InitState(&finger_state[i], &Hand[i]);
            enabled_count++;
        }
    }

    if(enabled_count == 0U)
    {
        vTaskSuspend(NULL);
    }

    for(;;)
    {
        mode = HandApp_GetMode();
        allow_sensor_update = ((mode == HAND_APP_MODE_FREE) ||
                               (mode == HAND_APP_MODE_REHAB)) ? 1U : 0U;
        allow_servo_write = (mode == HAND_APP_MODE_FREE) ? 1U : 0U;

        if(allow_sensor_update == 0U)
        {
            for(i = Finger_Thumb; i < Finger_NUM_ALL ; i++)
            {
                finger_state[i].last_sent_angle = -1000;
            }
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }

        for(i = Finger_Thumb; i < Finger_NUM_ALL ; i++)
        {
            Finger_Control_UpdateOne(&Hand[i], &finger_state[i], send_deadband_base, allow_servo_write);
        }

#if FINGER_ALL_CTRL_DEBUG
        if((xTaskGetTickCount() - last_debug_tick) >= pdMS_TO_TICKS(FINGER_ALL_CTRL_DEBUG_PERIOD_MS))
        {
            Finger_Control_DebugDump(finger_state);
            last_debug_tick = xTaskGetTickCount();
        }
#endif

#if FINGER_PAIR_DEBUG
        if((xTaskGetTickCount() - last_pair_debug_tick) >= pdMS_TO_TICKS(FINGER_PAIR_DEBUG_PERIOD_MS))
        {
            Finger_Pair_DebugDump(finger_state);
            last_pair_debug_tick = xTaskGetTickCount();
        }
#endif

        /* 500ms 串口输出五指百分比 */
        {
            static TickType_t pct_tick = 0U;
            if((xTaskGetTickCount() - pct_tick) >= pdMS_TO_TICKS(500U))
            {
                char dbg[128];
                int len = snprintf(dbg, sizeof(dbg),
                    "F: T=%3u I=%3u M=%3u R=%3u L=%3u\r\n",
                    (unsigned)(angle_to_percent(finger_state[Finger_Thumb].angle_deg)),
                    (unsigned)(angle_to_percent(finger_state[Finger_Index].angle_deg)),
                    (unsigned)(angle_to_percent(finger_state[Finger_Middle].angle_deg)),
                    (unsigned)(angle_to_percent(finger_state[Finger_Ring].angle_deg)),
                    (unsigned)(angle_to_percent(finger_state[Finger_Little].angle_deg)));
                HAL_UART_Transmit(&huart3, (uint8_t *)dbg, (uint16_t)len, 100);
                pct_tick = xTaskGetTickCount();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(loop_ms));
    }
}

/*==================================================== PC Free Control ====================================================*/
static volatile uint8_t g_pc_free_targets[5] = {0U, 0U, 0U, 0U, 0U};
static volatile uint8_t g_pc_free_pending = 0U;

void FreeControl_SetTargets(uint8_t thumb,
                            uint8_t index,
                            uint8_t middle,
                            uint8_t ring,
                            uint8_t little)
{
    if(thumb > 100U) {
        thumb = 100U;
    }
    if(index > 100U) {
        index = 100U;
    }
    if(middle > 100U) {
        middle = 100U;
    }
    if(ring > 100U) {
        ring = 100U;
    }
    if(little > 100U) {
        little = 100U;
    }

    g_pc_free_targets[0] = thumb;
    g_pc_free_targets[1] = index;
    g_pc_free_targets[2] = middle;
    g_pc_free_targets[3] = ring;
    g_pc_free_targets[4] = little;
    g_pc_free_pending = 1U;
}

static uint8_t FreeControl_FetchTargets(uint8_t out_targets[5])
{
    if((out_targets == NULL) || (g_pc_free_pending == 0U))
    {
        return 0U;
    }

    for(uint8_t i = 0U; i < 5U; i++)
    {
        out_targets[i] = g_pc_free_targets[i];
    }

    g_pc_free_pending = 0U;
    return 1U;
}

static int16_t FreeControl_MapPercentToAngle(uint8_t percent)
{
    const int16_t open_angle = 49;
    const int16_t close_angle = 190;

    if(percent > 100U)
    {
        percent = 100U;
    }

    return (int16_t)(open_angle + (((close_angle - open_angle) * (int32_t)percent) / 100));
}

void PC_FreeControl_TASK(void *arg)
{
    const uint8_t servo_ids[5] = {5U, 1U, 2U, 3U, 4U};
    uint16_t target_steps[5];
    uint16_t speeds[5] = {900U, 900U, 900U, 900U, 900U};
    uint8_t current_targets[5] = {0U, 0U, 0U, 0U, 0U};
    uint8_t last_targets[5] = {255U, 255U, 255U, 255U, 255U};
    uint8_t changed;

    (void)arg;
    vTaskDelay(pdMS_TO_TICKS(500));

    for(;;)
    {
        if(FreeControl_FetchTargets(current_targets) != 0U)
        {
            changed = 0U;
            for(uint8_t i = 0U; i < 5U; i++)
            {
                if(current_targets[i] != last_targets[i])
                {
                    changed = 1U;
                }

                target_steps[i] = (uint16_t)Step_Angle_Update(FreeControl_MapPercentToAngle(current_targets[i]));
                last_targets[i] = current_targets[i];
            }

            if(changed != 0U)
            {
                SyncWritePos(servo_ids, 5, target_steps, 0, speeds);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(15));
    }
}

/*========================================================EMG==================================================================*/
static uint16_t EMG_CalcActivityWindow(uint16_t *avg_out)
{
    enum { EMG_WINDOW_SAMPLES = 32 };
    uint16_t samples[EMG_WINDOW_SAMPLES];
    uint32_t sum = 0;
    uint32_t abs_sum = 0;
    uint16_t valid = 0;
    uint16_t avg = 0;

    for(int i = 0; i < EMG_WINDOW_SAMPLES; i++)
    {
        uint16_t raw = EMG_ReadRaw();
        if(raw == UINT16_MAX)
        {
            continue;
        }

        samples[valid++] = raw; //记录每一次adc值  存入数组 中间vaild索引加1
        sum += raw;
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    if(valid == 0U)
    {
        if(avg_out != NULL) 
        {
            *avg_out = 0;
        }
        return 0;
    }

    avg = (uint16_t)(sum / valid);

    for(uint16_t i = 0; i < valid; i++)
    {
        int32_t diff = (int32_t)samples[i] - (int32_t)avg;
        if(diff < 0)
        {
            diff = -diff;
        }
        abs_sum += (uint32_t)diff;
    }

    if(avg_out != NULL)
    {
        *avg_out = avg;
    }

    return (uint16_t)(abs_sum / valid);
}

static void EMG_PublishMetrics(uint16_t avg,uint16_t activity,uint16_t baseline,
                               uint16_t delta,uint16_t level,uint16_t peak,emg_state_t state,
                               uint32_t elapsed_sec,uint8_t running)
{
    taskENTER_CRITICAL();

    g_emg_metrics.avg = avg;
    g_emg_metrics.activity = activity;
    g_emg_metrics.baseline = baseline;
    g_emg_metrics.delta = delta;
    g_emg_metrics.level = level;
    g_emg_metrics.peak = peak;
    g_emg_metrics.state = state;
    g_emg_metrics.elapsed_sec = elapsed_sec;
    g_emg_metrics.running = running;

    taskEXIT_CRITICAL();

}

void EMG_TASK(void *arg)
{
    (void)arg;

    uint16_t peak_level = 0U;
    uint32_t elapsed_sec = 0U;
    TickType_t session_start_tick = 0;
    emg_state_t state = EMG_STATE_IDLE;


    char emg_buf[96];
    uint32_t baseline_sum = 0;
    uint16_t avg = 0;
    uint16_t activity = 0;
    uint16_t baseline_activity = 0;
    int32_t delta = 0;
    uint16_t level = 0;
    uint16_t level_target = 0;
    uint16_t session_peak_delta = 40;
    uint16_t adaptive_full_scale = 24;

    for(;;)
    {
        if(HandApp_IsMode(HAND_APP_MODE_EMG) == 0U)
        {
            g_auto_force = 0U;
            level = 0U;
            peak_level = 0U;

            EMG_PublishMetrics(0U,
                            0U,
                            0U,
                            0U,
                            0U,
                            0U,
                            EMG_STATE_IDLE,
                            0U,
                            0U);

            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(800));

        baseline_activity = UINT16_MAX;

        for(int i = 0; i < 20; i++)
        {
            uint16_t sample_activity;

            if(HandApp_IsMode(HAND_APP_MODE_EMG) == 0U)
            {
                break;
            }

            sample_activity = EMG_CalcActivityWindow(&avg);

            if(sample_activity < baseline_activity)
            {
                baseline_activity = sample_activity;
            }

            vTaskDelay(pdMS_TO_TICKS(10));
        }

        if(baseline_activity == UINT16_MAX)
        {
            baseline_activity = 0U;
        }
        session_peak_delta = 40U;

        peak_level = 0U;
        level = 0U;
        session_start_tick = xTaskGetTickCount();


        while(HandApp_IsMode(HAND_APP_MODE_EMG) != 0U)
        {
            activity = EMG_CalcActivityWindow(&avg);

            delta = (int32_t)activity - (int32_t)baseline_activity;
            if(delta < 0)
            {
                delta = 0;
            }

  
            if((uint16_t)delta > session_peak_delta)
            {
                session_peak_delta = (uint16_t)delta;
            }


            adaptive_full_scale = (uint16_t)((session_peak_delta * 6U) / 10U);
            if(adaptive_full_scale < 20U)
            {
                adaptive_full_scale = 20U;
            }

            if((uint16_t)delta >= adaptive_full_scale)
            {
                level_target = 100U;
            }
            else
            {
                level_target = (uint16_t)(((uint32_t)delta * 100U) / adaptive_full_scale);
            }


            if(level_target >= level)
            {
                level = (uint16_t)(((uint32_t)level + (uint32_t)level_target * 3U) / 4U);
            }
            else
            {
                level = (uint16_t)(((uint32_t)level * 7U + (uint32_t)level_target) / 8U);
            }
            g_auto_force = level;

            state = EMG_STATE_REST;


            if(level >= 75U)
            {
                state = EMG_STATE_STRONG;
            }
            else if(level >= 20U)
            {
                state = EMG_STATE_ACTIVE;
            }
            else
            {
                state = EMG_STATE_REST;
            }
         if(level > peak_level)
            {
                peak_level = level;
            }

            elapsed_sec = (uint32_t)((xTaskGetTickCount() - session_start_tick) / pdMS_TO_TICKS(1000));

            EMG_PublishMetrics(avg,
                            activity,
                            baseline_activity,
                            (uint16_t)delta,
                            level,
                            peak_level,
                            state,
                            elapsed_sec,
                            1U);


            sprintf(emg_buf,
                    "EMG avg=%u act=%u base=%u delta=%ld lvl=%u\r\n",
                    avg,
                    activity,
                    baseline_activity,
                    delta,
                    level);
            HAL_UART_Transmit(&huart3, (uint8_t *)emg_buf, strlen(emg_buf), 100);

            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}
//====================================================================dht11==============================================================================

#if ENABLE_DHT11_TASK
void dht11_test(void * arg){
    (void)arg;
    
    uint8_t temp = 0;
    uint8_t hum = 9;
    uint8_t ret = 0;
    uint8_t raw[5] = {0};
    uint8_t sum = 0;
    char buf[128];
    dht11_init();
    vTaskDelay(pdMS_TO_TICKS(1500));

    for(;;)
    {
        ret = dht11_read_debug(&temp, &hum, raw, &sum);
        if(ret == 0)
        {
            g_dht_temp = temp;
            g_dht_hum = hum;
            g_dht_ok = 1U;
            g_dht_fail = 0U;
            sprintf(buf, "T=%u H=%u\r\n", temp, hum);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 100);
        }else
        {
            g_dht_ok = 0U;
            g_dht_fail = ret;
            sprintf(buf,
                    "DHT11 fail=%u raw=%u,%u,%u,%u,%u sum=%u\r\n",
                    ret,
                    raw[0], raw[1], raw[2], raw[3], raw[4],
                    sum);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 100);
        }
        
        vTaskDelay(pdMS_TO_TICKS(2000));

    }






}
#else
void dht11_test(void * arg)
{
    (void)arg;
    vTaskSuspend(NULL);
}
#endif


void flex_debug_task(void * arg)
{
   (void)arg;

    char buf[80];
    uint16_t raw0 = 0;
    uint16_t raw1 = 0;
    uint16_t last0 = get_flex_channel_adc(0);
    uint16_t last1 = get_flex_channel_adc(1);

    for(;;)
    {
        raw0 = get_flex_channel_adc(0);
        raw1 = get_flex_channel_adc(1);

        uint16_t d0 = Flex_GetAbs(raw0, last0);
        uint16_t d1 = Flex_GetAbs(raw1, last1);

        int len = snprintf(buf, sizeof(buf),
                           "ch0=%u d0=%u ch1=%u d1=%u%s\r\n",
                           raw0, d0, raw1, d1,
                           (d0 > 20U || d1 > 20U) ? " *" : "");

        if(len > 0)
        {
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, (uint16_t)len, 100);
        }

        last0 = raw0;
        last1 = raw1;

        vTaskDelay(pdMS_TO_TICKS(50));
    }


}




void EMG_GetMetrics(emg_metrics_t *out)
{
    if(out == NULL)
    {
        return;
    }

    taskENTER_CRITICAL();
    *out = g_emg_metrics;
    taskEXIT_CRITICAL();
}
