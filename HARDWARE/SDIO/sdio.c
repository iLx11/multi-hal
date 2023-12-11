/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 22:35
* @version: 1.0
* @description: 
********************************************************************************/


#include "sdio.h"
#include "dma.h"
#include "string.h"

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

void sdio_sd_init(void) {

    hsd.Instance = SDIO;
    /*
        主时钟 SDIOCLK 产生 CLK 引脚时钟有效沿选择，
        可选上升沿或下降沿，
        它设定 SDIO 时钟控制寄存器(SDIO_CLKCR)的 NEGEDGE 位的值，
        一般选择设置为上升沿。
    */
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    /*
        时钟分频旁路使用，可选使能或禁用，
        它设定 SDIO_CLKCR 寄存器的 BYPASS 位。
        如果使能旁路，SDIOCLK 直接驱动 CLK 线输出时钟；
        如果禁用，使用 SDIO_CLKCR 寄存器的 CLKDIV 位值分频 SDIOCLK，然后输出到 CLK 线。
        一般选择禁用时钟分频旁路。
    */
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    /*
        节能模式选择，可选使能或禁用，
        它设定 SDIO_CLKCR 寄存器的 PWRSAV 位的值。
        如果使能节能模式，CLK 线只有在总线激活时才有时钟输出；
        如果禁用节能模式，始终使能 CLK 线输出时钟。
    */
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    /*
        四线 SD 模式
        SD手册规定初始只能是是1 bit，之后再由HAL_SD_ConfigWideBusOperation改变为4 bits,
        即必须为：
        hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    */
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    /*
        硬件流控制选择，可选使能或禁用，
        它设定 SDIO_CLKCR 寄存器的 HWFC_EN 位的值。
        硬件流控制功能可以避免 FIFO 发送上溢和下溢错误。
    */
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
    /*
        时钟分频系数，它设定 SDIO_CLKCR 寄存器的 CLKDIV 位的值，
        设置 SDIOCLK 与 CLK 线输出时钟分频系数：
        CLK 线时钟频率 = SDIOCLK / ([CLKDIV+2])。
    */
    hsd.Init.ClockDiv = 2;
    if (HAL_SD_Init(&hsd) != HAL_OK) {
        printf("sd init error !\r\n");
    }
    // 在此处重新设置四线模式
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK) {
        printf("ConfigWideBusOperation error !\r\n");
    }
}

void HAL_SD_MspInit(SD_HandleTypeDef *sdHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (sdHandle->Instance == SDIO) {
        __HAL_RCC_SDIO_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**SDIO GPIO Configuration
        PC8     ------> SDIO_D0
        PC9     ------> SDIO_D1
        PC10     ------> SDIO_D2
        PC11     ------> SDIO_D3
        PC12     ------> SDIO_CK
        PD2     ------> SDIO_CMD
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
                              | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* SDIO DMA Init */
        dma_init(sdHandle);
        /* SDIO interrupt Init */

        HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SDIO_IRQn);
    }
}

/********************************************************************************
* DMA 初始化
********************************************************************************/
void dma_init(SD_HandleTypeDef *sdHandle) {
    /* SDIO_RX Init */
    hdma_sdio_rx.Instance = DMA2_Stream3;
    hdma_sdio_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_sdio_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_rx) != HAL_OK) {
        printf("dma init error");
    }
    __HAL_LINKDMA(sdHandle, hdmarx, hdma_sdio_rx);
    /* SDIO_TX Init */
    hdma_sdio_tx.Instance = DMA2_Stream6;
    hdma_sdio_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_sdio_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio_tx) != HAL_OK) {
        printf("dma init error");
    }
    __HAL_LINKDMA(sdHandle, hdmatx, hdma_sdio_tx);

    // DMA 中断初始化
    dma_irt_init();
}

/********************************************************************************
* 获取 SD 卡状态
********************************************************************************/
HAL_StatusTypeDef get_sd_state(void) {
    if(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
        return HAL_ERROR;
    return HAL_OK;
}

/********************************************************************************
* DMA 去初始化
********************************************************************************/
void dma_deinit(SD_HandleTypeDef *sdHandle) {
    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(sdHandle->hdmarx);
    HAL_DMA_DeInit(sdHandle->hdmatx);
    HAL_NVIC_DisableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);
}

/********************************************************************************
* DMA 写入 SD 卡
 * @param   write_buff 写入缓冲数组
 * @param   start_address 写入的开始地址
 * @param   block_nums 写入块的数量
********************************************************************************/
HAL_StatusTypeDef write_to_card_dma(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) {
    // 重新初始化 DMA
    dma_deinit(&hsd);
    dma_init(&hsd);
    __disable_irq();
    // 向SD卡块写入数据
    HAL_StatusTypeDef state = HAL_SD_WriteBlocks_DMA(&hsd, write_buff, start_address, block_nums);
    if(state == HAL_OK) {
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
//        printf("\r\nWrite Block DMA Success!\r\n");
    } else {
        printf("\r\nwrite_block_dma_failed -> %d\r\n", state);
    }
    __enable_irq();
    return state;
}

