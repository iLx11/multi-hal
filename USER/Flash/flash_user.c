/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 14:27
* @version: 1.0
* @description: 
********************************************************************************/


#include <stdio.h>
#include "flash_user.h"
#include "flash.h"

void flash_init_user(void) {
    flash_init();
    HAL_Delay(20);
    printf("flash_id -> %x\r\n", read_flash_id());
}

