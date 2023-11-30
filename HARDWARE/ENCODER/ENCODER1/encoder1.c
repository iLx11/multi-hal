/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:01
* @version: 1.0
* @description: 
********************************************************************************/


#include "encoder1.h"
#include "tim4.h"

uint8_t encoder_key_state = 0;

void encoder1_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    ENABLE_ENCODER2_KEY_RCC;

    GPIO_InitStruct.Pin = ENCODER1_KEY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ENCODER1_KEY_GPIO, &GPIO_InitStruct);
}

void encoder1_scan(void) {
    if(GET_ENCODER_KEY_STATE == GPIO_PIN_RESET  && encoder_key_state == 0) {
        printf(" ENCODER KEY IS DOWN");
        encoder_key_state = 1;
    }else if(GET_ENCODER_KEY_STATE == GPIO_PIN_SET && encoder_key_state == 1) {
        printf("ENCODER KEY IS UP");
        encoder_key_state = 0;
    }
}