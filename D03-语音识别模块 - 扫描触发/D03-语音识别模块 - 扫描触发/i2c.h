#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

// ¶¨ÒåI2CÒý½Å
#define SDA P21
#define SCL P20

void I2C_Start();
void I2C_Stop();
bit I2C_WaitAck();
void I2C_SendAck();
void I2C_SendByte(unsigned char dat);
unsigned char I2C_ReceiveByte();

#endif





