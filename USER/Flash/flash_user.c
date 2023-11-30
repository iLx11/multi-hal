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
    uint16_t flash_id = read_flash_id();
    if(flash_id == 0 || flash_id == 0xFFFF)
        printf(" flash_read_id_error !\r\n");
    else
        printf("flash_id_read_done -> %d\r\n", flash_id);
}

