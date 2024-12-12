#include "main.h"
#include "intrins.h"
#include "string.h"
#include "uart2.h"

sbit key =P3^3;
extern unsigned char flag_updata; //用于定时发数据，或者定时闪烁工作LED灯
extern unsigned int  time_updata;

unsigned char data_cmd[]={0xff,0x03,0x00,0x01,0xfe};

void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
		for(j=0;j<1000;j++);
}

void led()
{
	if(flag_updata==1)
	{
		flag_updata=0;
		LED1 = ~LED1;

	}
}
void uart_r()
{
	if(UART_CONFIG.ReceiveComplete==1)
	{
		UART_CONFIG.ReceiveComplete=0;
		if(memcmp(UART_CONFIG.ReceiveBuff,data_cmd,5)==0)
        {
            LED3 = ~LED3;
        }
	}
}
void main()
{
	unsigned char flag_key=0;
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0x00;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;

	init_uart2();
	init_time0();
	
	while(1)
	{
		led();
		
		if(key == 0 && flag_key==0)
		{
			delay(10);
			if(key == 0)
			{
				flag_key=1;
				
				LED4 = ~LED4;
                SendDatas(data_cmd,sizeof(data_cmd));
			}
		}
		else if(key == 1)
		{
			flag_key=0;
		}
        uart_r();
	}
}
