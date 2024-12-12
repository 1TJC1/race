#include "stc15.h"
#include "main.h"
#include "intrins.h"
//#include "spi.h"
//#include "lcd.h"
//#include "lcd_init.h"


/**************使用说明*****************
*
*	模式1：长按KEY1进入板载LED灯测试
*	模式2：长按KEY2进入外部GPIO测试
*	
*
*/

unsigned char STA_FLAG = 0;		  //0:空 1：板载LED  2: 外部GPIO

sbit KEY1 = P3^3;
sbit KEY2 = P3^2;

sbit PA0 = P1^1;
sbit PA1 = P1^0;
sbit PA2 = P0^7;
sbit PA3 = P0^6;
sbit PA4 = P0^5;

sbit PB0 = P0^4;
sbit PB1 = P0^3;
sbit PB2 = P0^2;
sbit PB3 = P0^1;
sbit PB4 = P0^0;

sbit PC0 = P2^5;
sbit PC1 = P2^6;
sbit PC2 = P2^7;
sbit PC3 = P4^5;
sbit PC4 = P4^6;

sbit PD0 = P2^0;
sbit PD1 = P2^1;
sbit PD2 = P2^2;
sbit PD3 = P2^3;
sbit PD4 = P2^4;

sbit PE0 = P3^5;
sbit PE1 = P3^6;
sbit PE2 = P3^7;
sbit PE3 = P4^1;
sbit PE4 = P4^2;

sbit PF0 = P3^4;
sbit PF1 = P3^4;
sbit PF2 = P3^4;
sbit PF3 = P3^1;
sbit PF4 = P3^0;

sbit PG0 = P1^3;
sbit PG1 = P1^4;
sbit PG2 = P1^5;
sbit PG3 = P1^6;
sbit PG4 = P1^7;


