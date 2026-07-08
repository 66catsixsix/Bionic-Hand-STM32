#ifndef   MY_FLEX_H
#define  MY_FLEX_H

#include <stdint.h>

typedef enum
{
    FLEX_TYPE_2_2_INCH = 0,
    FLEX_TYPE_4_5_INCH
}Flex_Type_t;


// typedef struct 
// {
    



// }




void flex_init();
void flex_task(void* arg);
uint16_t flex_read_adc(void);
void flex_calibration();
uint16_t get_flex_channel_adc(uint8_t ch_index);


#endif
