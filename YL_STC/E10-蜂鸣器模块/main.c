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

void main(void)
{
	GPIO_Init();
	BEEP=0;
	Timer0_Init();
	SPI_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"ϵͳ������");
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

