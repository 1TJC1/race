#include "i2c.h"
#include "intrins.h"
#include "lcd_init.h"


//// ��ʱ����
//void delay(unsigned int t) {
//    while (t--);
//}

// I2C��ʼ�ź�
void I2C_Start() {
    SDA = 1;
    SCL = 1;
    _nop_();
    SDA = 0;
    _nop_();
    SCL = 0;
}

// I2Cֹͣ�ź�
void I2C_Stop() {
    SDA = 0;
    SCL = 1;
    _nop_();
    SDA = 1;
    _nop_();
    SCL = 0;
}

// I2C�ȴ�Ӧ���ź�
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

// I2C����Ӧ���ź�
void I2C_SendAck() {
    SDA = 0; // ��SDA��������Ϊ��ack�෴��ֵ
    SCL = 1; // ����ʱ����
    _nop_();
    SCL = 0; // ����ʱ����
		SDA = 1;
}


// I2C����һ���ֽ�
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

// I2C����һ���ֽ�
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


