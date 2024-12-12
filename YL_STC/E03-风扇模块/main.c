#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"

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

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

void key(void)
{
	static unsigned int kt=0,kb=0;
	if(P33==0)
	{
		if(++kt>10 && kb==0)
		{
			kb=1;
			kt=0;
			P13 = !P13;
			P14 = !P14;
		}
	}
	else
	{
		kt=0;
		kb=0;
	}
}

void main(void)
{
	GPIO_Init();
	P13=1;
	P14=0;
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	Timer0_Init();
	EA=1;
	while(1)
	{
		if(P13==1 && P14==0)
		{
			LCD_ShowString(80,100,(u8 *)"fan  off ",WHITE,BLACK,16,0);
		}
		else if(P13==0 && P14==1)
		{
			LCD_ShowString(80,100,(u8 *)"fan  on  ",WHITE,BLACK,16,0);
		}
	}
}

void Timer0_Isr(void) interrupt 1
{
	key();
}
