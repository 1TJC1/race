#include <STC15F2K60S2.H>
#include "intrins.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
unsigned char mode=0;

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0x00;
	
	P1M1 = 0x00;//设置P1端口模式
	P1M0 = 0xff;
	            
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

void Delayms(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
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


void main(void)
{
	GPIO_Init();
	Timer0_Init();
	EA=1;
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	while(1)
	{
		if(P32==0)
		{
			mode=1;
			P1 = 0xF7;
			LCD_ShowString(30,100,"Regurgitant water lamp",WHITE,BLACK,16,0);
		}
		else if(P33==0)
		{
			mode=2;
			P1 = 0x7F;
			LCD_ShowString(30,100,"   Running water lamps     ",WHITE,BLACK,16,0);
		}
		if(mode==1)
		{
			if(P1 == 0xF7)
			{
				P1 = 0x7F;
				Delayms(300);
			}
			P1 = P1 >> 1;
			P1 |= 0x80;
			Delayms(300);
		}
		else if(mode==2)
		{
			if(P1 == 0x7F)
			{
				P1 = 0xF7;
				Delayms(300);
			}
			P1 = P1 << 1;
			P1 |= 0x01;
			Delayms(300);
		}
	}
}

void Timer0_Isr(void) interrupt 1
{
	
}
