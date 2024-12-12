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
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0x00;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//����P2�˿�ģʽ
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//����P3�˿�ģʽ
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//����P4�˿�ģʽ
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//����P5�˿�ģʽ
	P5M0 = 0x00;  
}

void Timer0_Init(void)		//1����@11.0592MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
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
