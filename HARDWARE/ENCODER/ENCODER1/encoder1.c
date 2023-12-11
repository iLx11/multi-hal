/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:01
* @version: 1.0
* @description: 
********************************************************************************/


#include "encoder1.h"
#include "tim4.h"

// ��ʼ״̬Ϊ̧��
uint8_t encoder_key_state = 1;

void encoder1_init(void) {
    // ��ʱ����ʼ��
    tim4_init();

    GPIO_InitTypeDef GPIO_InitStruct;
    ENABLE_ENCODER1_KEY_RCC;

    GPIO_InitStruct.Pin = ENCODER1_KEY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ENCODER1_KEY_GPIO, &GPIO_InitStruct);
}

void encoder1_scan(void) {
    // ����������ɨ��
    encoder_key_scan();
    // ��������תɨ��
    encoder_rotation_scan();
}

/********************************************************************************
* ��������תɨ��
********************************************************************************/
static void encoder_rotation_scan(void) {
    uint16_t encoder_counter = get_encoder_counter();
    if(encoder_counter % 4 == 0 && encoder_counter != 0) {
        if(encoder_counter == 4) {
            if(GET_ENCODER_KEY_STATE == ENCODE1_KEY_DOWN) {
                // ��ס�����ص�
                encoder1_hold_a_callback();
                set_encoder_counter(0);
                return;
            }
            // ��������ʱ��ص�
            encoder1_anticlockwise_callback();
        }else if(encoder_counter == 124) {
            if(GET_ENCODER_KEY_STATE == ENCODE1_KEY_DOWN) {
                // ��ס�����ص�
                encoder1_hold_c_callback();
                set_encoder_counter(0);
                return;
            }
            // ������˳ʱ��ص�
            encoder1_clockwise_callback();
        }
        set_encoder_counter(0);
    }
}

/********************************************************************************
* ����������ɨ��
********************************************************************************/
static void encoder_key_scan(void) {
    if(GET_ENCODER_KEY_STATE == ENCODE1_KEY_DOWN  && encoder_key_state == 1) {
        encoder_key_state = ENCODE1_KEY_DOWN;
        // ���������»ص�
        encoder1_key_down_callback();
    }else if(GET_ENCODER_KEY_STATE == ENCODE1_KEY_UP && encoder_key_state == 0) {
        encoder_key_state = ENCODE1_KEY_UP;
        // ������̧��ص�
        encoder1_key_up_callback();
    }
}