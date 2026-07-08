#include "feetech_motor.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "SCServo.h"
#include "string.h"

//GlobalVar
extern void ftUart_Send(uint8_t *nDat, int nLen);
extern int ftUart_Read(uint8_t *nDat, int nLen);
extern UART_HandleTypeDef huart3;

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
    //uint8_t checksum = ~(tx_buf[2] + tx_buf[3] + tx_buf[4] + tx_buf[5])
  
}

//motor_id_get
uint8_t get_motor_id()
{
    char id_test[128];
    int id_get = readByte(0xFE,0x05);
    if(id_get != -1)
    {
        sprintf(id_test, "更改ID中...\n 现在id为:%d\r\n", id_get);
        HAL_UART_Transmit(&huart3,(uint8_t*)id_test,strlen(id_test),100);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    return id_get;
}

void change_motor_id(uint8_t new_id)
{
    //uint8_t old_id = 1;  
    //需要修改ID在编译上电前修改该变量
    int now_id = get_motor_id(); 
    char now_id_buf[128];

    if(new_id == now_id)
    {
        sprintf(now_id_buf,"与新设置ID相同,不进行保存\r\n");
        HAL_UART_Transmit(&huart3,(uint8_t*)now_id_buf,strlen(now_id_buf),100);
    }
    else if (now_id == 1 || now_id != new_id)
    {
        unLockEprom(now_id);
        vTaskDelay(pdMS_TO_TICKS(50));
        writeByte(now_id, 0x05, new_id);
        vTaskDelay(pdMS_TO_TICKS(50));
        LockEprom(new_id);          
        vTaskDelay(pdMS_TO_TICKS(500));            

        int ping_res = Ping(new_id); 
        if (ping_res == new_id) {
        sprintf(now_id_buf,"Change ID Success! ID is : %d\r\n",now_id);
        HAL_UART_Transmit(&huart3, now_id_buf, strlen(now_id_buf), 100);
    }   else {
        HAL_UART_Transmit(&huart3, (uint8_t*)"Change ID Failed!\r\n", 19, 100);
    }


}
}