#include "i2c.h"
#include "intrins.h"
#include "lcd_init.h"


// ��ʱ����
void delay(unsigned int t) {
    while (t--);
}

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

// ģ��I2C���豸��������
void I2C_Master_Transmit(unsigned char device_addr, unsigned char *dat) {
    I2C_Start();
    I2C_SendByte(device_addr << 1); // �����豸��ַ+д����
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
//    I2C_SendByte(dat >> 8); // ���͸�8λ����
		I2C_SendByte(dat[0]);
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
//    I2C_SendByte(dat & 0xff); // ���͵�8λ����
		I2C_SendByte(dat[1]);
    if (I2C_WaitAck()) {
        I2C_Stop();
        return;
    }
    I2C_Stop();
}

// ģ��I2C���豸��������
unsigned int I2C_Master_Receive(unsigned char device_addr, unsigned char *data_buffer, unsigned char length) {
    unsigned char i;
    I2C_Start();
    I2C_SendByte((device_addr << 1) + 1); // �����豸��ַ+������
    if (I2C_WaitAck()) {
        I2C_Stop();
        return 0;
    }
    for (i = 0; i < length; i++) {
        data_buffer[i] = I2C_ReceiveByte();
        if (i == length - 1) {
            I2C_Stop(); // ���һ���ֽں���ֹͣ�ź�
        } else {
            I2C_SendAck(); // ����Ӧ���ź�
        }
    }
    return 1;
}

