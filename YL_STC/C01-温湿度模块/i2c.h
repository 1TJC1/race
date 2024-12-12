#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

// ����I2C����
#define SDA P21
#define SCL P20

void I2C_Start();
void I2C_Stop();
bit I2C_WaitAck();
void I2C_SendAck();
void I2C_SendByte(unsigned char dat);
unsigned char I2C_ReceiveByte();
void I2C_Master_Transmit(unsigned char device_addr, unsigned char *dat);
unsigned int I2C_Master_Receive(unsigned char device_addr, unsigned char *data_buffer, unsigned char length);

#endif





