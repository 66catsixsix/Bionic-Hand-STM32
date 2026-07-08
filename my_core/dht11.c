#include "dht11.h"
#include "FreeRTOS.h"
#include "task.h"

static uint8_t dht11_bit_error = 0;

// 极速切换输入输出模式，利用位带操作或寄存器直接写入防止HAL库耗时被影响
static void dht11_pin_output(void)
{
    GPIOB->MODER &= ~(3U << (1 * 2));
    GPIOB->MODER |= (1U << (1 * 2)); // General purpose output mode
}

static void dht11_pin_input(void)
{
    GPIOB->MODER &= ~(3U << (1 * 2)); // Input mode
}

static void dht11_dwt_init(void)
{   
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->LAR = 0xC5ACCE55;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void dht11_delayus(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while((DWT->CYCCNT - start) < ticks);
}

static uint8_t dht11_checkresponse(void)
{
    uint32_t timeout = 0;
    dht11_pin_input();

    // 等待传感器拉低电平
    while((DHT11_PORT->IDR & DHT11_PIN) != 0)
    {
        dht11_delayus(1);
        if(++timeout > 250) return 1;
    }

    // 等待传感器拉低结束
    timeout = 0;
    while((DHT11_PORT->IDR & DHT11_PIN) == 0)
    {
        dht11_delayus(1);
        if(++timeout > 250) return 1;
    }

    // 等待传感器拉高结束
    timeout = 0;
    while((DHT11_PORT->IDR & DHT11_PIN) != 0)
    {
        dht11_delayus(1);
        if(++timeout > 250) return 1;
    }
    return 0;
}

static uint8_t dht11_readbit(void)
{
    uint32_t timeout = 0;
    uint8_t bit;
    
    // 等待低电平结束（大约50us）
    while((DHT11_PORT->IDR & DHT11_PIN) == 0)
    {
        dht11_delayus(1);
        if(++timeout > 250)
        {
            dht11_bit_error = 1U;
            return 0;
        }
    }

    // 高电平开始后延时采样：约 26us 为 0，约 70us 为 1
    dht11_delayus(40);
    bit = ((DHT11_PORT->IDR & DHT11_PIN) != 0) ? 1U : 0U;

    // 等待本位高电平结束，避免影响下一位
    timeout = 0;
    while((DHT11_PORT->IDR & DHT11_PIN) != 0)
    {
        dht11_delayus(1);
        if(++timeout > 250)
        {
            dht11_bit_error = 1U;
            break;
        }
    }

    return bit;
}

static uint8_t dht11_readbyte(void)
{
    uint8_t i;
    uint8_t data = 0;
    for(i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_readbit();
    }
    return data;
}

void dht11_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
    GPIO_InitStruct.Pull = GPIO_PULLUP;         
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
    
    dht11_dwt_init();
}

uint8_t dht11_read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t raw[5] = {0};
    uint8_t sum = 0;

    return dht11_read_debug(temperature, humidity, raw, &sum);
}

uint8_t dht11_read_debug(uint8_t *temperature,
                         uint8_t *humidity,
                         uint8_t raw[5],
                         uint8_t *sum_out)
{
    uint8_t rh_int, rh_dec, temp_int, temp_dec, checksum;
    uint8_t sum;

    dht11_pin_output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(22); // 保持低电平至少18ms

    taskENTER_CRITICAL(); // 释放总线后的微秒级时序不能被任务调度打断

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    dht11_delayus(30); 

    if (dht11_checkresponse() != 0)
    {
        taskEXIT_CRITICAL();
        return 1;
    }

    dht11_bit_error = 0U;

    rh_int   = dht11_readbyte();
    rh_dec   = dht11_readbyte();
    temp_int = dht11_readbyte();
    temp_dec = dht11_readbyte();
    checksum = dht11_readbyte();

    if(raw != NULL)
    {
        raw[0] = rh_int;
        raw[1] = rh_dec;
        raw[2] = temp_int;
        raw[3] = temp_dec;
        raw[4] = checksum;
    }

    taskEXIT_CRITICAL(); // 读完立马开中断

    if(dht11_bit_error != 0U)
    {
        return 3;
    }

    sum = (uint8_t)(rh_int + rh_dec + temp_int + temp_dec);
    if(sum_out != NULL)
    {
        *sum_out = sum;
    }

    if (sum != checksum)
    {
        return 2;
    }

    *humidity = rh_int;
    *temperature = temp_int;

    return 0;
}
