#include "main.h"

/***********************************************************
* 名    称：  YS-XFS5051 文本合成函数
* 功    能：  发送合成文本到XFS5051芯片进行合成播放
* 入口参数：  *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（“飞音云电子”）；
**********************************************************/
void XFS_FrameInfo(unsigned char *HZdata,unsigned char len);
void YS_Sound_Set(unsigned char n);
void YS_MANSound_Set(void);

