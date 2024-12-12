#include <STC15F2K60S2.H>
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "string.h"
#include "stdio.h"
#include "intrins.h"

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

unsigned int state=0;

unsigned char cgqzt[20];

float adc;

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

void InitADC()
{
    P1ASF = 0x10;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
}

unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //返回ADC结果
}

void main(void)
{
	GPIO_Init();
	InitADC();
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);
	display_printf(60,100,WHITE,BLACK,24,0,"系统启动中");
	LCD_ShowString(0,220,(u8 *)"  Load and Initialization...",WHITE,BLACK,16,0);
	LCD_Clear1(BLACK);
	while(1)
	{
		adc = (float)GetADCResult(4)*5/256;
		if(P13==0)
		{
			state=0;
		}
		else if(P13==1)
		{
			state=1;
		}
		sprintf(cgqzt,"state:%d adc:%.2fV",state,adc);
		LCD_ShowString(60,100,cgqzt,WHITE,BLACK,16,0);
	}
}