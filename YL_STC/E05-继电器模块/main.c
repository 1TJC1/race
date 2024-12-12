#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"

unsigned int jdq=0;

unsigned char jdqsz[10];

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

void key(void)
{
	static unsigned int kt=0,kb=0;
	if(P33==0)
	{
		if(++kt>10 && kb==0)
		{
			kb=1;
			kt=0;
			P15 = !P15;
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
		if(P15 == 0)
		{
			jdq = 0;
		}
		else
		{
			jdq = 1;
		}
		sprintf(jdqsz,"state:%d",jdq);
		LCD_ShowString(80,100,(u8 *)jdqsz,WHITE,BLACK,16,0);
	}
}

void Timer0_Isr(void) interrupt 1
{
	key();
}