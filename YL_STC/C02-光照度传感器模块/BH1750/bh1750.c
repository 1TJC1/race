#include "bh1750.h"
#include "i2c.h"
#include "lcd_init.h"

u8 BUF[2];
u16 result;

BH1750_Data_TypeDef BH1750_Data;


void I2C_BH1750_Transmit(u8 BH1750_address,u8 *Data,u8 length)
{
	u8 i=0;
	I2C_Start();
	
	I2C_SendByte(BH1750_address);
	I2C_WaitAck();
	
	for(i=0;i<length;i++)
	{
		I2C_SendByte(Data[i]);
		I2C_WaitAck();
	}
	
	I2C_Stop();
}

u8 I2C_Master_Receive(u8 BH1750_address, u8 *data_buffer, u8 length) 
{
	u8 i=0;
	I2C_Start();
	
	I2C_SendByte(BH1750_address + 1); // 发送设备地址+读命令
	if(I2C_WaitAck()) 
	{
		I2C_Stop();
		return 0;
	}
	for (i = 0; i < length; i++) 
	{
		data_buffer[i] = I2C_ReceiveByte();
		if (i == length - 1) 
		{
			I2C_Stop(); // 最后一个字节后发送停止信号
		} 
		else 
		{
			I2C_SendAck(); // 发送应答信号
		}
	}
	return 1;
}

/***************************************************************
* 函数名称: Init_BH1750
* 说    明: 写命令初始化BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Init_BH1750(void)  //上电
{
	u8 t_Data = 0x01; //上电指令(0X01)
	I2C_BH1750_Transmit(BH1750_Addr,&t_Data,1);
//	HAL_I2C_Master_Transmit(&hi2c2,BH1750_Addr,&t_Data,1,0xff);
}

/***************************************************************
* 函数名称: Start_BH1750
* 说    明: 启动BH1750
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Start_BH1750(void)
{
	u8 t_Data = 0x10; //连续高分辨率测量(0x10)
	I2C_BH1750_Transmit(BH1750_Addr,&t_Data,1);
//	HAL_I2C_Master_Transmit(&hi2c2,BH1750_Addr,&t_Data,1,0xff); 
}

void Init_BH1750_config(void)
{
	Init_BH1750();
}
/***************************************************************
* 说    明: 测量光照强度
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void BH1750_Read_Data(void)
{
	Start_BH1750();
	Delayxms(200);
	I2C_Master_Receive(BH1750_Addr,BUF,2); 
//	HAL_I2C_Master_Receive(&hi2c2, BH1750_Addr+1,BUF,2,0xff); 
	result=BUF[0];
	result=(result<<8)+BUF[1];  //合成数据，即光照数据	
	BH1750_Data.Lux=(float)(result/1.2); //光照强度=(寄存器值[15:0]*分辨率)/1.2（单位：勒克斯lux）
}
