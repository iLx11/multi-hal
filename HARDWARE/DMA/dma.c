/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/30 22:48
* @version: 1.0
* @description: 
********************************************************************************/


#include "dma.h"

void dma_irt_init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA2_Stream3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    /* DMA2_Stream6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}