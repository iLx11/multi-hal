/********************************************************************************
* @author: iLx1
* @email: colorful_ilx1@163.com
* @date: 2023/11/25 20:04
* @version: 1.0
* @description: 
********************************************************************************/


#include "oled_96.h"
#include "i2c1.h"

uint8_t oled_96_page[PAGE_96_WIDTH][PAGE_96_NUM];

#if SOFT_96_I2C
static void write_byte_to_slave_96(uint8_t data);
static void i2c_96_start_condition(void);
static void i2c_96_end_condition(void);
static void i2c_96_wait_ack(void);
static void i2c_delay() {
    uint8_t t = 10;
    while (t --);
}
#endif


/********************************************************************************
* OLED ��ʼ��
********************************************************************************/
void oled_96_init(void) {
#if SOFT_96_I2C
    GPIO_InitTypeDef GPIO_InitStruct;
    ENABLE_OLED_96_I2C_RCC;

    //GPIO��ʼ������
    GPIO_InitStruct.Pin = OLED_96_I2C_SOFT_SCL_PIN | OLED_96_I2C_SOFT_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;//����
    HAL_GPIO_Init( OLED_96_I2C_SOFT_SCL_GPIO,&GPIO_InitStruct);//��ʼ��
#else
    // Ӳ�� I2C ��ʼ��
    i2c1_init();
#endif

    // ���û�� RES ���ţ�������ʾ�д��󣬾�ע�͵�ƽ����
//    OLED_96_RES_RESET;
    HAL_Delay(200);
//    OLED_96_RES_SET;

    oled_96_wt_byte_all(0xAE,OLED_96_CMD);//--turn off oled panel
    oled_96_wt_byte_all(0x00,OLED_96_CMD);//---set low column address
    oled_96_wt_byte_all(0x10,OLED_96_CMD);//---set high column address
    oled_96_wt_byte_all(0x40,OLED_96_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_96_wt_byte_all(0x81,OLED_96_CMD);//--set contrast control register
    oled_96_wt_byte_all(0xCF,OLED_96_CMD);// Set SEG Output Current Brightness
    oled_96_wt_byte_all(0xA1,OLED_96_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    oled_96_wt_byte_all(0xC8,OLED_96_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    oled_96_wt_byte_all(0xA6,OLED_96_CMD);//--set normal display
    oled_96_wt_byte_all(0xA8,OLED_96_CMD);//--set multiplex ratio(1 to 64)
    oled_96_wt_byte_all(0x3f,OLED_96_CMD);//--1/64 duty
    oled_96_wt_byte_all(0xD3,OLED_96_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oled_96_wt_byte_all(0x00,OLED_96_CMD);//-not offset
    oled_96_wt_byte_all(0xd5,OLED_96_CMD);//--set display clock divide ratio/oscillator frequency
    oled_96_wt_byte_all(0x80,OLED_96_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    oled_96_wt_byte_all(0xD9,OLED_96_CMD);//--set pre-charge period
    oled_96_wt_byte_all(0xF1,OLED_96_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_96_wt_byte_all(0xDA,OLED_96_CMD);//--set com pins hardware configuration
    oled_96_wt_byte_all(0x12,OLED_96_CMD);
    oled_96_wt_byte_all(0xDB,OLED_96_CMD);//--set vcomh
    oled_96_wt_byte_all(0x40,OLED_96_CMD);//Set VCOM Deselect Level
    oled_96_wt_byte_all(0x20,OLED_96_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_96_wt_byte_all(0x02,OLED_96_CMD);//
    oled_96_wt_byte_all(0x8D,OLED_96_CMD);//--set Charge Pump enable/disable
    oled_96_wt_byte_all(0x14,OLED_96_CMD);//--set(0x10) disable
    oled_96_wt_byte_all(0xA4,OLED_96_CMD);// Disable Entire Display On (0xa4/0xa5)
    oled_96_wt_byte_all(0xA6,OLED_96_CMD);// Disable Inverse Display On (0xa6/a7)
    for(uint8_t i = 0; i < OLED_96_NUM; i ++) {
        oled_96_clear_index(i);
    }
    oled_96_wt_byte_all(0xAF,OLED_96_CMD);
}

/********************************************************************************
* ���� OLED ��ʾ
********************************************************************************/
void enable_oled_96_display(uint8_t index) {
    // ��ɱ�ʹ��
    oled_96_wt_byte_all(0x8D, OLED_96_CMD);
    // �����ܵ�Դ
    oled_96_wt_byte_all(0x14, OLED_96_CMD);
    // ������Ļ
    oled_96_wt_byte_all(0xAF, OLED_96_CMD);
}

/********************************************************************************
* �ر� OLED ��ʾ
********************************************************************************/
void disable_oled_96_display(uint8_t index) {
    // ��ɱ�ʹ��
    oled_96_wt_byte_all(0x8D, OLED_96_CMD);
    // �ر��ܵ�Դ
    oled_96_wt_byte_all(0x10, OLED_96_CMD);
    // �ر���Ļ
    oled_96_wt_byte_all(0xAE, OLED_96_CMD);
}


/********************************************************************************
* OLED ����Ƭѡ�������ʾ
********************************************************************************/
void oled_96_clear_index(uint8_t index) {
    for(uint8_t i = 0; i < PAGE_96_NUM; i ++){
        for(uint8_t j = 0; j < OLED_96_WIDTH; j ++){
            oled_96_page[j][i] = 0;
        }
    }
    oled_96_refresh_index(index);
}

/********************************************************************************
* �������ݵ������豸
********************************************************************************/
void oled_96_wt_byte_all(uint8_t data, uint8_t cmd) {
    for (int i = 0; i < OLED_96_NUM; i++) {
        oled_96_wt_byte(data, cmd, i);
    }
}

#if SOFT_96_I2C
/********************************************************************************
* ------ ��� I2C ------ start
********************************************************************************/
/********************************************************************************
* ���䵥�ֽ�����
********************************************************************************/
void oled_96_wt_byte(uint8_t data, uint8_t cmd, uint8_t index) {
    // ��ʼ�ź�
    i2c_96_start_condition();
    // д���ַ
    write_byte_to_slave_96(I2C_device_address_array_96[index]);
    // �ȴ���Ӧ
    i2c_96_wait_ack();
    if(cmd)
        write_byte_to_slave_96(0x40);
    else
        write_byte_to_slave_96(0x00);
    i2c_96_wait_ack();
    write_byte_to_slave_96(data);
    i2c_96_wait_ack();
    // �����ź�
    i2c_96_end_condition();
}
/********************************************************************************
* I2C д��һ���ֽ�
********************************************************************************/
static void write_byte_to_slave_96(uint8_t data) {
    for(uint8_t i = 0; i < 8; i ++) {
        if(data & 0x80)
            OLED_96_I2C_SOFT_SDA_SET;
        else
            OLED_96_I2C_SOFT_SDA_RESET;
        i2c_delay();
        OLED_96_I2C_SOFT_SCL_SET;
        i2c_delay();
        OLED_96_I2C_SOFT_SCL_RESET;
        data <<= 1;
    }
}

/********************************************************************************
* I2C ��ʼ�ź�
********************************************************************************/
static void i2c_96_start_condition(void) {
    OLED_96_I2C_SOFT_SDA_SET;
    OLED_96_I2C_SOFT_SCL_SET;
    i2c_delay();
    OLED_96_I2C_SOFT_SDA_RESET;
    i2c_delay();
    OLED_96_I2C_SOFT_SCL_RESET;
    i2c_delay();
}

/********************************************************************************
* I2C �����ź�
********************************************************************************/
static void i2c_96_end_condition(void) {
    OLED_96_I2C_SOFT_SDA_RESET;
    OLED_96_I2C_SOFT_SCL_SET;
    i2c_delay();
    OLED_96_I2C_SOFT_SDA_SET;
}

/********************************************************************************
* I2C �ȴ� ACK ��Ӧ
********************************************************************************/
static void i2c_96_wait_ack(void) {
    OLED_96_I2C_SOFT_SDA_SET;
    i2c_delay();
    OLED_96_I2C_SOFT_SCL_SET;
    i2c_delay();
    OLED_96_I2C_SOFT_SCL_RESET;
    i2c_delay();
}


/********************************************************************************
* OLED ����Ƭѡ�߸�����ʾ
********************************************************************************/
void oled_96_refresh_index(uint8_t index) {
    for(uint8_t i = 0; i < PAGE_96_NUM; i ++) {
        oled_96_wt_byte(0xb0 + i, OLED_96_CMD, index);
        oled_96_wt_byte(0x00, OLED_96_CMD, index);
        oled_96_wt_byte(0x10, OLED_96_CMD, index);
        i2c_96_start_condition();
        write_byte_to_slave_96(I2C_device_address_array_96[index]);
        i2c_96_wait_ack();
        write_byte_to_slave_96(0x40);
        i2c_96_wait_ack();
        for(uint8_t j = 0; j < OLED_96_WIDTH; j ++) {
            write_byte_to_slave_96(oled_96_page[j][i]);
            i2c_96_wait_ack();
        }
        i2c_96_end_condition();
    }
}

/********************************************************************************
* ------ ��� I2C ------ end
********************************************************************************/
#else
/********************************************************************************
* ------ Ӳ�� I2C ------ start
********************************************************************************/
/********************************************************************************
* ���䵥�ֽ�����
********************************************************************************/
void oled_96_wt_byte(uint8_t data, uint8_t cmd, uint8_t index) {
    uint8_t i2c_buffer[2];
    if(cmd)
        i2c_buffer[0] = 0x40;
    else
        i2c_buffer[0] = 0x00;
    i2c_buffer[1] = data;
    i2c1_transmit_byte(I2C_device_address_array_96[index], i2c_buffer, 2);
}
/********************************************************************************
* OLED ����Ƭѡ�߸�����ʾ
********************************************************************************/
void oled_96_refresh_index(uint8_t index) {
    uint8_t i2c_buffer[129];
    i2c_buffer[0] = 0x40;
    for(uint8_t i = 0; i < PAGE_96_NUM; i ++) {
        oled_96_wt_byte(0xb0 + i, OLED_96_CMD, index);
        oled_96_wt_byte(0x00, OLED_96_CMD, index);
        oled_96_wt_byte(0x10, OLED_96_CMD, index);
        for(uint8_t j = 0; j < OLED_96_WIDTH; j ++) {
            i2c_buffer[j + 1] = oled_96_page[j][i];
        }
        i2c1_transmit_byte(I2C_device_address_array_96[index],i2c_buffer, sizeof(i2c_buffer));
    }
}
/********************************************************************************
* ------ Ӳ�� I2C ------ end
********************************************************************************/
#endif

/********************************************************************************
* OLED ��ʾͼƬ
********************************************************************************/
void oled_96_show_pic_index(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode, uint8_t index) {
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint8_t x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++) {
        for (i = 0; i < sizex; i++) {
            temp = BMP[j];
            j++;
            for (m = 0; m < 8; m++) {
                if (temp & 0x01)
                    oled_96_page_draw_point(x, y, mode);
                else
                    oled_96_page_draw_point(x, y, !mode);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex) {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
    oled_96_refresh_index(index);
}

/********************************************************************************
* OLED page ���黭��
********************************************************************************/
void oled_96_page_draw_point(uint8_t x, uint8_t y, uint8_t f) {
    uint8_t page = y / 8;
    uint8_t page_byte = y % 8;
    uint8_t set = (1 << page_byte);
    if(f) {
        // ��Ϊ 1 ���
        oled_96_page[x][page] |= set;
    } else {
        // ��Ϊ 0 ���
        oled_96_page[x][page] &= (~ set);
        /*// ��һ�ַ�ʽ
        oled_96_page[x][page] = ~oled_96_page[x][page];
        oled_96_page[x][page] |= set;
        oled_96_page[x][page] = ~oled_96_page[x][page];*/
    }
}
