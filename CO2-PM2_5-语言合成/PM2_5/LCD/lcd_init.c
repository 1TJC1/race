#include "lcd_init.h"
#include "spi.h"
#include "intrins.h"

/******************************************************************************
      函数说明：ms级延时
      入口数据：x 要延时的时间
      返回值：  无
******************************************************************************/
void Delayxms(unsigned int x)	//@11.0592MHz
{
	unsigned char data i, j;
	while(--x)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{
    LCD_CS_Clr();
		SPI_SendByte(dat);
//		SPI_SendData(&dat, 1);
//    HAL_SPI_Transmit(&userSPI,&dat,1,0xffff);
//  HAL_SPI_Transmit_DMA(&userSPI,&dat,1);
    LCD_CS_Set();
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
    LCD_DC_Set();//写数据
    LCD_CS_Clr();
		SPI_SendByte(dat);
//		SPI_SendData(&dat, 1);
//    HAL_SPI_Transmit(&userSPI, &dat, 1, 0xffff);
//  HAL_SPI_Transmit_DMA(&userSPI, &dat, 1);
    LCD_CS_Set() ;

}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
    u8 a[2];

    a[0] = dat>>8;
    a[1] = (u8)dat;

    LCD_DC_Set();//写数据
    LCD_CS_Clr();
		SPI_SendByte(a[0]);
		SPI_SendByte(a[1]);
//		SPI_SendData(a, 2);
//    HAL_SPI_Transmit(&userSPI, a, 2, 0x1000);
//  HAL_SPI_Transmit_DMA(&userSPI, a, 2);
    LCD_CS_Set() ;

}

/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
    LCD_DC_Clr();//写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//写数据
}
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+80);
		LCD_WR_DATA(x2+80);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
} 
void LCD_Init(void)
{
    LCD_RES_Clr();//复位
    Delayxms(1);
    LCD_RES_Set();
    Delayxms(1);

    LCD_BLK_Set();//打开背光
		Delayxms(1);

    LCD_WR_REG(0x11);                                             //无此指令，不能正常初始化芯片，无显示
    Delayxms(12);
    LCD_WR_REG(0x36);                                             //设置内存扫描方向，0X00正常扫描，从上往下，从左往右，RGB方式
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
    else LCD_WR_DATA8(0xA0);

    LCD_WR_REG(0x3A);                                             //数据格式，65K色,565
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);                                             //帧频设置
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);                                             //GATE 设置
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);                                             //VCOM设置
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);                                             //LCM设置,默认0x2c
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);                                             //VDV&VRH SET ,默认0x01
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);                                             //VRHS SET，默认0x0b
    LCD_WR_DATA8(0x12);                                           //此处根据实际情况修正

    LCD_WR_REG(0xC4);                                             //VDV SET，默认0x20
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);                                             // FR SET, 默认0x0F
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xD0);                                             //电源控制1
    LCD_WR_DATA8(0xA4);                                           //该参数不变
    LCD_WR_DATA8(0xA1);                                           //此处根据实际情况修改

    LCD_WR_REG(0xE0);                                             //正极性GAMMA调整
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);                                              //负极性GAMMA调整
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21);                                             //反显开，默认是0X20，正常模式

    LCD_WR_REG(0x11);

    LCD_WR_REG(0x29);                                             //显示开，等待MCU数传送
}
