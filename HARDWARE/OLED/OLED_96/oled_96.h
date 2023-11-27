/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/25 20:04
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_OLED_96_H
#define MULTI_OLED_96_H

#include "stm32f4xx_hal.h"

// 定义软件 I2C
#define SOFT_96_I2C 0

#define OLED_96_NUM 1

#define PAGE_96_NUM 8
#define PAGE_96_WIDTH 144

#define OLED_96_WIDTH 128
#define OLED_96_HEIGHT 64

#define OLED_96_I2C_SLAVE1_ADDRESS 0x78

#if SOFT_96_I2C

#define OLED_96_I2C_SOFT_SCL_GPIO GPIOB
#define OLED_96_I2C_SOFT_SDA_GPIO GPIOB
#define OLED_96_I2C_SOFT_SCL_PIN GPIO_PIN_6
#define OLED_96_I2C_SOFT_SDA_PIN GPIO_PIN_7

#define ENABLE_OLED_96_I2C_RCC __HAL_RCC_GPIOB_CLK_ENABLE()

#define OLED_96_I2C_SOFT_SCL_RESET HAL_GPIO_WritePin(OLED_96_I2C_SOFT_SCL_GPIO, OLED_96_I2C_SOFT_SCL_PIN, GPIO_PIN_RESET)
#define OLED_96_I2C_SOFT_SCL_SET HAL_GPIO_WritePin(OLED_96_I2C_SOFT_SCL_GPIO, OLED_96_I2C_SOFT_SCL_PIN, GPIO_PIN_SET)

#define OLED_96_I2C_SOFT_SDA_RESET HAL_GPIO_WritePin(OLED_96_I2C_SOFT_SDA_GPIO, OLED_96_I2C_SOFT_SDA_PIN, GPIO_PIN_RESET)
#define OLED_96_I2C_SOFT_SDA_SET HAL_GPIO_WritePin(OLED_96_I2C_SOFT_SDA_GPIO, OLED_96_I2C_SOFT_SDA_PIN, GPIO_PIN_SET)

#endif

// RES
#define OLED_96_RES_GPIO GPIOG
#define OLED_96_RES_PIN GPIO_PIN_1

#define ENABLE_OLED_96_RCC __HAL_RCC_GPIOG_CLK_ENABLE()

// RES
// 接低电平复位
#define OLED_96_RES_RESET HAL_GPIO_WritePin(OLED_96_RES_GPIO, OLED_96_RES_PIN, GPIO_PIN_RESET)
#define OLED_96_RES_SET HAL_GPIO_WritePin(OLED_96_RES_GPIO, OLED_96_RES_PIN, GPIO_PIN_SET)

// 写命令
#define OLED_96_CMD  0
// 写数据
#define OLED_96_DATA 1


void oled_96_init(void);

void enable_oled_96_display(uint8_t index);

void disable_oled_96_display(uint8_t index);

void oled_96_refresh_index(uint8_t index);

void oled_96_clear_index(uint8_t index);

void oled_96_show_pic_index(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode, uint8_t index);

void oled_96_wt_byte(uint8_t data, uint8_t cmd, uint8_t index);

void oled_96_wt_byte_all(uint8_t data, uint8_t cmd);

void oled_96_page_draw_point(uint8_t x, uint8_t y, uint8_t f);

// I2C 硬件地址数组
static uint8_t I2C_device_address_array_96[OLED_96_NUM] = {OLED_96_I2C_SLAVE1_ADDRESS};


void oled_test();
void oled_init_96();



#endif //MULTI_OLED_96_H
