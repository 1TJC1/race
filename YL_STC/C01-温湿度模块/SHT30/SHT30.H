#ifndef __SHT30_H__
#define __SHT30_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "lcd_init.h"

/* E53_IA1传感器数据类型定义 ------------------------------------------------------------*/
typedef struct
{
		float    Humidity;        //湿度
    float    Temperature;     //温度
} SHT30_Data_TypeDef;

/* 寄存器宏定义 --------------------------------------------------------------------*/
#define I2C_OWN_ADDRESS	0x0A
#define SHT30_Addr 0x44

void SHT30_reset(void);
void Init_SHT30();
void SHT30_Read_Data(void);

#endif





