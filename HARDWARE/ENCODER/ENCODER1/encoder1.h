/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:01
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_ENCODER1_H
#define MULTI_ENCODER1_H

#define ENCODER1_KEY_GPIO GPIOD
#define ENCODER1_KEY_PIN GPIO_PIN_11
#define ENABLE_ENCODER2_KEY_RCC __HAL_RCC_GPIOD_CLK_ENABLE()

#define GET_ENCODER_KEY_STATE HAL_GPIO_ReadPin(ENCODER1_KEY_GPIO, ENCODER1_KEY_PIN)

void encoder1_init(void);
void encoder1_scan(void);

#endif //MULTI_ENCODER1_H
