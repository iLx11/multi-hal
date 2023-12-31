/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:01
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_ENCODER1_H
#define MULTI_ENCODER1_H

#include "stm32f4xx_hal.h"

#define ENCODE1_KEY_DOWN 0
#define ENCODE1_KEY_UP 1

#define ENCODER1_KEY_GPIO GPIOD
#define ENCODER1_KEY_PIN GPIO_PIN_11
#define ENABLE_ENCODER1_KEY_RCC __HAL_RCC_GPIOD_CLK_ENABLE()

#define GET_ENCODER_KEY_STATE HAL_GPIO_ReadPin(ENCODER1_KEY_GPIO, ENCODER1_KEY_PIN)

void encoder1_init(void);
void encoder1_scan(void);

static void encoder_key_scan(void);

static void encoder_rotation_scan(void);

__weak void encoder1_key_down_callback(void);

__weak void encoder1_key_up_callback(void);

__weak void encoder1_clockwise_callback(void);

__weak void encoder1_anticlockwise_callback(void);

__weak void encoder1_hold_c_callback(void);

__weak void encoder1_hold_a_callback(void);

#endif //MULTI_ENCODER1_H
