/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/25 17:48
* @version: 1.0
* @description: 
********************************************************************************/


#include "i2c1.h"

// I2C ���
I2C_HandleTypeDef hi2c1;

// ��ʼ������
void i2c1_init(void) {
    hi2c1.Instance = I2C1;
    // ʱ���ٶȣ�����ģʽ 400kHz����׼ 100kHz
    hi2c1.Init.ClockSpeed = 400000;
    // ����ģʽ����ʱ��
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    // 7 λ��ַ
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    // ˫��ַȷ��
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    // ���е�ַ���
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    // ʱ��������ģʽ
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        printf("i2c1 init error !");
    }
}

// �ײ��ʼ��
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(i2cHandle->Instance==I2C1) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}

/********************************************************************************
* ����һ���ֽڵ�����
********************************************************************************/
uint8_t i2c1_transmit_byte(uint16_t address, uint8_t* data, uint8_t size) {
    if(HAL_I2C_Master_Transmit(&hi2c1, address, data, size, HAL_MAX_DELAY) != HAL_OK) {
        printf("address -> %x\r\n", address);
        return 0;
    }
    return 1;
}