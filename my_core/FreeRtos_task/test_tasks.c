#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "SCServo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Finger.h"
#include "algo.h"
#include "flex.h"
#include "FreeRtos_task.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include <stdlib.h>


#define TEST_FLEX_CAPTURE_MS     3000U
#define TEST_FLEX_SAMPLE_MS      20U
#define TEST_FLEX_SETTLE_MS      1000U

#define TEST_ACTIVE_JOINT       Finger_Index

#define INDEX_EVENT_SAMPLE_MS        20U
#define INDEX_EVENT_PRE_FRAMES       48U
#define INDEX_EVENT_POST_FRAMES      32U
#define INDEX_EVENT_COOLDOWN_MS      1200U
#define INDEX_EVENT_ACTIVITY_DELTA_ADC 10U
#define INDEX_EVENT_COUPLE_DELTA_ADC 8U
#define INDEX_EVENT_HOLD_FRAMES      12U



/*

 * Temporary test sandbox.
 *
 * Put bring-up / debug / one-off verification code here instead of mixing it
 * into normal feature files. When a test is no longer needed, remove it here.
 */

extern UART_HandleTypeDef huart3;
extern Fingertask_t Hand[];
extern uint16_t get_flex_channel_adc(uint8_t ch_index);

typedef struct
{
    const char *name;
    uint8_t enabled;
    uint8_t adc_scan;
    uint8_t servo_id;
    uint16_t adc_open_ref;
    uint16_t adc_close_ref;
    int16_t open_angle_deg;
    int16_t close_angle_deg;
    uint16_t open_snap_adc;
    uint16_t close_snap_adc;
    int16_t angle_deadband_deg;
    uint16_t loop_ms;
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
} TestJointConfig_t;

typedef struct
{
    uint16_t pos_adc;
    uint16_t vel_adc;
    uint16_t last_vel_adc;
} TestFilterState_t;

typedef struct
{
    TickType_t tick;
    uint16_t root_raw;
    uint16_t root_pos;
    uint16_t root_delta;
    int16_t root_angle;
    uint16_t tip_raw;
    uint16_t tip_pos;
    uint16_t tip_delta;
    int16_t tip_angle;
} IndexEventFrame_t;

static const TestJointConfig_t g_test_joint_cfg[Finger_NUM_ALL] = {
    [Finger_Thumb] = {
        .name = "thumb_root",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 5
    },
    [Finger_Index] = {
        .name = "index_root",
        .enabled = 1,
        .adc_scan = 0,
        .servo_id = 1,
        .adc_open_ref = 2260,
        .adc_close_ref = 2178,
        .open_angle_deg = 49,
        .close_angle_deg = 190,
        .open_snap_adc = 8,
        .close_snap_adc = 6,
        .angle_deadband_deg = 5,
        .loop_ms = 10,
        .pos_fast_delta_adc = 16,
       .speed_delta_1 = 2,
        .speed_delta_2 = 5,
        .speed_delta_3 = 10,
        .speed_delta_4 = 14,

        .speed_val_1 = 180,
        .speed_val_2 = 320,
        .speed_val_3 = 550,
        .speed_val_4 = 1200,
        .speed_val_5 = 1600
    },
    [Finger_Middle] = {
        .name = "middle_root",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 2
    },
    [Finger_Ring] = {
        .name = "ring_root",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 3
    },
    [Finger_Little] = {
        .name = "little_root",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 4
    },
   [Finger_Index_L] = {
    .name = "index_tip",
    .enabled = 1,
    .adc_scan = 1,
    .servo_id = 6
    },
    [Finger_Middle_L] = {
        .name = "middle_tip",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 7
    },
    [Finger_Ring_L] = {
        .name = "ring_tip",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 8
    },
    [Finger_Little_L] = {
        .name = "little_tip",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 9
    },
    [Finger_Thumb_L] = {
        .name = "thumb_tip",
        .enabled = 0,
        .adc_scan = 255,
        .servo_id = 10
    }
};

