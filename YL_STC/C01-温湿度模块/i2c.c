#include "i2c.h"
#include "intrins.h"
#include "lcd_init.h"


// 延时函数
void delay(unsigned int t) {
    while (t--);
}

// I2C起始信号
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    _nop_();
    SDA = 0;
    _nop_();
    SCL = 0;
}

// I2C停止信号
void I2C_Stop() {
    SDA = 0;
    SCL = 1;
    _nop_();
    SDA = 1;
    _nop_();
    SCL = 0;
}

// I2C等待应答信号
bit I2C_WaitAck() {
    bit ack;
    SDA = 1;
    _nop_();
    SCL = 1;
    _nop_();
    ack = SDA;
    SCL = 0;
    return ack;
}

// I2C发送应答信号
void I2C_SendAck() {
    SDA = 0; // 将SDA引脚设置为与ack相反的值
    SCL = 1; // 拉高时钟线
    _nop_();
    SCL = 0; // 拉低时钟线
		SDA = 1;
}


// I2C发送一个字节
void I2C_SendByte(unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SDA = CY;
        _nop_();
        SCL = 1;
        _nop_();
        SCL = 0;
    }
}

// I2C接收一个字节
unsigned char I2C_ReceiveByte() {
    unsigned char i, dat = 0;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1;
        _nop_();
        dat |= SDA;
        SCL = 0;
    }
    return dat;
}

// 模拟I2C主设备发送数据
void I2C_Master_Transmit(unsigned char device_addr, unsigned char *dat) {
    I2C_Start();
    I2C_SendByte(device_addr << 1); // 发送设备地址+写命令
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
//    I2C_SendByte(dat >> 8); // 发送高8位数据
		I2C_SendByte(dat[0]);
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
//    I2C_SendByte(dat & 0xff); // 发送低8位数据
		I2C_SendByte(dat[1]);
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
    I2C_Stop();
}

// 模拟I2C主设备接收数据
unsigned int I2C_Master_Receive(unsigned char device_addr, unsigned char *data_buffer, unsigned char length) {
    unsigned char i;
    I2C_Start();
    I2C_SendByte((device_addr << 1) + 1); // 发送设备地址+读命令
    if (I2C_WaitAck()) {
        I2C_Stop();
        return 0;
    }
    for (i = 0; i < length; i++) {
        data_buffer[i] = I2C_ReceiveByte();
        if (i == length - 1) {
            I2C_Stop(); // 最后一个字节后发送停止信号
        } else {
            I2C_SendAck(); // 发送应答信号
        }
    }
    return 1;
}

