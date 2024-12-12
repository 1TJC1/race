#include <STC15F2K60S2.H>
#include "intrins.h"

#define ws2812 P15

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0x00;
	
	P1M1 = 0x00;//设置P1端口模式
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//设置P2端口模式
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//设置P3端口模式
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//设置P4端口模式
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//设置P5端口模式
	P5M0 = 0x00;  
}

void Delay300us()		//@27.000MHz
{
	unsigned char i, j;

	i = 8;
	j = 222;
	do
	{
		while (--j);
	} while (--i);
}

void ws2812_1(void)
{
	ws2812=1;
	_nop_();
	_nop_();
	ws2812=0;
	_nop_();
}

void ws2812_0(void)
{
	ws2812=0;
	_nop_();
	_nop_();
	ws2812=1;
	_nop_();
}

void ws2812_end(void)
{
	ws2812=0;
	Delay300us();
}

void lcd(void)
{
	static unsigned int i=0;
	for(i=0;i<24;i++)
	{
		if(i<16)
		{
			ws2812_1();
		}
		else
		{
			ws2812_0();
		}
	}
	ws2812_end();
}

void main(void)
{
	GPIO_Init();
	while(1)
	{
		lcd();
	}
}