static void test_log(const char *fmt, ...)
{
    char buf[160];
    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if(len < 0) {
        return;
    }
    if(len >= (int)sizeof(buf)) {
        len = (int)sizeof(buf) - 1;
    }

    HAL_UART_Transmit(&huart3, (uint8_t *)buf, (uint16_t)len, 100);
}

static uint16_t test_capture_adc_average(uint8_t scan,
                                         uint32_t capture_ms,
                                         uint32_t sample_ms,
                                         uint16_t *min_out,
                                         uint16_t *max_out)
{
    uint32_t sample_sum = 0;
    uint32_t sample_cnt = 0;
    uint16_t sample_min = 4095;
    uint16_t sample_max = 0;
    TickType_t start_tick = xTaskGetTickCount();
    TickType_t wait_ticks = pdMS_TO_TICKS(capture_ms);

    while((xTaskGetTickCount() - start_tick) < wait_ticks) {
        uint16_t raw = get_flex_channel_adc(scan);

        if(raw < sample_min) {
            sample_min = raw;
        }
        if(raw > sample_max) {
            sample_max = raw;
        }

        sample_sum += raw;
        sample_cnt++;
        vTaskDelay(pdMS_TO_TICKS(sample_ms));
    }

    if(sample_cnt == 0U) {
        sample_cnt = 1U;
    }

    if(min_out) {
        *min_out = sample_min;
    }
    if(max_out) {
        *max_out = sample_max;
    }

    return (uint16_t)(sample_sum / sample_cnt);
}

static uint16_t test_abs_diff_u16(uint16_t a, uint16_t b)
{
    if(a >= b) {
        return (uint16_t)(a - b);
    }
    return (uint16_t)(b - a);
}

static uint16_t test_median3_u16(uint16_t a, uint16_t b, uint16_t c)
{
    if(a > b) {
        uint16_t t = a;
        a = b;
        b = t;
    }

    if(b > c) {
        uint16_t t = b;
        b = c;
        c = t;
    }

    if(a > b) {
        uint16_t t = a;
        a = b;
        b = t;
    }

    return b;
}


static void test_print_capture_stats(const char *name,
                                     uint16_t avg,
                                     uint16_t min_val,
                                     uint16_t max_val)
{
    test_log("%s avg=%u min=%u max=%u span=%u\r\n",
             name,
            (unsigned int)avg,
            (unsigned int)min_val,
            (unsigned int)max_val,
            (unsigned int)(max_val - min_val));
        }

static const TestJointConfig_t *test_get_active_joint_cfg(void)
{
    return &g_test_joint_cfg[TEST_ACTIVE_JOINT];
}

static uint16_t test_joint_update_pos_filter(const TestJointConfig_t *cfg,
                                             uint16_t prev_adc,
                                             uint16_t raw_adc)
{
    uint16_t delta_adc = test_abs_diff_u16(prev_adc, raw_adc);

   if(delta_adc >= cfg->pos_fast_delta_adc) {
    return (uint16_t)(((uint32_t)prev_adc + (uint32_t)raw_adc * 3U) / 4U);
}


    if(delta_adc >= 6U) {
        return (uint16_t)(((uint32_t)prev_adc * 3U + raw_adc) / 4U);
    }

    return (uint16_t)(((uint32_t)prev_adc * 7U + raw_adc) / 8U);
}

static uint16_t test_joint_update_vel_filter(uint16_t prev_adc, uint16_t raw_adc)
{
    uint16_t delta_adc = test_abs_diff_u16(prev_adc, raw_adc);

    if(delta_adc <= 2U) {
        return prev_adc;
    }

    if(delta_adc <= 6U) {
        return (uint16_t)(((uint32_t)prev_adc * 3U + raw_adc) / 4U);
    }

    return (uint16_t)(((uint32_t)prev_adc + raw_adc) / 2U);
}

static uint16_t test_joint_speed_from_delta(const TestJointConfig_t *cfg, uint16_t delta_adc)
{
    if(delta_adc <= cfg->speed_delta_1) {
        return cfg->speed_val_1;
    }
    if(delta_adc <= cfg->speed_delta_2) {
        return cfg->speed_val_2;
    }
    if(delta_adc <= cfg->speed_delta_3) {
        return cfg->speed_val_3;
    }
    if(delta_adc <= cfg->speed_delta_4) {
        return cfg->speed_val_4;
    }
    return cfg->speed_val_5;
}


