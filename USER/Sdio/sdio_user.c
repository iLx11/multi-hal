/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 23:10
* @version: 1.0
* @description: 
********************************************************************************/


#include "sdio_user.h"
#include "sdio.h"

uint8_t data_buff[512];

void sdio_init_user(void) {
    // sdio 初始化
    sdio_sd_init();
    // 打印卡信息
    print_card_info();
    // 擦除
//    erase_card_block();
    // 写入数据
//    write_to_card(data_buff, 0, 1);
    // 读取数据
    read_card_dma(data_buff, 0, 1);
}
