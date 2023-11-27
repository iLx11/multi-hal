/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 13:55
* @version: 1.0
* @description: 
********************************************************************************/


#include <stdio.h>
#include "flash.h"
#include "spi1.h"

/********************************************************************************
* FLASH 初始化
********************************************************************************/
void flash_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // cs 片选
    GPIO_InitStruct.Pin = FLASH_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//    DISABLE_FLASH_SPI_CS;
    // spi1 初始化
    spi1_init();
//    spi1_set_speed(1);

}

/********************************************************************************
* 读取 FLASH id
********************************************************************************/
uint16_t read_flash_id(void) {
    ENABLE_FLASH_SPI_CS;
    spi1_transmit_receive_byte(0x90);//发送读取ID命令
    spi1_transmit_receive_byte(0x00);
    spi1_transmit_receive_byte(0x00);
    spi1_transmit_receive_byte(0x00);
    return ((spi1_transmit_receive_byte(0xFF) << 8) | spi1_transmit_receive_byte(0xFF));
    DISABLE_FLASH_SPI_CS;
}