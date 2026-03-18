#include "feetech_motor.h"
#include "main.h"

//GlobalVar
extern void ftUart_Send(uint8_t *nDat, int nLen);
extern int ftUart_Read(uint8_t *nDat, int nLen);

//TODO read_function_Data

int16_t scs_read_reg(uint8_t id, uint8_t reg, uint8_t read_len)
{
    uint8_t tx_buf[8];
    tx_buf[0] = 0xFF;     //frame_head
    tx_buf[1] = 0xFF;      //frame_tail
    tx_buf[2] = id;       //motor_id 
    tx_buf[3] = 0x04;  //len
    tx_buf[4] = 0x02;      //command
    tx_buf[5] = reg;       //reg
    tx_buf[6] = read_len; //read_len
    tx_buf[7] = ~(tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5] + tx_buf[6]) & 0xFF;

    //transmit
    ftUart_Send(tx_buf,8);
    //received
    uint8_t rx_buf[8] = {0};
    uint8_t rx_len = read_len + 6;

    int ret = ftUart_Read(rx_buf,rx_len);
    if(ret <= 0)
    {
        return -1;
    }
    if(rx_buf[0] != 0xFF || rx_buf[1] != 0xFF)
    {
        return -1;
    }
    if(rx_buf[2] != id)
    {
        return -1;    //id error
    }
    if(rx_buf[4] != 0x00)
    {
        return -1;
    }

    int16_t value = 0;
    if(read_len == 1)
    {
        value = rx_buf[5];
    }
    else if(read_len == 2)
    {
        value = (int16_t)(rx_buf[5] | (rx_buf[6] << 8));

    }
    return value;
    
    //checksum
    uint8_t checksum = ~(tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5])
  
}

//LocalVar  calibration
static float g_k = 0.087912f; //
static float g_b = 0.0f;
static float g_alpha = 0.2f;
static float g_angle_f = 0.0f;

