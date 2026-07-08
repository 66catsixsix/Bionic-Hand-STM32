#include "Finger.h"
#include "FreeRtos_task.h"
#include "lvgl.h"
#include "main.h"

#include <string.h>

Fingertask_t Hand[Finger_NUM_ALL];
volatile uint8_t g_auto_status = 0;
volatile uint16_t g_auto_cycles = 5;
volatile uint16_t g_auto_force = 32;
volatile uint8_t g_dht_temp = 28U;
volatile uint8_t g_dht_hum = 71U;
volatile uint8_t g_dht_ok = 1U;
volatile uint8_t g_dht_fail = 0U;

static hand_app_mode_t s_mode = HAND_APP_MODE_IDLE;

void HandApp_SetMode(hand_app_mode_t mode)
{
    s_mode = mode;
}

hand_app_mode_t HandApp_GetMode(void)
{
    return s_mode;
}

uint8_t HandApp_IsMode(hand_app_mode_t mode)
{
    return (uint8_t)(s_mode == mode);
}

void EMG_GetMetrics(emg_metrics_t *out)
{
    uint32_t tick;
    uint16_t phase;
    uint16_t level;

    if(out == NULL) {
        return;
    }

    memset(out, 0, sizeof(*out));

    if(s_mode != HAND_APP_MODE_EMG) {
        out->state = EMG_STATE_IDLE;
        return;
    }

    tick = lv_tick_get();
    phase = (uint16_t)((tick / 18U) % 200U);
    level = (phase <= 100U) ? phase : (uint16_t)(200U - phase);

    out->avg = (uint16_t)(1700U + level * 3U);
    out->activity = (uint16_t)(45U + level * 5U);
    out->baseline = 45U;
    out->delta = (uint16_t)(level * 5U);
    out->level = level;
    out->peak = 100U;
    out->elapsed_sec = tick / 1000U;
    out->running = 1U;

    if(level >= 75U) {
        out->state = EMG_STATE_STRONG;
    }
    else if(level >= 20U) {
        out->state = EMG_STATE_ACTIVE;
    }
    else {
        out->state = EMG_STATE_REST;
    }
}

uint32_t HAL_GetTick(void)
{
    return lv_tick_get();
}

void Hand_Init(void)
{
    static const uint8_t servo_ids[Finger_NUM_ALL] = {
        5, 1, 2, 3, 4, 6, 7, 8, 9, 10
    };
    static const uint16_t adc_scan[Finger_NUM_ALL] = {
        8, 0, 2, 4, 6, 1, 3, 5, 7, 9
    };
    static const uint16_t demo_step[Finger_NUM_ALL] = {
        420, 530, 610, 480, 700, 430, 520, 490, 560, 390
    };
    static const uint16_t open_refs[Finger_NUM_ALL] = {
        1875, 2005, 2093, 2340, 2295, 1765, 1855, 1734, 1925, 1675
    };
    static const uint16_t close_refs[Finger_NUM_ALL] = {
        1280, 1850, 1910, 2165, 1320, 813, 789, 931, 880, 665
    };

    memset(Hand, 0, sizeof(Hand));

    for(uint8_t i = 0; i < Finger_NUM_ALL; i++) {
        Hand[i].servo_id = servo_ids[i];
        Hand[i].adc_scan = adc_scan[i];
        Hand[i].target_angle = (int16_t)demo_step[i];
        Hand[i].calib_ok = 1;
        Hand[i].adc_open_ref = open_refs[i];
        Hand[i].adc_close_ref = close_refs[i];
    }
}

void Hand_Update_Motor(void)
{
}

void serival_auto(lv_event_t *e)
{
    LV_UNUSED(e);
}

void auto_scr_init(void)
{
}
