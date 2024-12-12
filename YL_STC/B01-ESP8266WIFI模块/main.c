#include "main.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"

#define S2_S0 0x01              //P_SW2.0
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1

u8 value_uart;
u8 uart2str_buf[100];
struct
{
	u8 ReceiveBuff[100];	//���ջ�������
	u8 ReceiveCount;			//������յ��������ж����ֽ�
	u8 timecount;				//�ж��û�н��յ��ֽڣ����ݶ�����ʱ��
	u8 timerun;					//����ʱ���Ƿ�ʼ�ۼ�
	u8 ReceiveComplete;	//һ֡���ݽ�����ɱ�־	
}UART_CONFIG;

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
//	TI = 1;
//} 
void Uart2Init(void)		//9600bps@11.0592MHz
{
	P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0xFE;			//���ö�ʱ��ʼֵ
	T2H = 0xFF;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2 = 0x01;                 //ʹ�ܴ���2�ж�
}

void SendData(u8 dat)
{
    S2BUF = dat;                 //д���ݵ�UART���ݼĴ���
    while (!(S2CON & S2TI));               //�ȴ�ǰ������ݷ������
		S2CON &= ~S2TI;         //���S2TIλ
}
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

u8 wifi_init()
{
	SendString("AT+RST\r\n");
	while(UART_CONFIG.ReceiveComplete!=1);
	UART_CONFIG.ReceiveComplete=0;
	sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
	Delayxms(500);
	while(!(strstr(uart2str_buf, "ready")))
	{
		sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
	}
	
	if(strstr(uart2str_buf, "ready"))
	{
		SendString("AT+CWMODE=3\r\n");
		Delayxms(500);
		while(UART_CONFIG.ReceiveComplete!=1);
		UART_CONFIG.ReceiveComplete=0;
		memset(uart2str_buf, 0, sizeof uart2str_buf);
		sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
		
		if(strstr(uart2str_buf, "OK"))
		{
			SendString("AT+CWJAP=\"YLZN7777\",\"12456789\"\r\n");
			Delayxms(4000);
			while(UART_CONFIG.ReceiveComplete!=1);
			UART_CONFIG.ReceiveComplete=0;
			memset(uart2str_buf, 0, sizeof uart2str_buf);
			sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
			
			if(strstr(uart2str_buf, "OK"))
			{
				SendString("AT+CIPSTART=\"TCP\",\"192.168.100.25\",8234\r\n");
				Delayxms(4000);
				while(UART_CONFIG.ReceiveComplete!=1);
				UART_CONFIG.ReceiveComplete=0;
				memset(uart2str_buf, 0, sizeof uart2str_buf);
				sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
				
				if(strstr(uart2str_buf, "OK"))
				{
					SendString("AT+CIPMODE=1\r\n");
					Delayxms(500);
					while(UART_CONFIG.ReceiveComplete!=1);
					UART_CONFIG.ReceiveComplete=0;
					memset(uart2str_buf, 0, sizeof uart2str_buf);
					sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
					if(strstr(uart2str_buf, "OK"))
					{
						SendString("AT+CIPSEND\r\n");
						Delayxms(500);
						while(UART_CONFIG.ReceiveComplete!=1);
						UART_CONFIG.ReceiveComplete=0;
						memset(uart2str_buf, 0, sizeof uart2str_buf);
						sprintf(uart2str_buf,"%s",UART_CONFIG.ReceiveBuff);
						if(strstr(uart2str_buf, "OK"))
						{
							return 1;
						}
						else{return 0;}
					}
					else{return 0;}
				}
				else{return 0;}
			}
			else{return 0;}
		}
		else{return 0;}
	}
	else{return 0;}
}

void main()
{
	GPIO_Init();
	Timer0_Init();
	Uart2Init();
	EA = 1;
	SPI_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Clear1(BLACK);
	ESP8266_RST=1;
	while(wifi_init()!=1);
	while(1)
	{
		LED3=0;
		SendString("HELLO WORLD\r\n");
		Delayxms(500);
	}
}

void Timer0_Isr(void) interrupt 1
{
	static unsigned int count1 = 0;
	if (++count1 >= 1000)
	{
		count1 = 0;
	}
	////UART��ʱʱ�䣬�ж�һ֡���ݽ������///////////////////
	if(UART_CONFIG.timerun==1)
	{
		UART_CONFIG.timecount++;
	}
	if(UART_CONFIG.timecount>=20)//20ms
	{
		UART_CONFIG.timerun=0;
		UART_CONFIG.timecount=0;
		
		UART_CONFIG.ReceiveCount=0;
		UART_CONFIG.ReceiveComplete=1;
	}
}
void Uart2() interrupt 8
{
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         //���S2RIλ
				value_uart = S2BUF;
				UART_CONFIG.ReceiveBuff[UART_CONFIG.ReceiveCount]=value_uart;
				UART_CONFIG.ReceiveCount++;
				if(UART_CONFIG.ReceiveCount==1)
				{
					UART_CONFIG.timerun=1;
				}
				UART_CONFIG.timecount=0;
    }
}

