#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "BH1750.h"


extern BH1750_Data_TypeDef BH1750_Data;

u8 bh1750_Collect=0;


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
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
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
//	ES = 1;
//	TI = 1;
//} 

void main()
{
	GPIO_Init();
	Timer0_Init();
	EA = 1;
	SPI_Init();
	Init_BH1750_config();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	LCD_DrawLine(20,40,20,140,BROWN);		//坐标轴
	LCD_DrawLine(20,140,200,140,BROWN);
	while(1)
	{
		static u8 Lux_value_pro=0;//前一次的光照度数值
		static u8 cnt1_value=1;
		LED1=0;
		if(bh1750_Collect)
		{
			bh1750_Collect=0;
			
			BH1750_Read_Data();//温湿度采集
			
			display_printf(40,165,WHITE,BLACK,16,0,"光照度：%d ",(int)BH1750_Data.Lux);
			LCD_DrawLine(20+(cnt1_value-1),140-Lux_value_pro,20+cnt1_value,140-(int)(BH1750_Data.Lux/655.35f),GREEN);//温度的折线图
			Lux_value_pro=(int)(BH1750_Data.Lux/655.35f);
			
			cnt1_value++;
			if(cnt1_value>=180)
			{
				cnt1_value=1;
				LCD_Fill( 20,40,200,140,BLACK);//清
				
				LCD_DrawLine(20,40,20,140,BROWN);		//坐标轴
				LCD_DrawLine(20,140,200,140,BROWN);
			}
		}
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count1 = 0;
	if (++count1 >= 100)
	{
		LED8 = ~LED8;
		count1 = 0;
		bh1750_Collect=1;
	}
}

