/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/26 13:55
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_FLASH_H
#define MULTI_FLASH_H

#include "stm32f4xx_hal.h"

#define MANUFACTURE_DEVICE_ID_CMD	0x90
#define READ_STATE_REGISTER_1   0x05
#define READ_STATE_REGISTER_2   0x35
#define READ_DATA_CMD	        0x03
#define WRITE_ENABLE_CMD	    0x06
#define WRITE_DISABLE_CMD	    0x04
#define SECTOR_ERASE_CMD	    0x20
#define CHIP_ERASE_CMD	        0xc7
#define PAGE_PROGRAM_CMD        0x02

// FLASH Æ¬Ñ¡Ïß
#define FLASH_SPI_CS_GPIO GPIOB
#define FLASH_SPI_CS_PIN GPIO_PIN_14

#define ENABLE_FLASH_SPI_CS HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO, FLASH_SPI_CS_PIN, GPIO_PIN_RESET)
#define DISABLE_FLASH_SPI_CS HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO, FLASH_SPI_CS_PIN, GPIO_PIN_SET)

void flash_init(void);
uint16_t read_flash_id(void);

#endif //MULTI_FLASH_H
