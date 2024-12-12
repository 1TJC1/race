#ifndef __BH1750_H__
#define __BH1750_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* E53_IA1传感器数据类型定义 ------------------------------------------------------------*/
typedef struct
{
		float    Lux;							//光照强度
		
} BH1750_Data_TypeDef;

/* 寄存器宏定义 --------------------------------------------------------------------*/
#define I2C_OWN_ADDRESS  0x0A

#define BH1750_Addr 0x46 //0x46->0100 0110----->0x23->0010 0011     0xb8->1011 1000----->0x5c->0101 1100
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

void BH1750_Read_Data(void);
void Init_BH1750_config(void);

#endif