void test_tasks_placeholder(void)
{
    /* Keep this file buildable before real test tasks are added. */
}

void test_single_servo_task(void *arg)
{
    (void)arg;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void test_single_flex_task(void *arg)
{
    (void)arg;
    uint16_t open1_avg;
    uint16_t open1_min;
    uint16_t open1_max;
    uint16_t close_avg;
    uint16_t close_min;
    uint16_t close_max;
    uint16_t open2_avg;
    uint16_t open2_min;
    uint16_t open2_max;
    uint16_t diff_open1_close;
    uint16_t diff_open2_close;
    uint16_t diff_open1_open2;
    const TestJointConfig_t *cfg = test_get_active_joint_cfg();

    vTaskDelay(pdMS_TO_TICKS(1500));

    test_log("\r\n=== FLEX OPEN-CLOSE-OPEN TEST START ===\r\n");
    test_log("scan=%u\r\n", (unsigned int)cfg->adc_scan);
    test_log("Step1: keep finger OPEN/STRAIGHT for %lu ms\r\n",
             (unsigned long)TEST_FLEX_CAPTURE_MS);
    vTaskDelay(pdMS_TO_TICKS(TEST_FLEX_SETTLE_MS));

    open1_avg = test_capture_adc_average(cfg->adc_scan,
                                         TEST_FLEX_CAPTURE_MS,
                                         TEST_FLEX_SAMPLE_MS,
                                         &open1_min,
                                         &open1_max);

    test_print_capture_stats("OPEN1", open1_avg, open1_min, open1_max);

    test_log("Step2: keep finger CLOSE/BENT for %lu ms\r\n",
             (unsigned long)TEST_FLEX_CAPTURE_MS);
    vTaskDelay(pdMS_TO_TICKS(TEST_FLEX_SETTLE_MS));

    close_avg = test_capture_adc_average(cfg->adc_scan,
                                         TEST_FLEX_CAPTURE_MS,
                                         TEST_FLEX_SAMPLE_MS,
                                         &close_min,
                                         &close_max);

    test_print_capture_stats("CLOSE", close_avg, close_min, close_max);

    test_log("Step3: keep finger OPEN/STRAIGHT again for %lu ms\r\n",
             (unsigned long)TEST_FLEX_CAPTURE_MS);
    vTaskDelay(pdMS_TO_TICKS(TEST_FLEX_SETTLE_MS));

    open2_avg = test_capture_adc_average(cfg->adc_scan,
                                         TEST_FLEX_CAPTURE_MS,
                                         TEST_FLEX_SAMPLE_MS,
                                         &open2_min,
                                         &open2_max);

    test_print_capture_stats("OPEN2", open2_avg, open2_min, open2_max);

    diff_open1_close = test_abs_diff_u16(open1_avg, close_avg);
    diff_open2_close = test_abs_diff_u16(open2_avg, close_avg);
    diff_open1_open2 = test_abs_diff_u16(open1_avg, open2_avg);

    test_log("DIFF open1-close=%u open2-close=%u open1-open2=%u\r\n",
             (unsigned int)diff_open1_close,
             (unsigned int)diff_open2_close,
             (unsigned int)diff_open1_open2);

    if(diff_open1_close < 20U || diff_open2_close < 20U) {
        test_log("Result: open-close delta is very small. Signal/noise issue is likely.\r\n");
    }
    else if(diff_open1_close < 80U || diff_open2_close < 80U) {
        test_log("Result: open-close delta is small. Mapping will be sensitive to noise.\r\n");
    }
    else {
        test_log("Result: open-close delta looks usable for mapping.\r\n");
    }

    if(diff_open1_open2 < 20U) {
        test_log("Return check: OPEN1 and OPEN2 are close. Hysteresis looks mild.\r\n");
    }
    else if(diff_open1_open2 < 60U) {
        test_log("Return check: OPEN1 and OPEN2 differ a bit. Some hysteresis or mounting shift exists.\r\n");
    }
    else {
        test_log("Return check: OPEN1 and OPEN2 differ a lot. Hysteresis / mounting / repeatability issue is strong.\r\n");
    }

    vTaskSuspend(NULL);
}

static void index_event_capture_frame(IndexEventFrame_t *frame,
                                      IndexEventFrame_t *last_frame,
                                      uint8_t has_last)
{
    Fingertask_t *root_cfg = &Hand[Finger_Index];
    Fingertask_t *tip_cfg = &Hand[Finger_Index_L];
    uint16_t root_raw;
    uint16_t tip_raw;

    root_raw = get_flex_channel_adc(root_cfg->adc_scan);
    tip_raw = get_flex_channel_adc(tip_cfg->adc_scan);

    frame->tick = xTaskGetTickCount();
    frame->root_raw = root_raw;
    frame->tip_raw = tip_raw;

    if(has_last == 0U)
    {
        frame->root_pos = root_raw;
        frame->tip_pos = tip_raw;
        frame->root_delta = 0U;
        frame->tip_delta = 0U;
    }
    else
    {
        frame->root_pos = Flex_UpdatePosFilter(root_cfg, last_frame->root_pos, root_raw);
        frame->tip_pos = Flex_UpdatePosFilter(tip_cfg, last_frame->tip_pos, tip_raw);
        frame->root_delta = Flex_GetAbs(frame->root_pos, last_frame->root_pos);
        frame->tip_delta = Flex_GetAbs(frame->tip_pos, last_frame->tip_pos);
    }

    frame->root_angle = Flex_map_to_angle(root_cfg, frame->root_pos);
    frame->tip_angle = Flex_map_to_angle(tip_cfg, frame->tip_pos);
}

static void index_event_print_frame(const IndexEventFrame_t *frame,
                                    TickType_t base_tick,
                                    const char *mark)
{
    test_log("%s t=%lu rr=%u rp=%u ra=%d rd=%u | tr=%u tp=%u ta=%d td=%u\r\n",
             mark,
             (unsigned long)((frame->tick - base_tick) * portTICK_PERIOD_MS),
             (unsigned int)frame->root_raw,
             (unsigned int)frame->root_pos,
             (int)frame->root_angle,
             (unsigned int)frame->root_delta,
             (unsigned int)frame->tip_raw,
             (unsigned int)frame->tip_pos,
             (int)frame->tip_angle,
             (unsigned int)frame->tip_delta);
}

static void index_event_dump_pre_frames(const IndexEventFrame_t frames[INDEX_EVENT_PRE_FRAMES],
                                        uint8_t head,
                                        uint8_t count)
{
    uint8_t i;
    uint8_t start;
    TickType_t base_tick;

    if(count == 0U)
    {
        return;
    }

    if(count < INDEX_EVENT_PRE_FRAMES)
    {
        start = 0U;
    }
    else
    {
        start = head;
    }

    base_tick = frames[start].tick;

    for(i = 0U; i < count; i++)
    {
        uint8_t idx = (uint8_t)((start + i) % INDEX_EVENT_PRE_FRAMES);
        index_event_print_frame(&frames[idx], base_tick, "pre");
    }
}

static uint8_t index_event_mode_is_active(void)
{
    return ((HandApp_IsMode(HAND_APP_MODE_FREE) != 0U) ||
            (HandApp_IsMode(HAND_APP_MODE_REHAB) != 0U)) ? 1U : 0U;
}

void test_index_event_recorder_task(void *arg)
{
    (void)arg;

    IndexEventFrame_t pre_frames[INDEX_EVENT_PRE_FRAMES] = {0};
    IndexEventFrame_t frame = {0};
    IndexEventFrame_t last_frame = {0};
    uint8_t pre_head = 0U;
    uint8_t pre_count = 0U;
    uint8_t has_last = 0U;
    uint16_t tip_closed_root_open_hold = 0U;
    TickType_t cooldown_until = 0U;
    uint8_t active_logged = 0U;

    vTaskDelay(pdMS_TO_TICKS(1500));
    test_log("\r\n=== IDX EVENT RECORDER READY ===\r\n");
    test_log("Run full-hand Free/Rehab movement. Prints only on suspected index events.\r\n");

    for(;;)
    {
        const char *event_name = NULL;
        TickType_t now;

        if(index_event_mode_is_active() == 0U)
        {
            has_last = 0U;
            pre_head = 0U;
            pre_count = 0U;
            tip_closed_root_open_hold = 0U;
            active_logged = 0U;
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        if(active_logged == 0U)
        {
            test_log("IDX recorder active in Free/Rehab mode\r\n");
            active_logged = 1U;
        }

        index_event_capture_frame(&frame, &last_frame, has_last);
        uint16_t root_raw_step = (has_last != 0U) ? Flex_GetAbs(frame.root_raw, last_frame.root_raw) : 0U;
        uint16_t tip_raw_step = (has_last != 0U) ? Flex_GetAbs(frame.tip_raw, last_frame.tip_raw) : 0U;
        has_last = 1U;

        pre_frames[pre_head] = frame;
        pre_head = (uint8_t)((pre_head + 1U) % INDEX_EVENT_PRE_FRAMES);
        if(pre_count < INDEX_EVENT_PRE_FRAMES)
        {
            pre_count++;
        }

        if((frame.tip_angle >= 95) && (frame.root_angle <= 140))
        {
            if(tip_closed_root_open_hold < 1000U)
            {
                tip_closed_root_open_hold++;
            }
        }
        else
        {
            tip_closed_root_open_hold = 0U;
        }

        now = xTaskGetTickCount();
        if((int32_t)(now - cooldown_until) >= 0)
        {
            if((tip_raw_step >= INDEX_EVENT_COUPLE_DELTA_ADC) &&
               (root_raw_step >= INDEX_EVENT_COUPLE_DELTA_ADC))
            {
                event_name = "IDX_COUPLE";
            }
            else if(tip_closed_root_open_hold >= INDEX_EVENT_HOLD_FRAMES)
            {
                event_name = "IDX_ROOT_STUCK";
            }
            else if((root_raw_step >= INDEX_EVENT_ACTIVITY_DELTA_ADC) ||
                    (tip_raw_step >= INDEX_EVENT_ACTIVITY_DELTA_ADC) ||
                    (frame.root_delta >= INDEX_EVENT_ACTIVITY_DELTA_ADC) ||
                    (frame.tip_delta >= INDEX_EVENT_ACTIVITY_DELTA_ADC))
            {
                event_name = "IDX_ACTIVITY";
            }
        }

        if(event_name != NULL)
        {
            uint8_t i;
            TickType_t event_tick = frame.tick;

            test_log("\r\n=== %s START ===\r\n", event_name);
            test_log("root=ch%u/id%u tip=ch%u/id%u sample_ms=%u\r\n",
                     (unsigned int)Hand[Finger_Index].adc_scan,
                     (unsigned int)Hand[Finger_Index].servo_id,
                     (unsigned int)Hand[Finger_Index_L].adc_scan,
                     (unsigned int)Hand[Finger_Index_L].servo_id,
                     (unsigned int)INDEX_EVENT_SAMPLE_MS);
            index_event_dump_pre_frames(pre_frames, pre_head, pre_count);

            for(i = 0U; i < INDEX_EVENT_POST_FRAMES; i++)
            {
                vTaskDelay(pdMS_TO_TICKS(INDEX_EVENT_SAMPLE_MS));
                last_frame = frame;
                index_event_capture_frame(&frame, &last_frame, 1U);
                index_event_print_frame(&frame, event_tick, "post");
            }

            test_log("=== %s END ===\r\n\r\n", event_name);

            cooldown_until = xTaskGetTickCount() + pdMS_TO_TICKS(INDEX_EVENT_COOLDOWN_MS);
            has_last = 0U;
            pre_head = 0U;
            pre_count = 0U;
            tip_closed_root_open_hold = 0U;
        }
        else
        {
            last_frame = frame;
            vTaskDelay(pdMS_TO_TICKS(INDEX_EVENT_SAMPLE_MS));
        }
    }
}



// //PA0
// static int16_t test_abs_i16(int16_t x)
// {
//     return (x >= 0) ? x : -x;
// }

static int16_t pa0_map_to_index_root_angle(uint16_t adc)
{
    const TestJointConfig_t *cfg = test_get_active_joint_cfg();
    int32_t angle;
 

    if(adc >= (cfg->adc_open_ref - cfg->open_snap_adc)) {
        return cfg->open_angle_deg;
    }

    if(adc <= (cfg->adc_close_ref + cfg->close_snap_adc)) {
        return cfg->close_angle_deg;
    }

    if(adc > cfg->adc_open_ref) {
        adc = cfg->adc_open_ref;
    }

    if(adc < cfg->adc_close_ref) {
        adc = cfg->adc_close_ref;
    }

    angle = cfg->open_angle_deg +
            ((int32_t)(cfg->adc_open_ref - adc) *
             (cfg->close_angle_deg - cfg->open_angle_deg)) /
            (cfg->adc_open_ref - cfg->adc_close_ref);

    return (int16_t)angle;
}



void test_pa0_index_root_task(void *arg)
{
    (void)arg;
    uint16_t raw_adc;
    uint16_t filt_pos_adc;
    uint16_t filt_vel_adc;
    uint16_t last_vel_adc;
    uint16_t delta_adc;
    uint16_t servo_speed;
    uint16_t raw_adc_1;
    uint16_t raw_adc_2;
    uint16_t raw_med_adc;

    int16_t angle_deg;
    int16_t last_sent_angle = -1000;
    const TestJointConfig_t *cfg = test_get_active_joint_cfg();



   vTaskDelay(pdMS_TO_TICKS(1500));

    delta_adc = 0U;
    servo_speed = cfg->speed_val_1;

    raw_adc = get_flex_channel_adc(cfg->adc_scan);
    filt_pos_adc = raw_adc;
    filt_vel_adc = raw_adc;
    last_vel_adc = raw_adc;
    raw_adc_1 = raw_adc;
    raw_adc_2 = raw_adc;
    raw_med_adc = raw_adc;


    test_log("\r\n=== PA0 INDEX ROOT TEST START ===\r\n");

 for(;;)
{
   raw_adc = get_flex_channel_adc(cfg->adc_scan);

raw_med_adc = test_median3_u16(raw_adc, raw_adc_1, raw_adc_2);
raw_adc_2 = raw_adc_1;
raw_adc_1 = raw_adc;

/* 位置通道：吃中值后的ADC，先把单点尖峰去掉 */
filt_pos_adc = test_joint_update_pos_filter(cfg, filt_pos_adc, raw_med_adc);

/* 速度通道：也先吃中值后的ADC，不然speed会一直被尖峰冲高 */
filt_vel_adc = test_joint_update_vel_filter(filt_vel_adc, raw_med_adc);


    delta_adc = (uint16_t)test_abs_i16((int16_t)filt_vel_adc - (int16_t)last_vel_adc);
    last_vel_adc = filt_vel_adc;

    int16_t send_deadband_deg = cfg->angle_deadband_deg;

if(delta_adc >= cfg->speed_delta_4) {
    send_deadband_deg = 2;
}
else if(delta_adc >= cfg->speed_delta_2) {
    send_deadband_deg = 3;
}


    servo_speed = test_joint_speed_from_delta(cfg, delta_adc);
    angle_deg = pa0_map_to_index_root_angle(filt_pos_adc);

    if((last_sent_angle < 0) ||
   (test_abs_i16(angle_deg - last_sent_angle) >= send_deadband_deg))

    {
        WritePos(cfg->servo_id,
                 (uint16_t)Step_Angle_Update(angle_deg),
                 0,
                 servo_speed);

        last_sent_angle = angle_deg;

        test_log("raw=%u med=%u pos=%u vel=%u angle=%d speed=%u\r\n",
            (unsigned int)raw_adc,
            (unsigned int)raw_med_adc,
            (unsigned int)filt_pos_adc,
            (unsigned int)filt_vel_adc,
            (int)angle_deg,
            (unsigned int)servo_speed);

    }

    vTaskDelay(pdMS_TO_TICKS(cfg->loop_ms));
}

}
