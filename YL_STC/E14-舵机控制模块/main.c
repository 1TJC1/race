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

void Timer0_Init(void)		//500΢��@11.0592MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x33;				//���ö�ʱ��ʼֵ
	TH0 = 0xFE;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
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
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"ϵͳ������");
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
