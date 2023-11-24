/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/8/20 10:56
* @version: 1.0
* @description: ¾ØÕó¼üÅÌÇý¶¯´úÂë
********************************************************************************/


#ifndef MULTI_MATRIX_KEY_H
#define MULTI_MATRIX_KEY_H

#define ROW_NUM 4
#define COL_NUM 4

#define STATE 0xffff

#define ROW_GPIO GPIOF
#define COL_GPIO GPIOD
#define ENABLE_ROW_RCC __HAL_RCC_GPIOF_CLK_ENABLE()
#define ENABLE_COL_RCC __HAL_RCC_GPIOD_CLK_ENABLE()

#define ROW4_PIN GPIO_PIN_3
#define ROW3_PIN GPIO_PIN_2
#define ROW2_PIN GPIO_PIN_1
#define ROW1_PIN GPIO_PIN_0

#define COL4_PIN GPIO_PIN_10
#define COL3_PIN GPIO_PIN_9
#define COL2_PIN GPIO_PIN_8
#define COL1_PIN GPIO_PIN_14

#define SET_ROW1_HIGH HAL_GPIO_WritePin(ROW_GPIO, ROW1_PIN, GPIO_PIN_SET)
#define SET_ROW2_HIGH HAL_GPIO_WritePin(ROW_GPIO, ROW2_PIN, GPIO_PIN_SET)
#define SET_ROW3_HIGH HAL_GPIO_WritePin(ROW_GPIO, ROW3_PIN, GPIO_PIN_SET)
#define SET_ROW4_HIGH HAL_GPIO_WritePin(ROW_GPIO, ROW4_PIN, GPIO_PIN_SET)

#define SET_ROW1_LOW HAL_GPIO_WritePin(ROW_GPIO, ROW1_PIN, GPIO_PIN_RESET)
#define SET_ROW2_LOW HAL_GPIO_WritePin(ROW_GPIO, ROW2_PIN, GPIO_PIN_RESET)
#define SET_ROW3_LOW HAL_GPIO_WritePin(ROW_GPIO, ROW3_PIN, GPIO_PIN_RESET)
#define SET_ROW4_LOW HAL_GPIO_WritePin(ROW_GPIO, ROW4_PIN, GPIO_PIN_RESET)

#define GET_COL1_STATE HAL_GPIO_ReadPin(COL_GPIO, COL1_PIN)
#define GET_COL2_STATE HAL_GPIO_ReadPin(COL_GPIO, COL2_PIN)
#define GET_COL3_STATE HAL_GPIO_ReadPin(COL_GPIO, COL3_PIN)
#define GET_COL4_STATE HAL_GPIO_ReadPin(COL_GPIO, COL4_PIN)

#define KEY_DOWN 0
#define KEY_UP 1

void matrix_init();

void matrix_scan();
#endif //MULTI_MATRIX_KEY_H
