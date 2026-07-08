#ifndef MY_FreeRots_Task
#define  MY_FreeRots_Task

#include "FreeRTOS.h"

#ifndef ENABLE_DHT11_TASK
#define ENABLE_DHT11_TASK 1
#endif

typedef enum {

     serival_mode = 1,
     auto_mode = 0
    
} servo_mode_t;

typedef enum {
     HAND_APP_MODE_IDLE = 0,
     HAND_APP_MODE_FREE,
     HAND_APP_MODE_REHAB,
     HAND_APP_MODE_EMG,
     HAND_APP_MODE_CALIB
} hand_app_mode_t;

typedef enum{

     EMG_STATE_IDLE = 0, //未进入测试
     EMG_STATE_REST,    //当前采样窗口的肌电活动
     EMG_STATE_ACTIVE,   //有效发力
     EMG_STATE_STRONG   //强发力状态
}emg_state_t;

typedef struct 
{
     uint16_t avg;       //当前采样的adc平均值
     uint16_t activity; //当前采样的肌电活动读
     uint16_t baseline;  //进入肌电模式后测试得到的静息基线活动读
     uint16_t delta;     //当前活动度
     uint16_t level;     //映射发力等级
     uint16_t peak;      //肌电测试过程中最高发力等级
     uint32_t elapsed_sec;//本次肌电测试持续时间
     emg_state_t state;//当前发力状态结果
     uint8_t running; //运行标志位
}emg_metrics_t;


void EMG_GetMetrics(emg_metrics_t *out);//lvgl定是刷新指标

extern volatile uint8_t g_dht_temp;
extern volatile uint8_t g_dht_hum;
extern volatile uint8_t g_dht_ok;
extern volatile uint8_t g_dht_fail;

void HandApp_SetMode(hand_app_mode_t mode);
hand_app_mode_t HandApp_GetMode(void);
uint8_t HandApp_IsMode(hand_app_mode_t mode);

// 任务函数声明
void task_lvgl(void *arg);
void task_servo(void *arg);
void flex_task(void* arg);
void flex_calibration_task(void * arg);
void task_usart3(void *arg);
void id_change_task(void * arg);
void id_total_task(void * arg);
void task_touch(void *arg); 
void EMG_TASK(void * arg);
void dht11_test(void * arg);
void flex_debug_task(void * arg);

void Finger_Index_Root_TASK(void * arg);
void Finger_All_Control_TASK(void * arg);
#endif
