#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdio.h"

u16 dj_state=0;

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0xff;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//����P2�˿�ģʽ
	P2M0 = 0x60;
	            
	P3M1 = 0x00;//����P3�˿�ģʽ
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//����P4�˿�ģʽ
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//����P5�˿�ģʽ
	P5M0 = 0x00;  
}

void Timer0_Init(void)		//1����@11.0592MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 0;				//��ʱ��0ֹͣ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
}

void Uart1Init(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE8;			//���ö�ʱ��ʼֵ
	TH1 = 0xFF;			//���ö�ʱ��ʼֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
//	ES = 1;
	TI=1;
} 

//void SendData(u8 dat)
//{
//    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
//    while (!TI);               //�ȴ�ǰ������ݷ������
//		TI=0;
//}

//void SendString(char *s,u8 len)
//{
//	u8 i =0;
//	for(i=0;i<len;i++)
//	{
//		SendData(s[i]);
//	}
//}

void main()
{
	GPIO_Init();
	Timer0_Init();
	Uart1Init();
	EA = 1;
	SPI_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	PWMA = 0;PWMB = 0;
	while(1)
	{
		switch (dj_state)
    {
    	case 0 : display_printf(80,145,GREEN,BLACK,16,0,"��");PWMA = 0;PWMB = 1;
    		break;
    	case 1 : display_printf(80,145,GREEN,BLACK,16,0,"ͣ");PWMA = 0;PWMB = 0;
    		break;
    	case 2 : display_printf(80,145,GREEN,BLACK,16,0,"��");PWMA = 1;PWMB = 0;
    		break;
    	case 3 : display_printf(80,145,GREEN,BLACK,16,0,"ͣ");PWMA = 0;PWMB = 0;
    		break;
    	default:
    		break;
    }
		LED1=0;
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count1 = 0;
	if (++count1 >= 5000)
	{
		LED8 = ~LED8;
		count1 = 0;
		if(dj_state++>=4)
		{
			dj_state=0;
		}
	}
}



