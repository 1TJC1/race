#include "uart2.h"
#include "intrins.h"

#define FOSC	11059200L          //系统频率
#define BAUD	9600             //串口波特率

#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位

#define T1MS (65536-FOSC/1000)      //1T模式
//#define T1MS (65536-FOSC/12/1000) //12T模式

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define S2_S0 0x01              //P_SW2.0

bit busy;

unsigned char flag_updata=0; //用于定时发数据，或者定时闪烁工作LED灯
unsigned int  time_updata=0;

struct UART_SYS UART_CONFIG;
/*----------------------------
UART2 中断服务程序
-----------------------------*/
void Uart2() interrupt 8 //
{
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //清除S2RI位

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
        S2CON &= ~S2TI;         //清除S2TI位
        busy = 0;               //清忙标志
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
发送串口数据
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON &= ~S2TB8;        //设置校验位为0
#elif (PARITYBIT == EVEN_PARITY)
        S2CON |= S2TB8;         //设置校验位为1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        S2CON |= S2TB8;         //设置校验位为1
#elif (PARITYBIT == EVEN_PARITY)
        S2CON &= ~S2TB8;        //设置校验位为0
#endif
    }
    busy = 1;
    S2BUF = ACC;                //写数据到UART2数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
	while (*s)                  //检测字符串结束标志
	{
		SendData(*s++);         //发送当前字符
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
	///////////////////////////串口2初始化////////////////////////////////////////////////////////
    P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
//  P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)

#if (PARITYBIT == NONE_PARITY)
    S2CON = 0x50;               //8位可变波特率
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    S2CON = 0xda;               //9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    S2CON = 0xd2;               //9位可变波特率,校验位初始为0
#endif

    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR |= 0x14;                //T2为1T模式, 并启动定时器2
    IE2 = 0x01;                 //使能串口2中断
    EA = 1;
}

void init_time0()
{
	//////////////////////定时器0初始化，自动重装载/////////////////////////////////////////////////////
		AUXR |= 0x80;                   //定时器0为1T模式
//  AUXR &= 0x7f;                   //定时器0为12T模式
    TMOD |= 0x00;                    //设置定时器为模式0(16位自动重装载)
    TL0 = T1MS;                     //初始化计时值
    TH0 = T1MS >> 8;
    TR0 = 1;                        //定时器0开始计时
    ET0 = 1;                        //使能定时器0中断
    EA = 1;
///////////////////////////////////////////////////////////////////////////////////////////////		
		
}

