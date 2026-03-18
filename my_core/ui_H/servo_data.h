#ifndef SERVO_DATA_H
#define SERVO_DATA_H

#include <stdint.h>

/*
 * task_servo 写入, task_lvgl 通过 lv_timer 回调读取
 * 用 volatile 防止编译器优化掉跨任务的读写
 */
typedef struct {
    volatile int16_t  pos;      /* 当前位置 (0~4095)    */
    volatile int16_t  speed;    /* 当前速度 (step/s)    */
    volatile int16_t  voltage;  /* 电压 (单位 0.1V)     */
    volatile int16_t  temp;     /* 温度 (°C)            */
    volatile uint16_t goal_pos; /* 目标位置 (由主控写入) */
} ServoFeedback_t;

extern ServoFeedback_t g_servo;

#endif /* SERVO_DATA_H */
