#include "i2c.h"
#include "intrins.h"
#include "lcd_init.h"


//// 延时函数
//void delay(unsigned int t) {
//    while (t--);
//}

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


