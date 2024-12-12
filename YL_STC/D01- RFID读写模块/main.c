#include <STC15F2K60S2.H>
#include "rc522.h"
#include "usart.h"
#include "stdio.h"

unsigned char UI1[20],i=0,statue;

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0xff;
	
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

void main(void)
{
	GPIO_Init();
	UartInit();
	EA = 1;
	TI = 1;
	RC522_Init();
	
	while(1)
	{
			RC522_Handel();
	}
}
