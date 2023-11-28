/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 14:27
* @version: 1.0
* @description: 
********************************************************************************/


#include <stdio.h>
#include "flash_user.h"
#include "flash_test.h"
#include "flash.h"
const uint8_t g_text_buf[] = {"STM32 SPI TEST"};
#define TEXT_SIZE   sizeof(g_text_buf)      /* TEXT×Ö·û´®³¤¶È */
void flash_init_user(void) {

   /* uint8_t datatemp[TEXT_SIZE];
    uint32_t flashsize;
    uint16_t id = 0;
    norflash_init();

    id = norflash_read_id();

    flashsize = 16 * 1024 * 1024;
    printf("id -> %d\r\n", id);

    norflash_write((uint8_t *)g_text_buf, flashsize - 100, TEXT_SIZE);
    norflash_read(datatemp, flashsize - 100, TEXT_SIZE);
    printf("datatemp -> %s\r\n",datatemp);*/

    uint32_t flash_max_size = 16 * 1024 * 1024;
    flash_init();

    uint16_t flash_id = read_flash_id();
    if(flash_id == 0 || flash_id == 0xFFFF) printf(" flash_read_id_error !\r\n");
    else {
        printf("flash_id_read_done -> %d\r\n", flash_id);
//        uint8_t test_data[4] = {"abc"};
        write_to_flash((uint8_t *) g_text_buf, flash_max_size - 100, TEXT_SIZE);
        uint8_t receive_data[TEXT_SIZE];
        read_flash(receive_data, flash_max_size - 100, TEXT_SIZE);
        printf("receive_data -> %s\r\n", receive_data);
    }
}

