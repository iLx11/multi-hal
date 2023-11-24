#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx_hal.h"
#include "stdlib.h"

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif
#define SOFT_SPI 1

#if SOFT_SPI

#define OLED_SPI_SOFT_SCL_GPIO GPIOB
#define OLED_SPI_SOFT_SDA_GPIO GPIOB
#define OLED_SPI_SOFT_SCL_PIN GPIO_PIN_13
#define OLED_SPI_SOFT_SDA_PIN GPIO_PIN_15

#define OLED_SPI_SOFT_SCL_RESET HAL_GPIO_WritePin(OLED_SPI_SOFT_SCL_GPIO, OLED_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)
#define OLED_SPI_SOFT_SCL_SET HAL_GPIO_WritePin(OLED_SPI_SOFT_SCL_GPIO, OLED_SPI_SOFT_SCL_PIN, GPIO_PIN_RESET)

#define OLED_SPI_SOFT_SDA_RESET HAL_GPIO_WritePin(OLED_SPI_SOFT_SDA_GPIO, OLED_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)
#define OLED_SPI_SOFT_SDA_SET HAL_GPIO_WritePin(OLED_SPI_SOFT_SDA_GPIO, OLED_SPI_SOFT_SDA_PIN, GPIO_PIN_RESET)

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

//-----------------OLED端口定义----------------

#define OLED_SCL_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_RESET)//SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, GPIO_PIN_RESET)//SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15, GPIO_PIN_SET)

#define OLED_RES_Clr() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6, GPIO_PIN_SET)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7, GPIO_PIN_RESET)//DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7, GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8, GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8, GPIO_PIN_SET)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

