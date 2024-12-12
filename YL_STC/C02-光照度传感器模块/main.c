#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "BH1750.h"


extern BH1750_Data_TypeDef BH1750_Data;

u8 bh1750_Collect=0;


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
	Init_BH1750_config();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	LCD_DrawLine(20,40,20,140,BROWN);		//������
	LCD_DrawLine(20,140,200,140,BROWN);
	while(1)
	{
		static u8 Lux_value_pro=0;//ǰһ�εĹ��ն���ֵ
		static u8 cnt1_value=1;
		LED1=0;
		if(bh1750_Collect)
		{
			bh1750_Collect=0;
			
			BH1750_Read_Data();//��ʪ�Ȳɼ�
			
			display_printf(40,165,WHITE,BLACK,16,0,"���նȣ�%d ",(int)BH1750_Data.Lux);
			LCD_DrawLine(20+(cnt1_value-1),140-Lux_value_pro,20+cnt1_value,140-(int)(BH1750_Data.Lux/655.35f),GREEN);//�¶ȵ�����ͼ
			Lux_value_pro=(int)(BH1750_Data.Lux/655.35f);
			
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
		bh1750_Collect=1;
	}
}

