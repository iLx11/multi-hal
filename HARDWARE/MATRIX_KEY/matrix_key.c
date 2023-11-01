/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/8/20 10:56
* @version: 1.0
* @description: 
********************************************************************************/


#include "matrix_key.h"
#include "stm32f4xx_hal.h"

uint16_t key_state = STATE;

static void set_row_low(uint8_t row);

static uint8_t get_col_state(uint8_t col);

// 外部重写按键的回调函数
extern void key_down_callback(uint8_t row, uint8_t col);
extern void key_up_callback(uint8_t row, uint8_t col);

/********************************************************************************
* 判断键盘抬起和放下
********************************************************************************/
#define IS_KEY_UP(row, col) ((key_state >> row * ROW_NUM + col) & 0x01) == KEY_UP
#define IS_KEY_DOWN(row, col) ((key_state >> row * ROW_NUM + col) & 0x01) == KEY_DOWN

/********************************************************************************
* 设置记录键盘的上一次状态
********************************************************************************/
#define SET_KEY_UP(row, col) key_state |= (0x01 << (row * ROW_NUM + col))
#define SET_KEY_DOWN(row, col) key_state &= ~(0x01 << (row * ROW_NUM + col))

/********************************************************************************
* 初始化
********************************************************************************/
void matrix_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ROW_GPIO, &GPIO_InitStruct);

    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN | COL4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(COL_GPIO, &GPIO_InitStruct);

}

/********************************************************************************
* 键盘扫描
********************************************************************************/
void matrix_scan() {
    for(uint8_t row = 0; row < ROW_NUM; row ++) {
        set_row_low(row);
        for(uint8_t col = 0; col < COL_NUM; col ++) {
            if(get_col_state(col) == KEY_DOWN && IS_KEY_UP(row, col)) {
                // 记录当前按下的状态
                SET_KEY_DOWN(row, col);
                // 执行按下回调函数
                key_down_callback(row, col);
            }else if(get_col_state(col) == KEY_UP && IS_KEY_DOWN(row, col)) {
                // 记录当前按下的状态
                SET_KEY_UP(row, col);
                // 执行抬起回调函数
                key_up_callback(row, col);
            }
        }
    }
}

/********************************************************************************
* 设置 row
********************************************************************************/
static void set_row_low(uint8_t row) {
    if(row == 0) {SET_ROW1_LOW; SET_ROW2_HIGH; SET_ROW3_HIGH; SET_ROW4_HIGH;}
    if(row == 1) {SET_ROW1_HIGH; SET_ROW2_LOW; SET_ROW3_HIGH; SET_ROW4_HIGH;}
    if(row == 2) {SET_ROW1_HIGH; SET_ROW2_HIGH; SET_ROW3_LOW; SET_ROW4_HIGH;}
    if(row == 3) {SET_ROW1_HIGH; SET_ROW2_HIGH; SET_ROW3_HIGH; SET_ROW4_LOW;}
}

/********************************************************************************
* 获取 col 状态
********************************************************************************/
static uint8_t get_col_state(uint8_t col) {
    if(col == 0) return GET_COL1_STATE;
    if(col == 1) return GET_COL2_STATE;
    if(col == 2) return GET_COL3_STATE;
    if(col == 3) return GET_COL4_STATE;
}