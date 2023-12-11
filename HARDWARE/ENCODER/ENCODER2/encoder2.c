/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 14:34
* @version: 1.0
* @description: 
********************************************************************************/


#include "encoder2.h"

uint8_t encoder2_key_state = 1;

uint8_t encoder_direction = 0;

void encoder2_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    ENABLE_ENCODER2_KEY_RCC;

    GPIO_InitStruct.Pin = ENCODER2_KEY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ENCODER2_KEY_GPIO, &GPIO_InitStruct);

    ENABLE_ENCODER2_RCC;

    GPIO_InitStruct.Pin = ENCODER2_A_PIN | ENCODER2_B_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER2_A_GPIO, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(ENCODER2_A_PIN);
    HAL_GPIO_EXTI_IRQHandler(ENCODER2_B_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN) {
    if (GPIO_PIN == ENCODER2_A_PIN) {
        if(GET_ENCODER2_B_STATE == GPIO_PIN_SET) {
            encoder_direction = 1;
        }
        if(encoder_direction == 2 && GET_ENCODER2_B_STATE == GPIO_PIN_RESET) {
            encoder_direction = 0;
            if(GET_ENCODER2_KEY_STATE == ENCODE2_KEY_DOWN) {
                // 按住右旋回调
                encoder2_hold_c_callback();
                return;
            }
            encoder2_clockwise_callback();
        }
    } else if (GPIO_PIN == ENCODER2_B_PIN) {
        if(GET_ENCODER2_A_STATE == GPIO_PIN_SET) {
            encoder_direction = 2;
        }
        if(encoder_direction == 1 && GET_ENCODER2_A_STATE == GPIO_PIN_RESET) {
            encoder_direction = 0;
            if(GET_ENCODER2_KEY_STATE == ENCODE2_KEY_DOWN) {
                // 按住左旋回调
                encoder2_hold_a_callback();
                return;
            }
            encoder2_anticlockwise_callback();
        }
    }
}
/********************************************************************************
* 编码器按键扫描
********************************************************************************/
void encoder2_scan(void) {
    if(GET_ENCODER2_KEY_STATE == ENCODE2_KEY_DOWN  && encoder2_key_state == 1) {
        encoder2_key_state = ENCODE2_KEY_DOWN;
        // 编码器按下回调
        encoder2_key_down_callback();
    }else if(GET_ENCODER2_KEY_STATE == ENCODE2_KEY_UP && encoder2_key_state == 0) {
        encoder2_key_state = ENCODE2_KEY_UP;
        // 编码器抬起回调
        encoder2_key_up_callback();
    }
}