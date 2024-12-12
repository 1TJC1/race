#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"

sbit PWM=P1^5;

unsigned int c=0;
unsigned char jd[10];

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

void Timer0_Init(void)		//500微秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x33;				//设置定时初始值
	TH0 = 0xFE;				//设置定时初始值
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
			if(c++>=4)
			{
				c=0;
			}
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
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	Timer0_Init();
	EA = 1;
	while(1)
	{
		sprintf(jd,"angle:%d  ",c);
		LCD_ShowString(80,100,(u8 *)jd,WHITE,BLACK,16,0);
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count=0;
	key();
	if(count<=c)
	{
		PWM=1;
	}
	else
	{
		PWM=0;
	}
	count++;
	if(count>=40)
	{
		count=0;
	}
}
