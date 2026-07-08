#ifndef MY_FREERTOS_TASK_SIM_H
#define MY_FREERTOS_TASK_SIM_H

#include <stdint.h>

typedef enum {
    HAND_APP_MODE_IDLE = 0,
    HAND_APP_MODE_FREE,
    HAND_APP_MODE_REHAB,
    HAND_APP_MODE_EMG,
    HAND_APP_MODE_CALIB
} hand_app_mode_t;

typedef enum {
    EMG_STATE_IDLE = 0,
    EMG_STATE_REST,
    EMG_STATE_ACTIVE,
    EMG_STATE_STRONG
} emg_state_t;

typedef struct {
    uint16_t avg;
    uint16_t activity;
    uint16_t baseline;
    uint16_t delta;
    uint16_t level;
    uint16_t peak;
    uint32_t elapsed_sec;
    emg_state_t state;
    uint8_t running;
} emg_metrics_t;

void EMG_GetMetrics(emg_metrics_t *out);
void HandApp_SetMode(hand_app_mode_t mode);
hand_app_mode_t HandApp_GetMode(void);
uint8_t HandApp_IsMode(hand_app_mode_t mode);

extern volatile uint8_t g_dht_temp;
extern volatile uint8_t g_dht_hum;
extern volatile uint8_t g_dht_ok;
extern volatile uint8_t g_dht_fail;

#endif
