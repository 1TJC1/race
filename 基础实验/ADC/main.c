#include "stc15.h"
#include "main.h"
#include "intrins.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdio.h"


/**************使用说明*****************
*
*	模式1：长按KEY1进入板载LED灯测试
*	模式2：长按KEY2进入外部GPIO测试
*	
*
*/
float adc;
unsigned char cgqzt[20];
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

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

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
	SPI_Init();
	LCD_Init();//LCD初始化
	LCD_Clear1(BLACK);


	while(1)
	{

		adc = (float)GetADCResult(4)*5/256;
		sprintf(cgqzt,"adc:%.2fV",adc);
		LCD_ShowString(60,100,cgqzt,WHITE,BLACK,16,0);
		
	}

}																																					   