void Delay1ms(unsigned int ms)		//@11.0592MHz
{
	unsigned char i, j;
	unsigned int k = 0;
	
	for(k=0;k<ms;k++)
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

void GPIO_Init(void)
{
	P0M1 = 0x00; P0M0 = 0xff; 
	P1M1 = 0x00; P1M0 = 0xff;        
	P2M1 = 0x00; P2M0 = 0xff;
	P3M1 = 0x0c;P3M0 = 0xf3;  
	P4M1 = 0x00; P4M0 = 0xFF;  

	P0 = 0;
	P1 = 0;
	P2 = 0;
	P3 = 0;
	P4 = 0;

}

//void KEY_Handle(void)
//{
//	static unsigned int time_cnt = 0;
//
//	if(KEY1 == 0)	  //输入模式
//	{
//		Delay1ms(1);
//		if(KEY1 == 0)
//		{
//			LCD_ShowString(80,160,(u8 *)"01",WHITE,BLACK,16,0);
//			STA_FLAG = 1;
//		}
//	}
//	else if(KEY2 == 0)	//输出模式
//	{
//		Delay1ms(1);
//		if(KEY2 == 0)
//		{
//			LCD_ShowString(80,160,(u8 *)"02",WHITE,BLACK,16,0);
//			STA_FLAG = 2;
//		}
//	}	
//}

//void LED_Board_Handle(void)
//{
//	static unsigned char i = 0;
//
//	P0 = ~(0X01<<i);;
//	i++;
//	
//	if(i==8)
//		i=0;
//}
//void LED_Handle(void)
//{
//	static unsigned char i = 0;
//	unsigned char value = 0;
//	
//	switch(i++)
//	{
//		case 0: 
//			PA0=0;PA1=1;PA2=1;PA3=1;PA4=1;
//			PB0=0;PB1=1;PB2=1;PB3=1;PB4=1; 
//			PC0=0;PC1=1;PC2=1;PC3=1;PC4=1; 
//			PD0=0;PD1=1;PD2=1;PD3=1;PD4=1; 
//			PE0=0;PE1=1;PE2=1;PE3=1;PE4=1;
//			PF0=0;PF1=1;PF2=1;PF3=1;PF4=1; 
//			PG0=0;PG1=1;PG2=1;PG3=1;PG4=1;
//		break;
//
//		case 1: 
//			PA0=1;PA1=0;PA2=1;PA3=1;PA4=1;
//			PB0=1;PB1=0;PB2=1;PB3=1;PB4=1; 
//			PC0=1;PC1=0;PC2=1;PC3=1;PC4=1; 
//			PD0=1;PD1=0;PD2=1;PD3=1;PD4=1; 
//			PE0=1;PE1=0;PE2=1;PE3=1;PE4=1;
//			PF0=1;PF1=0;PF2=1;PF3=1;PF4=1; 
//			PG0=1;PG1=0;PG2=1;PG3=1;PG4=1;
//		break;
//
//		case 2: 
//			PA0=1;PA1=1;PA2=0;PA3=1;PA4=1;
//			PB0=1;PB1=1;PB2=0;PB3=1;PB4=1; 
//			PC0=1;PC1=1;PC2=0;PC3=1;PC4=1; 
//			PD0=1;PD1=1;PD2=0;PD3=1;PD4=1; 
//			PE0=1;PE1=1;PE2=0;PE3=1;PE4=1;
//			PF0=1;PF1=1;PF2=0;PF3=1;PF4=1; 
//			PG0=1;PG1=1;PG2=0;PG3=1;PG4=1;
//		break;
//
//		case 3: 
//			PA0=1;PA1=1;PA2=1;PA3=0;PA4=1;						  
//			PB0=1;PB1=1;PB2=1;PB3=0;PB4=1; 
//			PC0=1;PC1=1;PC2=1;PC3=0;PC4=1; 
//			PD0=1;PD1=1;PD2=1;PD3=0;PD4=1; 
//			PE0=1;PE1=1;PE2=1;PE3=0;PE4=1;
//			PF0=1;PF1=1;PF2=1;PF3=0;PF4=1;
//			PG0=1;PG1=1;PG2=1;PG3=0;PG4=1;
//		break;
//
//		case 4: 
//			PA0=1;PA1=1;PA2=1;PA3=1;PA4=0;
//			PB0=1;PB1=1;PB2=1;PB3=1;PB4=0; 
//			PC0=1;PC1=1;PC2=1;PC3=1;PC4=0; 
//			PD0=1;PD1=1;PD2=1;PD3=1;PD4=0; 
//			PE0=1;PE1=1;PE2=1;PE3=1;PE4=0;
//			PF0=1;PF1=1;PF2=1;PF3=1;PF4=0;
//			PG0=1;PG1=1;PG2=1;PG3=1;PG4=0;
//		break;
//
//		case 5: 
//			PA0=1;PA1=1;PA2=1;PA3=1;PA4=1;
//			PB0=1;PB1=1;PB2=1;PB3=1;PB4=1; 
//			PC0=1;PC1=1;PC2=1;PC3=1;PC4=1; 
//			PD0=1;PD1=1;PD2=1;PD3=1;PD4=1; 
//			PE0=1;PE1=1;PE2=1;PE3=1;PE4=1;
//			PF0=1;PF1=1;PF2=1;PF3=1;PF4=1; 
//			PG0=1;PG1=1;PG2=1;PG3=1;PG4=1;
//		break;
//
//		default:
//		break;
//	}
//	if(i==6)
//		i = 0;
//}

void main(void)
{
	GPIO_Init();
//	SPI_Init();
//	LCD_Init();//LCD初始化
//	LCD_Clear1(BLACK);
//	LCD_ShowString(40,100,(u8 *)"Board Peripheral Test",WHITE,BLACK,16,0);
//	LCD_ShowString(40,160,(u8 *)"KEY: -- ",WHITE,BLACK,16,0);

	/* 开机初始化为状态1 配置P3.2 P3.3为输入模式*/
//	STA_FLAG = 0;
//	KEY1 = 1;
//	KEY2 = 1;

	while(1)
	{
//		if(STA_FLAG == 1)
//		{
//			LED_Board_Handle();
//			PA2=0;PA3=0;PA4=0;PB0=0;PB1=0;PB2=0;PB3=0;PB4=0;
//		}
//		else if(STA_FLAG == 2)
//		{
//			LED_Handle();
//			PA2=1;PA3=1;PA4=1;PB0=1;PB1=1;PB2=1;PB3=1;PB4=1;
//		}
//		else
//			;
//
//		KEY_Handle();
//		Delay1ms(100);
		Delay1ms(1000);
		PA2=0;PA3=0;PA4=0;PB0=0;PB1=0;PB2=0;PB3=0;PB4=0;
		Delay1ms(1000);
		PA2=1;PA3=1;PA4=1;PB0=1;PB1=1;PB2=1;PB3=1;PB4=1;
	}

}