/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/23 20:59
* @version: 1.0
* @description: 
********************************************************************************/


#include <stdio.h>
#include "spi2.h"
#include "stm32f4xx_hal_spi.h"
// SPI���
SPI_HandleTypeDef handle_spi2;

void spi2_init(void) {
    // ���� SPI2
    handle_spi2.Instance = SPI2;
    // ����SPI����ģʽ:����Ϊ��SPI
    handle_spi2.Init.Mode = SPI_MODE_MASTER;
    // ����ȫ˫�����ǰ�˫��������ģʽ
    handle_spi2.Init.Direction = SPI_DIRECTION_2LINES;
    // ���� SPI �����ݴ�С������8λ֡�ṹ
    handle_spi2.Init.DataSize = SPI_DATASIZE_8BIT;
    // ����ͬ��ʱ�ӿ���״̬Ϊ�ߵ�ƽ
    handle_spi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    handle_spi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    // NSS �ܽ���Ӳ��������������SSI�� ������ �ڲ� NSS �ź��� SSI λ����
    handle_spi2.Init.NSS = SPI_NSS_SOFT;
    // ���岨����Ԥ��Ƶ��֮�� ������Ԥ��ƵֵΪ256
    handle_spi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    // ָ�����ݴ���� MSB ��ʼ���� LSB ��ʼ�� ���ݴ� MSB ��ʼ
    handle_spi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handle_spi2.Init.TIMode = SPI_TIMODE_DISABLE;
    handle_spi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    // CRC ֵ����Ķ���ʽ
    handle_spi2.Init.CRCPolynomial = 10;
    // ����ָ��������ʼ������ SPIX �Ĵ���d
    HAL_SPI_Init(&handle_spi2);
}

// SPI �ײ��ʼ��
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hspi->Instance == SPI2) {
        // ʱ��ʹ��
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /** SPI2 GPIO Configuration
            PB13     ------> SPI2_SCK
            PB14     ------> SPI2_MISO
            PB15     ------> SPI2_MOSI
        */
        // �ӻ���������Ϣ������ 14
        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

/********************************************************************************
* SPI ���Ͳ�����һ���ֽڵ�����
********************************************************************************/
uint8_t spi2_transmit_receive_byte(uint8_t byte_data) {
    uint8_t receive_data;
    if (HAL_SPI_TransmitReceive(&handle_spi2, &byte_data, &receive_data, 1, 1000) != HAL_OK) return 0;
    return receive_data;
}