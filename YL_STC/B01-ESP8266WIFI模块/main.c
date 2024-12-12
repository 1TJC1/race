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
	u8 ReceiveBuff[100];	//接收缓存数组
	u8 ReceiveCount;			//计算接收到的数据有多少字节
	u8 timecount;				//有多久没有接收到字节，数据断续的时间
	u8 timerun;					//断续时间是否开始累加
	u8 ReceiveComplete;	//一帧数据接收完成标志	
}UART_CONFIG;

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0xff;
	
	P1M1 = 0x00;//设置P1端口模式
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//设置P2端口模式
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//设置P3端口模式
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//设置P4端口模式
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//设置P5端口模式
	P5M0 = 0x00;  
}

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

//void Uart1Init(void)		//9600bps@11.0592MHz
//{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器时钟12T模式
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//	TMOD &= 0x0F;		//设置定时器模式
//	TL1 = 0xE8;			//设置定时初始值
//	TH1 = 0xFF;			//设置定时初始值
//	ET1 = 0;			//禁止定时器中断
//	TR1 = 1;			//定时器1开始计时
//	TI = 1;
//} 
void Uart2Init(void)		//9600bps@11.0592MHz
{
	P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xFE;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 = 0x01;                 //使能串口2中断
}

void SendData(u8 dat)
{
    S2BUF = dat;                 //写数据到UART数据寄存器
    while (!(S2CON & S2TI));               //等待前面的数据发送完成
		S2CON &= ~S2TI;         //清除S2TI位
}
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
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
	LCD_Init();//LCD初始化
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
	////UART超时时间，判断一帧数据接收完成///////////////////
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
        S2CON &= ~S2RI;         //清除S2RI位
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

