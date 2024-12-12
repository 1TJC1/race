#include "XFS5051.h"
#include "string.h"

unsigned char Frame_Info[50]; //定义的文本长度

extern void SendDatas(unsigned char *s,unsigned char len);//串口发送指令
/***********************************************************
* 名    称：  YS-XFS5051 文本合成函数
* 功    能：  发送合成文本到XFS5051芯片进行合成播放
* 入口参数：  *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（“飞音云电子”）；
**********************************************************/
void XFS_FrameInfo(unsigned char *HZdata,unsigned char len)
{
/****************需要发送的文本**********************************/ 
//	uint8_t Frame_Info[50]; //定义的文本长度
	unsigned int  HZ_Length;  
	HZ_Length =len; 			//需要发送文本的长度
 		 
/*****************帧固定配置信息**************************************/           
	Frame_Info[0] = 0xFD ; 			//构造帧头FD
	Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
	Frame_Info[2] = HZ_Length+2; 		//构造数据区长度的低字节
	Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
	Frame_Info[4] = 0x01;       //文本编码格式：GBK 
 
/*******************发送帧信息***************************************/		  
	memcpy(Frame_Info+5, HZdata, HZ_Length);
//		 PrintCom(Frame_Info,5+HZ_Length); //发送帧配置
	SendDatas(Frame_Info,5+HZ_Length);

}

/**************芯片设置命令*********************/
unsigned char XFS_StopCom[]={0xFD,0X00,0X01,0X02};//停止合成
unsigned char XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//暂停合成
unsigned char XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//恢复合成
unsigned char XFS_ChackCom[]={0XFD,0X00,0X01,0X21};//状态查询
unsigned char XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//进入POWER DOWN 状态命令
unsigned char XFS_Sound[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x34,0x5D};
unsigned char XFS_man[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x33,0x5D };
////发出1帧，字节数 9
//-> 0xFD 0x00 0x06 0x01 0x01 0x5B 0x76 0x34 0x5D 
//发出1帧，字节数 9
//-> 0xFD 0x00 0x06 0x01 0x01 0x5B 0x76 0x33 0x5D 
/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
void YS_XFS_Set(unsigned char *Info_data,unsigned char len)
{
	unsigned char Com_Len;
	Com_Len =len;
	SendDatas(Info_data,Com_Len);
}


/**********************************************************/
void YS_Sound_Set(unsigned char n)
{
	unsigned char Com_Len;
	Com_Len =9;
	XFS_Sound[7]=n+0x30;
	SendDatas(XFS_Sound,Com_Len);
}
/**********************************************************/
void YS_MANSound_Set(void)
{
	SendDatas(XFS_man,sizeof(XFS_man));
}
