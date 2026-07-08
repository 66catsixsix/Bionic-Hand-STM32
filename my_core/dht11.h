#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

// 请确认原理图中接的引脚
#define DHT11_PORT GPIOB
#define DHT11_PIN  GPIO_PIN_1

void dht11_init(void);
uint8_t dht11_read(uint8_t *temperature, uint8_t *humidity);
uint8_t dht11_read_debug(uint8_t *temperature,
                         uint8_t *humidity,
                         uint8_t raw[5],
                         uint8_t *sum_out);

#endif
