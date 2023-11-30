/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 14:34
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_ENCODER2_H
#define MULTI_ENCODER2_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

#define ENCODE2_KEY_DOWN 0
#define ENCODE2_KEY_UP 1

#define ENCODER2_KEY_GPIO GPIOG
#define ENCODER2_KEY_PIN GPIO_PIN_8
#define ENABLE_ENCODER2_RCC __HAL_RCC_GPIOG_CLK_ENABLE()
#define GET_ENCODER2_KEY_STATE HAL_GPIO_ReadPin(ENCODER2_KEY_GPIO, ENCODER2_KEY_PIN)

#define ENCODER2_A_GPIO GPIOG
#define ENCODER2_A_PIN GPIO_PIN_6
#define GET_ENCODER2_A_STATE HAL_GPIO_ReadPin(ENCODER2_A_GPIO, ENCODER2_A_PIN)
#define ENCODER2_B_GPIO GPIOG
#define ENCODER2_B_PIN GPIO_PIN_7
#define GET_ENCODER2_B_STATE HAL_GPIO_ReadPin(ENCODER2_B_GPIO, ENCODER2_B_PIN)

void encoder2_init(void);

void encoder2_scan(void);

__weak void encoder2_key_down_callback();

__weak void encoder2_key_up_callback();

__weak void encoder2_clockwise_callback(void);

__weak void encoder2_anticlockwise_callback(void);

__weak void encoder2_hold_c_callback(void);

__weak void encoder2_hold_a_callback(void);

#endif //MULTI_ENCODER2_H
