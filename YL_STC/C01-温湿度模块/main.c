#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "SHT30.h"


extern SHT30_Data_TypeDef SHT30_Data;

u8 SHT30_Collect=0;


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
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
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
//	ES = 1;
//	TI = 1;
//} 

void main()
{
	GPIO_Init();
	Timer0_Init();
	EA = 1;
	SPI_Init();
	SHT30_reset();
	Init_SHT30();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	LCD_DrawLine(20,40,20,140,BROWN);		//������
	LCD_DrawLine(20,140,200,140,BROWN);
	while(1)
	{
		static u8 temp_value_pro=0;
		static u8 humi_value_pro=0;
		static u8 cnt1_value=1;
		LED1=0;
		if(SHT30_Collect)
		{
			SHT30_Collect=0;
			SHT30_Read_Data();//��ʪ�Ȳɼ�
			display_printf(80,165,GREEN,BLACK,16,0,"�¶ȣ�%d ",(int)SHT30_Data.Temperature);
			display_printf(80,205,LIGHTBLUE,BLACK,16,0,"ʪ�ȣ�%d ",(int)SHT30_Data.Humidity);
			LCD_DrawLine(20+(cnt1_value-1),140-temp_value_pro,20+cnt1_value,140-(int)SHT30_Data.Temperature,GREEN);//�¶ȵ�����ͼ
			temp_value_pro=(int)SHT30_Data.Temperature;
			LCD_DrawLine(20+(cnt1_value-1),140-humi_value_pro,20+cnt1_value,140-(int)SHT30_Data.Humidity,LIGHTBLUE);//ʪ�ȵ�����ͼ
			humi_value_pro=(int)SHT30_Data.Humidity;
			
			cnt1_value++;
			if(cnt1_value>=180)
			{
				cnt1_value=1;
				LCD_Fill( 20,40,200,140,BLACK);//��
				
				LCD_DrawLine(20,40,20,140,BROWN);		//������
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
		SHT30_Collect=1;
	}
}

