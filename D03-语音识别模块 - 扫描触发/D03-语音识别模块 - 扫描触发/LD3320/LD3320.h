/************************************************
*语音模块 ->小熊派开发板		连接说明
*		IQ	 ->		PB15		中断脚 MUC输入
*		RST  ->		PB14		复位脚 MUC输出
*		CS	 ->		PB13		片选脚 MUC输出
*		SDI  ->		PA8	数据脚 MCU输出
*		SDO	 ->		PC9		数据脚 MUC输出
*		CLK	 ->		PC8		时钟脚 MUC输出
*************************************************/


#include "string.h"
#include "stdio.h"
#include "main.h"


///以下三个状态定义用来记录程序是在运行ASR识别还是在运行MP3播放
#define LD_MODE_IDLE        0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3         0x40

///以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE         0x00	//表示没有在作ASR识别
#define LD_ASR_RUNING       0x01	//表示LD3320正在作ASR识别中
#define LD_ASR_FOUNDOK      0x10	//表示一次识别流程结束后，有一个识别结果
#define LD_ASR_FOUNDZERO    0x11	//表示一次识别流程结束后，没有识别结果
#define LD_ASR_ERROR        0x31	//	表示一次识别流程中LD3320芯片内部出现不正确的状态

#define CLK_IN          22/* user need modify this value according to clock in */
#define LD_PLL_11       (unsigned char)((CLK_IN/2.0)-1)

#define LD_PLL_ASR_19 		(unsigned char)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

#define MIC_VOL 0x43

///识别码（客户修改处）
#define CODE_KD	1	 //开灯
#define CODE_SS	2	 //关灯
#define CODE_AJCF	3	 //按键触发
#define CODE_QM	4	 //全灭
#define CODE_JT		5  //状态

#define CS_H() P20 = 1
#define CS_L() P20 = 0

#define RST_H() P21 = 1
#define RST_L() P21 = 0

#define CLK_H() P22 = 1
#define CLK_L() P22 = 0

#define SDI_H() P24 = 1
#define SDI_L() P24 = 0

#define READ_SDO() P23

#define LED1_OFF()  LED1 = 1
#define LED1_ON()   LED1 = 0

#define LED2_OFF()  LED2 = 1
#define LED2_ON()   LED2 = 0

#define LED3_OFF()  LED3 = 1
#define LED3_ON()   LED3 = 0

#define DELAY_NOP()	_nop_()

void LD_WriteReg(unsigned char address, unsigned char dataout);		//SPI写数据
static void LD_Init_Common(void);
static void LD_Init_ASR(void);
unsigned char LD_Check_ASRBusyFlag_b2(void);
static unsigned char LD_AsrAddFixed(void);
static void LD_reset(void);
static unsigned char LD_AsrRun(void);
static unsigned char RunASR(void);
static unsigned char LD_GetResult(void);
void LD3320_main(void);
void zhongduan(void);

//extern void zhongduan();
