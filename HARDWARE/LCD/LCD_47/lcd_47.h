/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/23 20:42
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef MULTI_LCD_47_H
#define MULTI_LCD_47_H

#include "stm32f4xx_hal.h"

// 定义软件 SPI
#define SOFT_SPI 1

#define LCD_47_NUM 1


/********************************************************************************
* 定义 LCD 横屏或竖屏显示 0 - 横屏显示， 1 - 竖屏显示
********************************************************************************/
#define LCD_DIRECTION 0

#if LCD_DIRECTION

#define LCD_WIDTH 172
#define LCD_HEIGHT 320

#else

#define LCD_WIDTH 320
#define LCD_HEIGHT 172

#endif

/********************************************************************************
* 软件 SPI 定义
********************************************************************************/
#if SOFT_SPI

#define LCD_SPI_SOFT_SCL_GPIO GPIOB
#define LCD_SPI_SOFT_SDA_GPIO GPIOB
#define LCD_SPI_SOFT_SCL_PIN GPIO_PIN_10
#define LCD_SPI_SOFT_SDA_PIN GPIO_PIN_15

#define LCD_SPI_SOFT_SCL_RESET HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO, LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SOFT_SCL_SET HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO, LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_SET)

#define LCD_SPI_SOFT_SDA_RESET HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO, LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SOFT_SDA_SET HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO, LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_SET)

#endif

/********************************************************************************
* 引脚定义
********************************************************************************/
// RES
#define LCD_47_RES_GPIO GPIOF
#define LCD_47_RES_PIN GPIO_PIN_6

// DC
#define LCD_47_DC_GPIO GPIOF
#define LCD_47_DC_PIN GPIO_PIN_7

// CS
#define LCD_47_CS1_GPIO GPIOF
#define LCD_47_CS1_PIN GPIO_PIN_8

#define ENABLE_LCD_47_RCC __HAL_RCC_GPIOF_CLK_ENABLE()


// RES
// 接低电平复位
#define LCD_47_RES_RESET HAL_GPIO_WritePin(LCD_47_RES_GPIO, LCD_47_RES_PIN, GPIO_PIN_RESET)
#define LCD_47_RES_SET HAL_GPIO_WritePin(LCD_47_RES_GPIO, LCD_47_RES_PIN, GPIO_PIN_SET)

// DC
// 命令 -> 拉低 / 数据 -> 拉高
#define LCD_47_DC_RESET  HAL_GPIO_WritePin(LCD_47_DC_GPIO, LCD_47_DC_PIN, GPIO_PIN_RESET)
#define LCD_47_DC_SET  HAL_GPIO_WritePin(LCD_47_DC_GPIO, LCD_47_DC_PIN, GPIO_PIN_SET)

// CS
#define ENABLE_LCD_47_CS(GPIO, PIN) HAL_GPIO_WritePin(GPIO, PIN, GPIO_PIN_RESET)
#define DISABLE_LCD_47_CS(GPIO, PIN) HAL_GPIO_WritePin(GPIO, PIN, GPIO_PIN_SET)

// 写命令
#define LCD_CMD  0
// 写数据
#define LCD_DATA 1

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void lcd_47_init(void);

static void lcd_gpio_init(void);

void enable_lcd_display(uint8_t index);

void disable_lcd_display(uint8_t index);

void lcd_refresh_index(uint8_t index);

void lcd_clear_index(uint8_t index);

void lcd_show_pic_index(uint8_t x, uint8_t y, uint16_t sizex, uint16_t sizey, const uint8_t pic[], uint8_t index);

void lcd_wt_byte(uint8_t data, uint8_t cmd, uint8_t index);

static void lcd_wt_d_byte(uint16_t data, uint8_t index);

static void lcd_send_byte(uint8_t data);

void lcd_wt_byte_all(uint8_t data, uint8_t cmd);

void lcd_page_draw_point(uint8_t x, uint8_t y, uint16_t color_data, uint8_t index);

void lcd_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color, uint8_t index);


typedef struct {
    GPIO_TypeDef* gpio_port;
    uint16_t gpio_pin;
} spi_device_cs;

// SPI 片选引脚数组
static spi_device_cs spi_device_cs_array[1] = {
        {LCD_47_CS1_GPIO, LCD_47_CS1_PIN}
};

#endif //MULTI_LCD_47_H
