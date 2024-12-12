#include "uart2.h"
#include "intrins.h"

#define FOSC	11059200L          //ϵͳƵ��
#define BAUD	9600             //���ڲ�����

#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT NONE_PARITY   //����У��λ

#define T1MS (65536-FOSC/1000)      //1Tģʽ
//#define T1MS (65536-FOSC/12/1000) //12Tģʽ

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define S2_S0 0x01              //P_SW2.0

bit busy;

unsigned char flag_updata=0; //���ڶ�ʱ�����ݣ����߶�ʱ��˸����LED��
unsigned int  time_updata=0;

struct UART_SYS UART_CONFIG;
/*----------------------------
UART2 �жϷ������
-----------------------------*/
void Uart2() interrupt 8 //
{
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //���S2RIλ

			UART_CONFIG.ReceiveBuff[UART_CONFIG.ReceiveCount]=S2BUF;
			UART_CONFIG.ReceiveCount++;
			if(UART_CONFIG.ReceiveCount==1)
			{
				UART_CONFIG.timerun=1;
			}
			UART_CONFIG.timecount=0;
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;         //���S2TIλ
        busy = 0;               //��æ��־
    }
}
/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 //1ms
{
	time_updata++;
	if(time_updata>=2000)
	{
		time_updata=0;
		flag_updata=1;
	}
	
	if(UART_CONFIG.timerun==1)
	{
		UART_CONFIG.timecount++;
	}
	if(UART_CONFIG.timecount>=10)//10ms
	{
		UART_CONFIG.timerun=0;
		UART_CONFIG.timecount=0;
		
		UART_CONFIG.ReceiveCount=0;
		UART_CONFIG.ReceiveComplete=1;
	}
}
/*----------------------------
���ʹ�������
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON &= ~S2TB8;        //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        S2CON |= S2TB8;         //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON |= S2TB8;         //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        S2CON &= ~S2TB8;        //����У��λΪ0
#endif
    }
    busy = 1;
    S2BUF = ACC;                //д���ݵ�UART2���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
	while (*s)                  //����ַ���������־
	{
		SendData(*s++);         //���͵�ǰ�ַ�
	}
}
void SendDatas(unsigned char *s,unsigned char len)
{
	unsigned char i=0;
	for(i=0;i<len;i++)
		SendData(s[i]);
}

void init_uart2()
{
	///////////////////////////����2��ʼ��////////////////////////////////////////////////////////
    P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
//  P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)

#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x50;               //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;               //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd2;               //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif

    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    IE2 = 0x01;                 //ʹ�ܴ���2�ж�
    EA = 1;
}

void init_time0()
{
	//////////////////////��ʱ��0��ʼ�����Զ���װ��/////////////////////////////////////////////////////
		AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ
    TMOD |= 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS;                     //��ʼ����ʱֵ
    TH0 = T1MS >> 8;
    TR0 = 1;                        //��ʱ��0��ʼ��ʱ
    ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
    EA = 1;
///////////////////////////////////////////////////////////////////////////////////////////////		
		
}

