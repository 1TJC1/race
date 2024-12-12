#include <STC15F2K60S2.H>
#include "spi.h"

void SPI_Init()
{
	SCLK = 1; // 时钟线初始状态为高电平
	MOSI = 1; // MOSI线初始状态为高电平
	CS = 1;   // 片选线初始状态为高电平
}

void SPI_SendByte(unsigned char dat)
{
	unsigned char i;

	for (i = 0; i < 8; i++) // 8位数据
	{
		MOSI = (dat & 0x80); // 将最高位写入MOSI线
		dat <<= 1;          // 左移一位
		SCLK = 0;           // 时钟线上升沿
		SCLK = 1;           // 时钟线下降沿
	}
	MOSI = 1;
}

void SPI_SendData(unsigned char *pData, unsigned int length)
{
    unsigned int i;

    CS = 0; // 选中外设
    for (i = 0; i < length; i++)
    {
        SPI_SendByte(pData[i]); // 发送数据
    }
    CS = 1; // 取消选中外设
}