/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 14:27
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_FLASH_USER_H
#define MULTI_FLASH_USER_H

// ×î´ó FLASH SIZE
#include "ff.h"

#define MAX_FLASH_SIZE (16 * 1024 * 1024);

void flash_init_user(void);

#endif //MULTI_FLASH_USER_H
