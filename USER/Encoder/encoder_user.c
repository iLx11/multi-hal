/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 8:25
* @version: 1.0
* @description: 
********************************************************************************/


#include "encoder_user.h"
#include "encoder1.h"

void encoder_init_user(void) {
    encoder1_init();
}

void encoder_scan_user(void) {
    encoder1_scan();
}
