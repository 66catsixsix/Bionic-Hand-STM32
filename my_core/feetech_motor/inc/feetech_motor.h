#ifndef FEETECH_H
#define FEETECH_H

#include <stdint.h>

//GlobalVar
#define SCS_POS_NOW       0x38   //motor_pos_now
#define SCS_SPEED_NOW     0x3A   //motor_speed_now
#define SCS_LOAD_NOW      0x3C   //motor_load_now
#define SCS_Voltag_NOW    0x3E   //motor_voltag_now 
#define SCS_Temp_NOW      0x3F   //motor_temp_now
#define SCS_broadcast     0xFE   //motor_broadcast

#define SCS_POS_MIN_DEFAULT          0
#define SCS_POS_MAX_DEFAULT          4095
#define SCS_ANGLE_MIN_X10_DEFAULT    0
#define SCS_ANGLE_MAX_X10_DEFAULT    3600

//function
int16_t scs_read_reg(uint8_t id,uint8_t reg,uint8_t read_len);
int16_t scs_read_reg_retry(uint8_t id, uint8_t reg, uint8_t read_len, uint8_t retry);
int16_t scs_read_pos_valid(uint8_t id);
int16_t scs_pos_ema_filter(int16_t sample);
int16_t scs_pos_to_angle_x10(int16_t pos, int16_t pos_min, int16_t pos_max, int16_t angle_min_x10, int16_t angle_max_x10);
uint8_t get_motor_id();
void change_motor_id(uint8_t new_id);


#endif
