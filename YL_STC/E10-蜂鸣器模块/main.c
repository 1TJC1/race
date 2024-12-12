#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"
#define BEEP P17
#define PWM  P16

unsigned char fmqzt[20];
unsigned int beep_zt=0,pwm_zt=0;

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

void main(void)
{
	GPIO_Init();
	BEEP=0;
	Timer0_Init();
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	EA = 1;
	LCD_Clear1(BLACK);
	LCD_ShowString(60,100,(u8 *)"BEEP     PWM  ",WHITE,BLACK,16,0);
	while(1)
	{
		
		sprintf(fmqzt,"  %d       %d",beep_zt,pwm_zt);
		LCD_ShowString(60,120,(u8 *)fmqzt,WHITE,BLACK,16,0);
		if(P33==0)
		{
			BEEP=1;
			beep_zt=1;
		}
		else
		{
			BEEP=0;
			beep_zt=0;
		}
		if(P32==0)
		{
			pwm_zt=1;
		}
		else
		{
			pwm_zt=0;
		}
		
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count=0;
	if(++count>=100 && P32==0)
	{
		PWM = !PWM;
	}
}	

