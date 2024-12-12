#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include <STC15F2K60S2.H>

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 240

#else
#define LCD_W 240
#define LCD_H 240
#endif

//-----------------LCD端口定义----------------
#define LCD_SCLK_Clr() P12 = 0
#define LCD_SCLK_Set() P12 = 1

#define LCD_MOSI_Clr() P55 = 0
#define LCD_MOSI_Set() P55 = 1

#define LCD_RES_Clr()  P40 = 0//RES
#define LCD_RES_Set()  P40 = 1

#define LCD_DC_Clr()   P53 = 0//DC
#define LCD_DC_Set()   P53 = 1

#define LCD_CS_Clr()   P47 = 0//CS
#define LCD_CS_Set()   P47 = 1

#define LCD_BLK_Clr()  P50 = 0//BLK
#define LCD_BLK_Set()  P50 = 1

void Delayxms(unsigned int x);
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化


#endif
