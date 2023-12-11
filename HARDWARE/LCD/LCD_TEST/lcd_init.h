#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stm32f4xx_hal.h"
#ifndef u8
#define u8 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif



#define LCD_WIDTH 320
#define LCD_HEIGHT 172


/********************************************************************************
* 软件 SPI 定义
********************************************************************************/


#define LCD_SPI_SOFT_SCL_GPIO GPIOB
#define LCD_SPI_SOFT_SDA_GPIO GPIOB
#define LCD_SPI_SOFT_SCL_PIN GPIO_PIN_10
#define LCD_SPI_SOFT_SDA_PIN GPIO_PIN_15

#define LCD_SPI_SOFT_SCL_RESET HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO, LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SOFT_SCL_SET HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO, LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_SET)

#define LCD_SPI_SOFT_SDA_RESET HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO, LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SOFT_SDA_SET HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO, LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_SET)



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

#define USE_HORIZONTAL 2 //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 172
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 172
#endif



//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr()  HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO,LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set()  HAL_GPIO_WritePin(LCD_SPI_SOFT_SCL_GPIO,LCD_SPI_SOFT_SCL_PIN, GPIO_PIN_SET)

#define LCD_MOSI_Clr()  HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO,LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set()  HAL_GPIO_WritePin(LCD_SPI_SOFT_SDA_GPIO,LCD_SPI_SOFT_SDA_PIN, GPIO_PIN_SET)

#define LCD_RES_Clr()   HAL_GPIO_WritePin(LCD_47_RES_GPIO,LCD_47_RES_PIN, GPIO_PIN_RESET)//RES
#define LCD_RES_Set()   HAL_GPIO_WritePin(LCD_47_RES_GPIO,LCD_47_RES_PIN, GPIO_PIN_SET)

#define LCD_DC_Clr()    HAL_GPIO_WritePin(LCD_47_DC_GPIO,LCD_47_DC_PIN, GPIO_PIN_RESET)//DC
#define LCD_DC_Set()    HAL_GPIO_WritePin(LCD_47_DC_GPIO,LCD_47_DC_PIN, GPIO_PIN_SET)
 		     
#define LCD_CS_Clr()    HAL_GPIO_WritePin(LCD_47_CS1_GPIO,LCD_47_CS1_PIN, GPIO_PIN_RESET)//CS
#define LCD_CS_Set()    HAL_GPIO_WritePin(LCD_47_CS1_GPIO,LCD_47_CS1_PIN, GPIO_PIN_SET)

#define LCD_BLK_Clr()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




