#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0x00;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0x08;
	            
	P2M1 = 0x00;//����P2�˿�ģʽ
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//����P3�˿�ģʽ
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//����P4�˿�ģʽ
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//����P5�˿�ģʽ
	P5M0 = 0x00;  
}

void main(void)
{
	GPIO_Init();
	P13 = 0;
	SPI_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"ϵͳ������");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	while(1)
	{
		if(P14==1 ||P15==1 ||P16==1 || P17==1)
		{
			P13 = 1;
		}
		else
		{
			P13 = 0;
		}
		if(P14==1)
		{
			LCD_ShowString(100,100,(u8 *)"TOUCH1",WHITE,BLACK,16,0);
		}
		else if(P15==1)
		{
			LCD_ShowString(100,100,(u8 *)"TOUCH2",WHITE,BLACK,16,0);
		}
		else if(P16==1)
		{
			LCD_ShowString(100,100,(u8 *)"TOUCH3",WHITE,BLACK,16,0);
		}
		else if(P17==1)
		{
			LCD_ShowString(100,100,(u8 *)"TOUCH4",WHITE,BLACK,16,0);
		}
		else
		{
			LCD_ShowString(100,100,(u8 *)"      ",WHITE,BLACK,16,0);
		}
	}
}
