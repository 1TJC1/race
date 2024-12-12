/*

	IRQ--P3.2     ������Ҫ��һ��

*/
#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "LDChip.h"
#include "Reg_RW.h"


u8 nAsrStatus=0;
u8 nAsrRes=0;
bit flag=0;

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
	TI = 1;
} 
void delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	for (j=0;j<5;j++)
	{
		for (g=0;g<uldata;g++)
		{
			_nop_();
			_nop_();
			_nop_();
		}
	}
}

/************************************************************************
���������� 	����ASRʶ������
��ڲ�����	none
�� �� ֵ��  asrflag��1->�����ɹ��� 0��>����ʧ��
����˵����	ʶ��˳������:
						1��RunASR()����ʵ����һ��������ASR����ʶ������
						2��LD_AsrStart() ����ʵ����ASR��ʼ��
						3��LD_AsrAddFixed() ����ʵ������ӹؼ����ﵽLD3320оƬ��
						4��LD_AsrRun()	����������һ��ASR����ʶ������					
						�κ�һ��ASRʶ�����̣�����Ҫ�������˳�򣬴ӳ�ʼ����ʼ��
**************************************************************************/
u8 RunASR()
{
	u8 i=0;
	u8 asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();
		delay(10);
		if (LD_AsrAddFixed()==0)
		{
			LD_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(10);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(10);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		
		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}
	return asrflag;
}
/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void User_Modification(u8 dat)
{
	if(dat ==0)
	{
		flag=1;
		printf("����յ�\r\n");
	}
	else if(flag)
	{
		flag=0;
		switch(nAsrRes)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
		{
			case CODE_DMCS:			/*���������ԡ�*/
					printf("\"�������\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_CSWB:			/*���������ϡ�*/
					printf("\"�������\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			
			case CODE_1KL1:	 /*���������*/
					printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_1KL2:		/*����Ϻ���*/
		
					printf("\"�Ϻ�\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_1KL3:	 /*������ơ�*/
					printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_1KL4:		/*����صơ�*/				
					printf("\"�ص�\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			
			case CODE_2KL1:	 /*���....��*/
					printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_2KL2:	 /*���....��*/
					printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_2KL3:	 /*���....��*/
					printf("\"����ת\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_2KL4:	 /*���....��*/
					printf("\"����ת\"ʶ��ɹ�\r\n"); /*text.....*/
															break;
						
			case CODE_3KL1:	 /*���....��*/
					printf("\"�򿪿յ�\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_3KL2:	 /*���....��*/
					printf("\"�رտյ�\"ʶ��ɹ�\r\n"); /*text.....*/
												break;
			case CODE_5KL1:	 /*���....��*/
					printf("\"����\"ʶ��ɹ�"); /*text.....*/
												break;
	//		case CODE_3KL4:	 /*���....��*/
	//				printf("\"�������\"ʶ��ɹ�"); /*text.....*/
	//											break;
	//					
	//					case CODE_4KL1:	 /*���....��*/
	//							printf("O"); /*text.....*/
	//														break;
	//					case CODE_4KL2:	 /*���....��*/
	//							printf("P"); /*text.....*/
	//														break;
	//					case CODE_4KL3:	 /*���....��*/
	//							printf("Q"); /*text.....*/
	//														break;
	//					case CODE_4KL4:	 /*���....��*/
	//							printf("R"); /*text.....*/
	//														break;
			
			default:
				break;
		}
	}
	else 	
	{
		printf("��˵��һ������\r\n"); /*text.....*/	
	}
}

void main()
{
	GPIO_Init();
	Timer0_Init();
	Uart1Init();
	IT0 = 1;                    //����INT0���ж����� (1:���½��� 0:�����غ��½���)
  EX0 = 1;                    //ʹ��INT0�ж�
	EA = 1;
	LD_Reset();
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	SCS=0;
	SPI_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:	
					 break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}

			case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
			{
				nAsrRes = LD_GetResult();		/*��ȡ���*/												
				User_Modification(nAsrRes);
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		} 
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count1 = 0;
	if (++count1 >= 1000)
	{
		LED8 = ~LED8;
		count1 = 0;
	}
}
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				//	LD3320 �ͳ��ж��źţ�����ASR�Ͳ���MP3���жϣ���Ҫ���жϴ������зֱ���
}


