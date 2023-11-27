/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/25 17:48
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_I2C1_H
#define MULTI_I2C1_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

void i2c1_init(void);
uint8_t i2c1_transmit_byte(uint16_t address, uint8_t* data, uint8_t size);
#endif //MULTI_I2C1_H
