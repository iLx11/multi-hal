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
    // sdio ��ʼ��
    sdio_sd_init();
    // ��ӡ����Ϣ
    print_card_info();
    // ����
//    erase_card_block();
    // д������
//    write_to_card(data_buff, 0, 1);
    // ��ȡ����
    read_card_dma(data_buff, 0, 1);
}
