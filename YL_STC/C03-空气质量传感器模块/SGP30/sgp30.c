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
	
	I2C_SendByte(sgp30_address + 1); // 发送设备地址+读命令
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
			I2C_Stop(); // 最后一个字节后发送停止信号
		} 
		else 
		{
			I2C_SendAck(); // 发送应答信号
		}
	}
	return 1;
}

/**
 * @brief	向SGP30发送一条指令(16bit)
 * @param	cmd SGP30指令
 * @retval	成功返回HAL_OK
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
 * @brief	软复位SGP30
 * @param	none
 * @retval	成功返回HAL_OK
*/
void sgp30_soft_reset(void)
{
    u8 cmd = 0x06;
		I2C_sgp30_Transmit(0x00,&cmd,1);
//    return HAL_I2C_Master_Transmit(&SGP30_I2C_Handle_Name, 0x00, &cmd, 1, 0xFFFF);
}

/**
 * @brief	初始化SGP30空气质量测量模式
 * @param	none
 * @retval	成功返回0，失败返回-1
*/
void sgp30_init(void)
{
    sgp30_soft_reset();

    Delayxms(100);
    
    sgp30_send_cmd(INIT_AIR_QUALITY);
    
    Delayxms(100);
}

/**
 * @brief	初始化SGP30空气质量测量模式
 * @param	none
 * @retval	成功返回HAL_OK
*/
void sgp30_start(void)
{
    sgp30_send_cmd(MEASURE_AIR_QUALITY);
}

#define CRC8_POLYNOMIAL 0x31

static u8 CheckCrc8(u8* const message, u8 initial_value)
{
    u8  remainder;	    //余数
    u8  i = 0, j = 0;  //循环变量

    /* 初始化 */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* 从最高位开始依次计算  */
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
    /* 返回计算的CRC码 */
    return remainder;
}

/**
 * @brief	读取一次空气质量数据
 * @param	none
 * @retval	成功返回0，失败返回-1
*/
void sgp30_read(void)
{
    //u8 recv_buffer[6]={0};
    
    /* 启动测量 */
    sgp30_start();
    
    Delayxms(100);
    
    /* 读取测量数据 */
		I2C_sgp30_Receive(SGP30_ADDR, (u8*)recv_buffer, 6);
//    status = HAL_I2C_Master_Receive(&SGP30_I2C_Handle_Name, SGP30_ADDR_READ, (u8*)recv_buffer, 6, 0xFFFF);

    
    /* 校验接收的测量数据 */
    if (CheckCrc8(&recv_buffer[0], 0xFF) == recv_buffer[2] && CheckCrc8(&recv_buffer[3], 0xFF) == recv_buffer[5]) 
		{
			/* 转换测量数据 */
			sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
			sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];
    }
}
