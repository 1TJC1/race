#include "main.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;

typedef struct UART_SYS//�����õ��Ľṹ��
{
	unsigned char ReceiveBuff[50];	//���ջ�������
	unsigned char ReceiveCount;			//������յ��������ж����ֽ�
	unsigned char timecount;				//�ж��û�н��յ��ֽڣ����ݶ�����ʱ��
	unsigned char timerun;					//����ʱ���Ƿ�ʼ�ۼ�
	unsigned char ReceiveComplete;	//һ֡���ݽ�����ɱ�־	
}; 

extern unsigned char flag_updata; //���ڶ�ʱ�����ݣ����߶�ʱ��˸����LED��
extern unsigned int  time_updata;

extern struct UART_SYS UART_CONFIG;

void SendData(BYTE dat);
void SendString(char *s);
void SendDatas(unsigned char *s,unsigned char len);
void init_uart2();
void init_time0();
