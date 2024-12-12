#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"

unsigned char rsdzt[10];

unsigned int rsd_zt=0;

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

void main(void)
{
	GPIO_Init();
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	while(1)
	{
		if(P15==0)
		{
			rsd_zt=1;
		}
		else if(P15==1)
		{
			rsd_zt=0;
		}
		sprintf(rsdzt,"state:%d",rsd_zt);
		LCD_ShowString(80,120,(u8 *)rsdzt,WHITE,BLACK,16,0);
	}
}
