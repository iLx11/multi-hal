/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:25
* @version: 1.0
* @description: 
********************************************************************************/


#include "encoder_user.h"
#include "encoder1.h"
#include "encoder2.h"
#include <stdio.h>

void encoder_init_user(void) {
    encoder1_init();
    encoder2_init();
}

void encoder_scan_user(void) {
    encoder1_scan();
    encoder2_scan();
}

void encoder1_key_down_callback(void) {
    printf("按下\r\n");
}

void encoder1_key_up_callback(void) {
    printf("抬起\r\n");
}

void encoder2_key_down_callback(void) {
    printf("按下\r\n");
}

void encoder2_key_up_callback(void) {
    printf("抬起\r\n");
}

void encoder2_clockwise_callback(void) {
    printf("顺时针\r\n");
}

void encoder2_anticlockwise_callback(void) {
    printf("逆时针\r\n");
}

void encoder2_hold_c_callback(void) {
    printf("按下顺时针\r\n");
}

void encoder2_hold_a_callback(void) {
    printf("按下逆时针\r\n");
}

void encoder1_clockwise_callback(void) {
    printf("顺时针\r\n");
}

void encoder1_anticlockwise_callback(void) {
    printf("逆时针\r\n");
}

void encoder1_hold_c_callback(void) {
    printf("按下顺时针\r\n");
}

void encoder1_hold_a_callback(void) {
    printf("按下逆时针\r\n");
}
