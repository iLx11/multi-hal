/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 22:35
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_SDIO_H
#define MULTI_SDIO_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sd.h"
#include <stdio.h>

// block 大小
#define SD_BLOCK_SIZE 512

// 超时时间
#define SD_TIMEOUT ((uint32_t)100000000)

// SD 卡所有的大小
#define SD_TOTAL_SIZE_BYTE(__Handle__)  (((uint64_t)((__Handle__)->SdCard.LogBlockNbr)*((__Handle__)->SdCard.LogBlockSize))>>0)
#define SD_TOTAL_SIZE_KB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr)*((__Handle__)->SdCard.LogBlockSize))>>10)
#define SD_TOTAL_SIZE_MB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr)*((__Handle__)->SdCard.LogBlockSize))>>20)
#define SD_TOTAL_SIZE_GB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr)*((__Handle__)>SdCard.LogBlockSize))>>30)


void sdio_sd_init(void);

void print_card_info(void);

void dma_init(SD_HandleTypeDef *sdHandle);

void dma_deinit(SD_HandleTypeDef *sdHandle);

HAL_StatusTypeDef get_sd_state(void);

HAL_StatusTypeDef write_to_card_dma(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) ;

HAL_StatusTypeDef read_card_dma(uint8_t* read_buff, uint32_t start_address, uint32_t block_nums);

HAL_StatusTypeDef write_to_card(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) ;

HAL_StatusTypeDef read_card(uint8_t* read_buff, uint32_t start_address, uint32_t block_nums);

HAL_StatusTypeDef erase_card_block(uint32_t start_address, uint32_t block_nums);

#endif //MULTI_SDIO_H
