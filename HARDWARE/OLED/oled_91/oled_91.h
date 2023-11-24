/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/23 20:42
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_OLED_91_H
#define MULTI_OLED_91_H

#include "stm32f4xx_hal.h"

// 定义软件 SPI
#define SOFT_SPI 0

#define OLED_91_NUM 1

#define PAGE_NUM 4
#define PAGE_WIDTH 144

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

#if SOFT_SPI

#define OLED_SPI_SOFT_SCL_GPIO GPIOB
#define OLED_SPI_SOFT_SDA_GPIO GPIOB
#define OLED_SPI_SOFT_SCL_PIN GPIO_PIN_13
#define OLED_SPI_SOFT_SDA_PIN GPIO_PIN_15

#define OLED_SPI_SOFT_SCL_RESET HAL_GPIO_WritePin(OLED_SPI_SOFT_SCL_GPIO, OLED_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)
#define OLED_SPI_SOFT_SCL_SET HAL_GPIO_WritePin(OLED_SPI_SOFT_SCL_GPIO, OLED_SPI_SOFT_SCL_PIN, GPIO_PIN_SET)

#define OLED_SPI_SOFT_SDA_RESET HAL_GPIO_WritePin(OLED_SPI_SOFT_SDA_GPIO, OLED_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)
#define OLED_SPI_SOFT_SDA_SET HAL_GPIO_WritePin(OLED_SPI_SOFT_SDA_GPIO, OLED_SPI_SOFT_SDA_PIN, GPIO_PIN_SET)

#endif

// RES
#define OLED_91_RES_GPIO GPIOF
#define OLED_91_RES_PIN GPIO_PIN_6

// DC
#define OLED_91_DC_GPIO GPIOF
#define OLED_91_DC_PIN GPIO_PIN_7

// CS
#define OLED_91_CS1_GPIO GPIOF
#define OLED_91_CS1_PIN GPIO_PIN_8

#define ENABLE_OLED_91_RCC __HAL_RCC_GPIOF_CLK_ENABLE()

// RES
// 接低电平复位
#define OLED_91_RES_RESET HAL_GPIO_WritePin(OLED_91_RES_GPIO, OLED_91_RES_PIN, GPIO_PIN_RESET)
#define OLED_91_RES_SET HAL_GPIO_WritePin(OLED_91_RES_GPIO, OLED_91_RES_PIN, GPIO_PIN_SET)

// DC
// 命令 -> 拉低 / 数据 -> 拉高
#define OLED_91_DC_RESET  HAL_GPIO_WritePin(OLED_91_DC_GPIO, OLED_91_DC_PIN, GPIO_PIN_RESET)
#define OLED_91_DC_SET  HAL_GPIO_WritePin(OLED_91_DC_GPIO, OLED_91_DC_PIN, GPIO_PIN_SET)

// CS
#define ENABLE_OLED_91_CS(GPIO, PIN) HAL_GPIO_WritePin(GPIO, PIN, GPIO_PIN_RESET)
#define DISABLE_OLED_91_CS(GPIO, PIN) HAL_GPIO_WritePin(GPIO, PIN, GPIO_PIN_SET)

// 写命令
#define OLED_CMD  0
// 写数据
#define OLED_DATA 1



void oled_91_init(void);

void enable_oled_display(uint8_t index);

void disable_oled_display(uint8_t index);

void oled_refresh_index(uint8_t index);

void oled_clear_index(uint8_t index);

void oled_show_pic_index(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode, uint8_t index);

void oled_wt_byte(uint8_t data, uint8_t cmd, uint8_t index);

void oled_wt_byte_all(uint8_t data, uint8_t cmd);

void oled_page_draw_point(uint8_t x, uint8_t y, uint8_t f);

typedef struct {
    GPIO_TypeDef* gpio_port;
    uint16_t gpio_pin;
} spi_device_cs;

// SPI 片选引脚数组
static spi_device_cs spi_device_cs_array[1] = {
        {OLED_91_CS1_GPIO, OLED_91_CS1_PIN}
};

#endif //MULTI_OLED_91_H
