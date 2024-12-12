#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"


float distance;//����������
u16 a;



void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0xff;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//����P2�˿�ģʽ
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//����P3�˿�ģʽ
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//����P4�˿�ģʽ
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//����P5�˿�ģʽ
	P5M0 = 0x00;  
}

void Timer0_Init(void)		//1����@11.0592MHz
{
	TMOD=0x01;//�����ʱ��0������ʽΪ1
	TH0=0;//���嶨ʱ����ֵ
	TL0=0;
	TR0=0;
	ET0=0;//����ʱ��0�ж�
}

//void Uart1Init(void)		//9600bps@11.0592MHz
//{
//	SCON = 0x50;		//8λ����,�ɱ䲨����
//	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
//	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
//	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
//	TL1 = 0xE8;			//���ö�ʱ��ʼֵ
//	TH1 = 0xFF;			//���ö�ʱ��ʼֵ
//	ET1 = 0;			//��ֹ��ʱ���ж�
//	TR1 = 1;			//��ʱ��1��ʼ��ʱ
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
	LCD_Init();//LCD��ʼ��
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


