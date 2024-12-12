#include <STC15F2K60S2.H>
#include "rc522.h"
#include "usart.h"
#include "stdio.h"

unsigned char UI1[20],i=0,statue;

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0xff;
	
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
