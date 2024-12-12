#include <STC15F2K60S2.H>
#include "intrins.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
unsigned char mode=0;

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0x00;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0xff;
	            
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
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"ϵͳ������");
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
