/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/29 18:55
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_FATFS_USER_H
#define MULTI_FATFS_USER_H

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "ff.h"

void fatfs_init_user(void);
// 文件系统挂载
static FRESULT file_sys_mount(FATFS* fs_obj, const char* fs_path, uint8_t dev_index);
// 文件写入
static FRESULT file_sys_write(FIL* fil, void* write_buff, uint32_t num_to_write);
// 文件读取
static FRESULT file_sys_read(FIL* fil, void* read_buff, uint32_t num_to_read);

static void fatfs_func_test(void);

#endif //MULTI_FATFS_USER_H
