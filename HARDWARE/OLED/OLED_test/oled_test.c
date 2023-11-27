
#include "stdlib.h"
#include "oled_test.h"
u8 OLED_GRAM[144][4];

//反显函数
void OLED_ColorTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
    }
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

void OLED_WR_Byte(u8 data,u8 cmd)
{
    if(cmd)
        OLED_91_DC_SET;
    else
        OLED_91_DC_RESET;
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
    for(uint8_t i = 0; i < 8; i ++)
    {
        OLED_SPI_SOFT_SCL_RESET;
        if(data & 0x80)
            OLED_SPI_SOFT_SDA_SET;
        else
            OLED_SDA_Clr();
        OLED_SCL_Set();
        data<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
    /*
    if(cmd)
        OLED_91_DC_SET;
    else
        OLED_91_DC_RESET;
    // 使能片选,有多个从设备时,需要根据不同的片选线使能
//    ENABLE_OLED_91_CS(spi_device_cs_array[index].gpio_port, spi_device_cs_array[index].gpio_pin);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);
#if SOFT_SPI
    for(uint8_t i = 0; i < 8; i ++) {
        // 拉低时钟
        OLED_SPI_SOFT_SCL_RESET;
        // 判断数据
        if(data & 0x80)
            OLED_SPI_SOFT_SDA_SET;
        else
            OLED_SPI_SOFT_SDA_RESET;
        // 拉高时钟
        OLED_SPI_SOFT_SCL_SET;
        // 数据移到下一位
        data <<= 1;
    }
#else
    // 硬件 SPI
    // 发送数据
    spi2_transmit_receive_byte(data);
#endif
    // 失能片选
//    DISABLE_OLED_91_CS(spi_device_cs_array[index].gpio_port, spi_device_cs_array[index].gpio_pin);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);

    OLED_91_DC_SET;
     */
}

//开启OLED显示
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void)
{
    u8 i,n;
    for(i=0;i<4;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }

}
//清屏函数
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<4;i++)
    {
        for(n=0;n<128;n++)
        {
            OLED_GRAM[n][i]=0;//清除所有数据
        }
    }
    OLED_Refresh();//更新显示
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
        OLED_GRAM[x][i]|=n;
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    }
}

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if (delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线
    else {incy=-1;delta_y=-delta_x;}
    if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        OLED_DrawPoint(uRow,uCol,mode);//画点
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);

        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);

        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
    while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x,y,*chr,size1,mode);
        if(size1==8)x+=6;
        else x+=size1/2;
        chr++;
    }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)
    {
        result*=m;
    }
    return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
    u8 t,temp,m=0;
    if(size1==8)m=2;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
        }
        else
        {
            OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
        }
    }
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
    u8 i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
            OLED_ShowChinese(128,8,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if(t==num)
        {
            for(r=0;r<16*space;r++)      //显示间隔
            {
                for(i=1;i<144;i++)
                {
                    for(n=0;n<4;n++)
                    {
                        OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh();
            }
            t=0;
        }
        m++;
        if(m==16){m=0;}
        for(i=1;i<144;i++)   //实现左移
        {
            for(n=0;n<4;n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
    u16 j=0;
    u8 i,n,temp,m;
    u8 x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0;n<sizey;n++)
    {
        for(i=0;i<sizex;i++)
        {
            temp=BMP[j];
            j++;
            for(m=0;m<8;m++)
            {
                if(temp&0x01)OLED_DrawPoint(x,y,mode);
                else OLED_DrawPoint(x,y,!mode);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==sizex)
            {
                x=x0;
                y0=y0+8;
            }
            y=y0;
        }
    }
}
//OLED的初始化
void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //GPIO初始化设置
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉
    HAL_GPIO_Init( GPIOB,&GPIO_InitStruct);//初始化

    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;//上拉
    HAL_GPIO_Init( GPIOF,&GPIO_InitStruct);//初始化

    OLED_RES_Clr();
    HAL_Delay(200);
    OLED_RES_Set();

    OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
    OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
    OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
    OLED_WR_Byte(0x00,OLED_CMD); /*set display start line*/
    OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
    OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/
    OLED_WR_Byte(0xff,OLED_CMD); /*128*/
    OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/
    OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
    OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/
    OLED_WR_Byte(0x1F,OLED_CMD); /*duty = 1/32*/
    OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
    OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/
    OLED_WR_Byte(0x80,OLED_CMD);
    OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/
    OLED_WR_Byte(0x1f,OLED_CMD);
    OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/
    OLED_WR_Byte(0x40,OLED_CMD);
    OLED_WR_Byte(0x8d,OLED_CMD); /*set charge pump enable*/
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}

