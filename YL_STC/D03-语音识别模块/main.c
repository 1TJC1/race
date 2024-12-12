/*

	IRQ--P3.2     额外需要接一个

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

void Uart1Init(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE8;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
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
功能描述： 	运行ASR识别流程
入口参数：	none
返 回 值：  asrflag：1->启动成功， 0—>启动失败
其他说明：	识别顺序如下:
						1、RunASR()函数实现了一次完整的ASR语音识别流程
						2、LD_AsrStart() 函数实现了ASR初始化
						3、LD_AsrAddFixed() 函数实现了添加关键词语到LD3320芯片中
						4、LD_AsrRun()	函数启动了一次ASR语音识别流程					
						任何一次ASR识别流程，都需要按照这个顺序，从初始化开始
**************************************************************************/
u8 RunASR()
{
	u8 i=0;
	u8 asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();
		delay(10);
		if (LD_AsrAddFixed()==0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay(10);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay(10);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		
		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}
	return asrflag;
}
/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void User_Modification(u8 dat)
{
	if(dat ==0)
	{
		flag=1;
		printf("许彬收到\r\n");
	}
	else if(flag)
	{
		flag=0;
		switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
		{
			case CODE_DMCS:			/*命令“代码测试”*/
					printf("\"代码测试\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_CSWB:			/*命令“测试完毕”*/
					printf("\"测试完毕\"识别成功\r\n"); /*text.....*/
												break;
			
			case CODE_1KL1:	 /*命令“北京”*/
					printf("\"北京\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_1KL2:		/*命令“上海”*/
		
					printf("\"上海\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_1KL3:	 /*命令“开灯”*/
					printf("\"开灯\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_1KL4:		/*命令“关灯”*/				
					printf("\"关灯\"识别成功\r\n"); /*text.....*/
												break;
			
			case CODE_2KL1:	 /*命令“....”*/
					printf("\"广州\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_2KL2:	 /*命令“....”*/
					printf("\"深圳\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_2KL3:	 /*命令“....”*/
					printf("\"向左转\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_2KL4:	 /*命令“....”*/
					printf("\"向右转\"识别成功\r\n"); /*text.....*/
															break;
						
			case CODE_3KL1:	 /*命令“....”*/
					printf("\"打开空调\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_3KL2:	 /*命令“....”*/
					printf("\"关闭空调\"识别成功\r\n"); /*text.....*/
												break;
			case CODE_5KL1:	 /*命令“....”*/
					printf("\"后退\"识别成功"); /*text.....*/
												break;
	//		case CODE_3KL4:	 /*命令“....”*/
	//				printf("\"代码测试\"识别成功"); /*text.....*/
	//											break;
	//					
	//					case CODE_4KL1:	 /*命令“....”*/
	//							printf("O"); /*text.....*/
	//														break;
	//					case CODE_4KL2:	 /*命令“....”*/
	//							printf("P"); /*text.....*/
	//														break;
	//					case CODE_4KL3:	 /*命令“....”*/
	//							printf("Q"); /*text.....*/
	//														break;
	//					case CODE_4KL4:	 /*命令“....”*/
	//							printf("R"); /*text.....*/
	//														break;
			
			default:
				break;
		}
	}
	else 	
	{
		printf("请说出一级口令\r\n"); /*text.....*/	
	}
}

void main()
{
	GPIO_Init();
	Timer0_Init();
	Uart1Init();
	IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
  EX0 = 1;                    //使能INT0中断
	EA = 1;
	LD_Reset();
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	SCS=0;
	SPI_Init();
	LCD_Init();//LCD初始化
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
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}

			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{
				nAsrRes = LD_GetResult();		/*获取结果*/												
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
	ProcessInt0();				//	LD3320 送出中断信号，包括ASR和播放MP3的中断，需要在中断处理函数中分别处理
}


