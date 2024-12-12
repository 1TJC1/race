#include "SHT30.h"
#include "i2c.h"

int POLYNOMIAL = 0x131;
unsigned char  data1[3];    //data array for checksum verification

SHT30_Data_TypeDef SHT30_Data;
/***************************************************************
* 函数名称: SHT30_reset
* 说    明: SHT30复位
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void SHT30_reset(void)
{
		u8 SHT3X_Resetcommand_Buffer[2]={0x30,0xA2}; //soft reset 
		I2C_Master_Transmit(SHT30_Addr, SHT3X_Resetcommand_Buffer);
//    HAL_I2C_Master_Transmit(&userI2C,SHT30_Addr<<1,SHT3X_Resetcommand_Buffer,2,0xff);
    Delayxms(15);
}
/***************************************************************
* 函数名称: Init_SHT30
* 说    明: 初始化SHT30，设置测量周期
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Init_SHT30(void)
{
		u8 SHT3X_Modecommand_Buffer[2]={0x22,0x36}; //periodic mode commands 
		I2C_Master_Transmit(SHT30_Addr, SHT3X_Modecommand_Buffer);
//		HAL_I2C_Master_Transmit(&userI2C,SHT30_Addr<<1,SHT3X_Modecommand_Buffer,2,0xff); //send periodic mode commands
}
/***************************************************************
* 函数名称: SHT3x_CheckCrc
* 说    明: 检查数据正确性
* 参    数: data：读取到的数据
						nbrOfBytes：需要校验的数量
						checksum：读取到的校对比验值
* 返 回 值: 校验结果，0-成功		1-失败
***************************************************************/
u8 SHT3x_CheckCrc(char dat[], char nbrOfBytes, char checksum)
{
    char crc = 0xFF;
    char bit1 = 0;
    char byteCtr ;
	
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (dat[byteCtr]);
        for ( bit1 = 8; bit1 > 0; --bit1)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
}
/***************************************************************
* 函数名称: SHT3x_CalcTemperatureC
* 说    明: 温度计算
* 参    数: u16sT：读取到的温度原始数据
* 返 回 值: 计算后的温度数据
***************************************************************/
float SHT3x_CalcTemperatureC(unsigned short u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [℃] --
    temperatureC = (175 * (float)u16sT / 65535 - 45); //T = -45 + 175 * rawValue / (2^16-1)
	
    return temperatureC;
}
/***************************************************************
* 函数名称: SHT3x_CalcRH
* 说    明: 湿度计算
* 参    数: u16sRH：读取到的湿度原始数据
* 返 回 值: 计算后的湿度数据
***************************************************************/
float SHT3x_CalcRH(unsigned short u16sRH)
{
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    humidityRH = (100 * (float)u16sRH / 65535);  // RH = rawValue / (2^16-1) * 10
	
    return humidityRH;
}
/***************************************************************
* 函数名称: Init_SHT30
* 说    明: 初始化Init_SHT30
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void Init_SHT30_config(void)
{
		Init_SHT30();
}
/***************************************************************
* 函数名称: SHT30_Read_Data
* 说    明: 测量温度、湿度
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void SHT30_Read_Data(void)
{
    //char  data[3];    //data array for checksum verification
//		unsigned char addr = 0;
    unsigned short tmp = 0;
//    float t = 0;
		u16 dat;
		u8 SHT3X_Fetchcommand_Bbuffer[2]={0xE0,0x00};								//read the measurement results
		u8 SHT3X_Data_Buffer[6]; 																		//byte 0,1 is temperature byte 4,5 is humidity
		
		I2C_Master_Transmit(SHT30_Addr,SHT3X_Fetchcommand_Bbuffer); //Read sht30 sensor data 
		I2C_Master_Receive(SHT30_Addr,SHT3X_Data_Buffer,6); 
//		HAL_I2C_Master_Transmit(&userI2C,SHT30_Addr<<1,SHT3X_Fetchcommand_Bbuffer,2,0xff); //Read sht30 sensor data 
//		HAL_I2C_Master_Receive(&userI2C,(SHT30_Addr<<1)+1,SHT3X_Data_Buffer,6,0xff); 
		
		//    /* check tem */
    data1[0] = SHT3X_Data_Buffer[0];
    data1[1] = SHT3X_Data_Buffer[1];
    data1[2] = SHT3X_Data_Buffer[2];
	
		tmp=SHT3x_CheckCrc(data1, 2, data1[2]);
		if( !tmp ) /* value is ture */
    {
        dat = ((u16)data1[0] << 8) | data1[1];
        SHT30_Data.Temperature = SHT3x_CalcTemperatureC( dat );    
    }
		
		//    /* check humidity */
		data1[0] = SHT3X_Data_Buffer[3];
    data1[1] = SHT3X_Data_Buffer[4];
    data1[2] = SHT3X_Data_Buffer[5];
	
		tmp=SHT3x_CheckCrc(data1, 2, data1[2]);
		if( !tmp ) /* value is ture */
    {
        dat = ((u16)data1[0] << 8) | data1[1];
        SHT30_Data.Humidity = SHT3x_CalcRH( dat );    
    } 
}

