#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"


float distance;//定义距离变量
u16 a;



void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0xff;
	
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

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	TMOD=0x01;//定义计时器0工作方式为1
	TH0=0;//定义定时器初值
	TL0=0;
	TR0=0;
	ET0=0;//开定时器0中断
}

//void Uart1Init(void)		//9600bps@11.0592MHz
//{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器时钟12T模式
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//	TMOD &= 0x0F;		//设置定时器模式
//	TL1 = 0xE8;			//设置定时初始值
//	TH1 = 0xFF;			//设置定时初始值
//	ET1 = 0;			//禁止定时器中断
//	TR1 = 1;			//定时器1开始计时
//	TI = 1;
//} 
void Delay10us(unsigned int xus)	//@11.0592MHz
{
	unsigned char data i;
	unsigned int x;
	for(x=0;x<xus;x++)
	{
		_nop_();
		i = 25;
		while (--i);
	}
}

void main()
{
	GPIO_Init();
	Timer0_Init();
	EA = 1;
	Echo=1;
	Trig=0;
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	while(1)
	{
		Trig=1;
		Delay10us(1);
		Trig=0;
		while(!Echo);
        TR0=1;
    while(Echo);
        TR0=0;
    a=TH0*256+TL0;
       TH0=0;
       TL0=0;
    distance= a*1.7/100;
		display_printf(80,100,WHITE,BLACK,16,0,"%.1fcm",distance);
		Delay10us(10000);
	}
}

//void Timer0_Isr(void) interrupt 1
//{
//	static unsigned int count1 = 0;
//	if (++count1 >= 1000)
//	{
//		LED8 = ~LED8;
//		count1 = 0;
//	}
//}


