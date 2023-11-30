/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/8/20 15:25
* @version: 1.0
* @description: 矩阵键盘逻辑代码
********************************************************************************/


#include "matrix_key_user.h"
#include <stdio.h>
#include "usbd_customhid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t media_buff[1] = {0};
uint8_t media_zero_buff[1] = {0x00};

uint8_t key_buff[8] = {0};
uint8_t key_zero_buff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void key_init_user() {
    matrix_init();
    HAL_Delay(2000);
}

void key_scan_user() {
    matrix_scan();
}
/********************************************************************************
* 键盘按下回调
********************************************************************************/
void key_down_callback(uint8_t row, uint8_t col) {
    uint8_t key_value = (col * ROW_NUM) + row;
    printf("key_value -> %d\r", key_value);
    /*if (key_value == 0) {
        key_buff[2] = 0x04;
    } else if (key_value == 4) {
        key_buff[2] = 0x05;
    }
    send_hid_buff();*/
}

/********************************************************************************
* 键盘抬起回调
********************************************************************************/
void key_up_callback(uint8_t row, uint8_t col) {
    printf("clear key\r");
}

/********************************************************************************
* 键盘按住回调
********************************************************************************/
//void key_hold_callback(uint8_t row, uint8_t col) {
//
//}

//void send_hid_buff() {
//    while (USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, key_buff, 8) != USBD_OK);
//}
//
//void send_hid_zero_buff() {
//    while (USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, key_zero_buff, 8) != USBD_OK);
//}