/********************************************************************************
* DMA 读取 SD 卡
 * @param   read_buff 读取缓冲数组
 * @param   start_address 读取的开始地址
 * @param   block_nums 读取块的数量
********************************************************************************/
HAL_StatusTypeDef read_card_dma(uint8_t* read_buff, uint32_t start_address, uint32_t block_nums) {
    // 重新初始化 DMA
    dma_deinit(&hsd);
    dma_init(&hsd);
    __disable_irq();
    HAL_StatusTypeDef state = HAL_SD_ReadBlocks_DMA(&hsd, read_buff, start_address, block_nums);
    if(state == HAL_OK) {
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
//        printf("\r\nRead Block DMA Success!\r\n");
//        for(uint32_t i = 0; i < (block_nums * SD_BLOCK_SIZE); i++) {
//            printf("0x%02x:%02x ", i, read_buff[i]);
//        }
//        printf("\r\n");
    } else {
        printf("\r\nread_block_dma_failed -> %d\r\n", state);
    }
    return state;
}


/********************************************************************************
* 打印 SD 卡信息
********************************************************************************/
void print_card_info(void) {
    printf("\r\nMicro SD Card Test...\r\n");
    // 检测SD卡是否正常（处于数据传输模式的传输状态） */
    printf("card_state -> %d\r\n", HAL_SD_GetCardState(&hsd));
    if(HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) {
        printf(" Initialize SD card successfully!\r\n");
        // 打印SD卡基本信息
        printf(" SD card information! --------------------------------------\r\n");
        // 显示容量
        printf(" CardCapacity  : %llu \r\n", (unsigned long long) hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);
        // 块大小
        printf(" CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);
        // 逻辑块数量
        printf(" LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);
        // 逻辑块大小
        printf(" LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);
        // 卡相对地址
        printf(" RCA           : %d \r\n", hsd.SdCard.RelCardAdd);
        // 卡类型
        printf(" CardType      : %d \r\n", hsd.SdCard.CardType);
        // 读取并打印SD卡的CID信息
        HAL_SD_CardCIDTypeDef sdcard_cid;
        HAL_SD_GetCardCID(&hsd, &sdcard_cid);
        printf(" ManufacturerID: %d \r\n", sdcard_cid.ManufacturerID);
    }
}

/********************************************************************************
* 写入 SD 卡
 * @param   write_buff 写入缓冲数组
 * @param   start_address 写入的开始地址
 * @param   block_nums 写入块的数量
********************************************************************************/
HAL_StatusTypeDef write_to_card(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) {
    __disable_irq();
    // 向SD卡块写入数据
    HAL_StatusTypeDef state = HAL_SD_WriteBlocks(&hsd, write_buff, start_address, block_nums, SD_TIMEOUT);
    if(state == HAL_OK) {
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
//        printf("\r\nWrite Block Success!\r\n");
    } else {
        printf("\r\nwrite_block_failed -> %d\r\n", state);
    }
    __enable_irq();
    return state;
}

/********************************************************************************
* 读取 SD 卡
 * @param   read_buff 读取缓冲数组
 * @param   start_address 读取的开始地址
 * @param   block_nums 读取块的数量
********************************************************************************/
HAL_StatusTypeDef read_card(uint8_t* read_buff, uint32_t start_address, uint32_t block_nums) {
    HAL_StatusTypeDef state = HAL_SD_ReadBlocks(&hsd, read_buff, start_address, block_nums, SD_TIMEOUT);
    if(state == HAL_OK) {
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
//        printf("\r\nRead Block Success!\r\n");
//        for(uint32_t i = 0; i < (block_nums * SD_BLOCK_SIZE); i++) {
//            printf("0x%02x:%02x ", i, read_buff[i]);
//        }
//        printf("\r\n");
    } else {
        printf("\r\nread_block_failed -> %d\r\n", state);
    }
    return state;
}

/********************************************************************************
* 擦除块数据
 * @param   start_address 擦除的开始地址
 * @param   block_nums 擦除块的数量
********************************************************************************/
HAL_StatusTypeDef erase_card_block(uint32_t start_address, uint32_t block_nums) {
    HAL_StatusTypeDef state = HAL_SD_Erase(&hsd, start_address, block_nums);
    if(state == HAL_OK) {
        // 等待 SD 卡准备新的操作
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
        printf("\r\nErase Block Success!\r\n");
    } else {
        printf("\r\nerase_block_failed -> %d\r\n", state);
    }
    return state;
}