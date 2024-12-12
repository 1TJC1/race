#include "sgp30.h"
#include "i2c.h"
#include "lcd_init.h"

sgp30_data_t sgp30_data;
u8 recv_buffer[6]={0};

void I2C_sgp30_Transmit(u8 sgp30_address,u8 *Data,u8 length)
{
	u8 i=0;
	I2C_Start();
	
	I2C_SendByte(sgp30_address);
	I2C_WaitAck();
	
	for(i=0;i<length;i++)
	{
		I2C_SendByte(Data[i]);
		I2C_WaitAck();
	}
	
	I2C_Stop();
}

u8 I2C_sgp30_Receive(u8 sgp30_address, u8 *data_buffer, u8 length) 
{
	u8 i=0;
	I2C_Start();
	
	I2C_SendByte(sgp30_address + 1); // �����豸��ַ+������
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
			I2C_Stop(); // ���һ���ֽں���ֹͣ�ź�
		} 
		else 
		{
			I2C_SendAck(); // ����Ӧ���ź�
		}
	}
	return 1;
}

/**
 * @brief	��SGP30����һ��ָ��(16bit)
 * @param	cmd SGP30ָ��
 * @retval	�ɹ�����HAL_OK
*/
void sgp30_send_cmd(sgp30_cmd_t cmd)
{
    u8 cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
		I2C_sgp30_Transmit(SGP30_ADDR,(u8*)cmd_buffer,2);
//    return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, SGP30_ADDR_WRITE, (u8*) cmd_buffer, 2, 0xFFFF);
}

/**
 * @brief	��λSGP30
 * @param	none
 * @retval	�ɹ�����HAL_OK
*/
void sgp30_soft_reset(void)
{
    u8 cmd = 0x06;
		I2C_sgp30_Transmit(0x00,&cmd,1);
//    return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, 0x00, &cmd, 1, 0xFFFF);
}

/**
 * @brief	��ʼ��SGP30������������ģʽ
 * @param	none
 * @retval	�ɹ�����0��ʧ�ܷ���-1
*/
void sgp30_init(void)
{
    sgp30_soft_reset();

    Delayxms(100);
    
    sgp30_send_cmd(INIT_AIR_QUALITY);
    
    Delayxms(100);
}

/**
 * @brief	��ʼ��SGP30������������ģʽ
 * @param	none
 * @retval	�ɹ�����HAL_OK
*/
void sgp30_start(void)
{
    sgp30_send_cmd(MEASURE_AIR_QUALITY);
}

#define CRC8_POLYNOMIAL 0x31

static u8 CheckCrc8(u8* const message, u8 initial_value)
{
    u8  remainder;	    //����
    u8  i = 0, j = 0;  //ѭ������

    /* ��ʼ�� */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* �����λ��ʼ���μ���  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }
    /* ���ؼ����CRC�� */
    return remainder;
}

/**
 * @brief	��ȡһ�ο�����������
 * @param	none
 * @retval	�ɹ�����0��ʧ�ܷ���-1
*/
void sgp30_read(void)
{
    //u8 recv_buffer[6]={0};
    
    /* �������� */
    sgp30_start();
    
    Delayxms(100);
    
    /* ��ȡ�������� */
		I2C_sgp30_Receive(SGP30_ADDR, (u8*)recv_buffer, 6);
//    status = HAL_I2C_Master_Receive(&SGP30_I2C_Handle_Name, SGP30_ADDR_READ, (u8*)recv_buffer, 6, 0xFFFF);

    
    /* У����յĲ������� */
    if (CheckCrc8(&recv_buffer[0], 0xFF) == recv_buffer[2] && CheckCrc8(&recv_buffer[3], 0xFF) == recv_buffer[5]) 
		{
			/* ת���������� */
			sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
			sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];
    }
}
