#include "main.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;

typedef struct UART_SYS//串口用到的结构体
{
	unsigned char ReceiveBuff[50];	//接收缓存数组
	unsigned char ReceiveCount;			//计算接收到的数据有多少字节
	unsigned char timecount;				//有多久没有接收到字节，数据断续的时间
	unsigned char timerun;					//断续时间是否开始累加
	unsigned char ReceiveComplete;	//一帧数据接收完成标志	
}; 

extern unsigned char flag_updata; //用于定时发数据，或者定时闪烁工作LED灯
extern unsigned int  time_updata;

extern struct UART_SYS UART_CONFIG;

void SendData(BYTE dat);
void SendString(char *s);
void SendDatas(unsigned char *s,unsigned char len);
void init_uart2();
void init_time0();
