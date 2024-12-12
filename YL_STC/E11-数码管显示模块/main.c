#include <STC15F2K60S2.H>
#include "intrins.h"

sbit SER  = P1^7;
sbit SCLK = P1^6;
sbit RCLK = P1^4;

unsigned char SMG_MG[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

unsigned int time=0;

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
void led_out(unsigned char num)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(num & 0x80)
		{
			SER = 1;
		}
		else
		{
			SER = 0;
		}
		num<<= 1;
		SCLK = 0;
		SCLK = 1;
	}
}

void shownum(unsigned int a,unsigned int b,unsigned int c,unsigned int d)
{
	led_out(SMG_MG[a]);
	led_out(0xfe);//1111 1110
	RCLK = 0;
	RCLK = 1;
	Delayms(1);
	led_out(SMG_MG[b]);
	led_out(0xfd);//1111 1101
	RCLK = 0;
	RCLK = 1;
	Delayms(1);
	led_out(SMG_MG[c]);
	led_out(0xfb);//1111 1011
	RCLK = 0;
	RCLK = 1;
	Delayms(1);
	led_out(SMG_MG[d]);
	led_out(0xf7);//1111 0111
	RCLK = 0;
	RCLK = 1;
	Delayms(1);
}

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

void main(void)
{
	GPIO_Init();
	Timer0_Init();
	EA = 1;
	while(1)
	{
		shownum(time,time,time,time);
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count=0;
	if(++count>=1000)
	{
		count=0;
		if(time++>=9)
		{
			time=0;
		}
	}
}
