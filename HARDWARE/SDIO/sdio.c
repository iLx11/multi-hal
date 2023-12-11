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
        ��ʱ�� SDIOCLK ���� CLK ����ʱ����Ч��ѡ��
        ��ѡ�����ػ��½��أ�
        ���趨 SDIO ʱ�ӿ��ƼĴ���(SDIO_CLKCR)�� NEGEDGE λ��ֵ��
        һ��ѡ������Ϊ�����ء�
    */
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    /*
        ʱ�ӷ�Ƶ��·ʹ�ã���ѡʹ�ܻ���ã�
        ���趨 SDIO_CLKCR �Ĵ����� BYPASS λ��
        ���ʹ����·��SDIOCLK ֱ������ CLK �����ʱ�ӣ�
        ������ã�ʹ�� SDIO_CLKCR �Ĵ����� CLKDIV λֵ��Ƶ SDIOCLK��Ȼ������� CLK �ߡ�
        һ��ѡ�����ʱ�ӷ�Ƶ��·��
    */
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    /*
        ����ģʽѡ�񣬿�ѡʹ�ܻ���ã�
        ���趨 SDIO_CLKCR �Ĵ����� PWRSAV λ��ֵ��
        ���ʹ�ܽ���ģʽ��CLK ��ֻ�������߼���ʱ����ʱ�������
        ������ý���ģʽ��ʼ��ʹ�� CLK �����ʱ�ӡ�
    */
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    /*
        ���� SD ģʽ
        SD�ֲ�涨��ʼֻ������1 bit��֮������HAL_SD_ConfigWideBusOperation�ı�Ϊ4 bits,
        ������Ϊ��
        hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    */
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    /*
        Ӳ��������ѡ�񣬿�ѡʹ�ܻ���ã�
        ���趨 SDIO_CLKCR �Ĵ����� HWFC_EN λ��ֵ��
        Ӳ�������ƹ��ܿ��Ա��� FIFO ����������������
    */
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
    /*
        ʱ�ӷ�Ƶϵ�������趨 SDIO_CLKCR �Ĵ����� CLKDIV λ��ֵ��
        ���� SDIOCLK �� CLK �����ʱ�ӷ�Ƶϵ����
        CLK ��ʱ��Ƶ�� = SDIOCLK / ([CLKDIV+2])��
    */
    hsd.Init.ClockDiv = 2;
    if (HAL_SD_Init(&hsd) != HAL_OK) {
        printf("sd init error !\r\n");
    }
    // �ڴ˴�������������ģʽ
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
* DMA ��ʼ��
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

    // DMA �жϳ�ʼ��
    dma_irt_init();
}

/********************************************************************************
* ��ȡ SD ��״̬
********************************************************************************/
HAL_StatusTypeDef get_sd_state(void) {
    if(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
        return HAL_ERROR;
    return HAL_OK;
}

/********************************************************************************
* DMA ȥ��ʼ��
********************************************************************************/
void dma_deinit(SD_HandleTypeDef *sdHandle) {
    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(sdHandle->hdmarx);
    HAL_DMA_DeInit(sdHandle->hdmatx);
    HAL_NVIC_DisableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);
}

/********************************************************************************
* DMA д�� SD ��
 * @param   write_buff д�뻺������
 * @param   start_address д��Ŀ�ʼ��ַ
 * @param   block_nums д��������
********************************************************************************/
HAL_StatusTypeDef write_to_card_dma(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) {
    // ���³�ʼ�� DMA
    dma_deinit(&hsd);
    dma_init(&hsd);
    __disable_irq();
    // ��SD����д������
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
* DMA ��ȡ SD ��
 * @param   read_buff ��ȡ��������
 * @param   start_address ��ȡ�Ŀ�ʼ��ַ
 * @param   block_nums ��ȡ�������
********************************************************************************/
HAL_StatusTypeDef read_card_dma(uint8_t* read_buff, uint32_t start_address, uint32_t block_nums) {
    // ���³�ʼ�� DMA
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
* ��ӡ SD ����Ϣ
********************************************************************************/
void print_card_info(void) {
    printf("\r\nMicro SD Card Test...\r\n");
    // ���SD���Ƿ��������������ݴ���ģʽ�Ĵ���״̬�� */
    printf("card_state -> %d\r\n", HAL_SD_GetCardState(&hsd));
    if(HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) {
        printf(" Initialize SD card successfully!\r\n");
        // ��ӡSD��������Ϣ
        printf(" SD card information! --------------------------------------\r\n");
        // ��ʾ����
        printf(" CardCapacity  : %llu \r\n", (unsigned long long) hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);
        // ���С
        printf(" CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);
        // �߼�������
        printf(" LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);
        // �߼����С
        printf(" LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);
        // ����Ե�ַ
        printf(" RCA           : %d \r\n", hsd.SdCard.RelCardAdd);
        // ������
        printf(" CardType      : %d \r\n", hsd.SdCard.CardType);
        // ��ȡ����ӡSD����CID��Ϣ
        HAL_SD_CardCIDTypeDef sdcard_cid;
        HAL_SD_GetCardCID(&hsd, &sdcard_cid);
        printf(" ManufacturerID: %d \r\n", sdcard_cid.ManufacturerID);
    }
}

/********************************************************************************
* д�� SD ��
 * @param   write_buff д�뻺������
 * @param   start_address д��Ŀ�ʼ��ַ
 * @param   block_nums д��������
********************************************************************************/
HAL_StatusTypeDef write_to_card(uint8_t* write_buff, uint32_t start_address, uint32_t block_nums) {
    __disable_irq();
    // ��SD����д������
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
* ��ȡ SD ��
 * @param   read_buff ��ȡ��������
 * @param   start_address ��ȡ�Ŀ�ʼ��ַ
 * @param   block_nums ��ȡ�������
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
* ����������
 * @param   start_address �����Ŀ�ʼ��ַ
 * @param   block_nums �����������
********************************************************************************/
HAL_StatusTypeDef erase_card_block(uint32_t start_address, uint32_t block_nums) {
    HAL_StatusTypeDef state = HAL_SD_Erase(&hsd, start_address, block_nums);
    if(state == HAL_OK) {
        // �ȴ� SD ��׼���µĲ���
        while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {}
        printf("\r\nErase Block Success!\r\n");
    } else {
        printf("\r\nerase_block_failed -> %d\r\n", state);
    }
    return state;
}