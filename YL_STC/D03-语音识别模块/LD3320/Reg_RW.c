#include "Reg_RW.h"
#include "LDChip.h"
#include "main.h"

//软件模拟SPI方式读写
#define DELAY_NOP	_nop_();_nop_();_nop_();_nop_();

void LD_WriteReg(unsigned char address,unsigned char dataout)
{
	unsigned char i = 0;
	unsigned char command=0x04;
//	SPIS =0;
	SCS = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if ((command & 0x80) == 0x80) 
			SDI = 1;
		else
			SDI = 0;
		
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}
	//write address
	for (i=0;i < 8; i++)
	{
		if ((address & 0x80) == 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	//write data
	for (i=0;i < 8; i++)
	{
		if ((dataout & 0x80) == 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		dataout = (dataout << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;
	SCS = 1;
}

unsigned char LD_ReadReg(unsigned char address)
{
	unsigned char i = 0; 
	unsigned char datain =0 ;
	unsigned char temp = 0; 
	unsigned char command=0x05;
//	SPIS =0;
	SCS = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if ((command & 0x80) == 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}

	//write address
	for (i=0;i < 8; i++)
	{
		if ((address & 0x80) == 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;

	//Read
	for (i=0;i < 8; i++)
	{
		datain = (datain << 1);
		temp = SDO;
		DELAY_NOP;
		SDCK = 0;  
		if (temp == 1)  
			datain |= 0x01; 
		DELAY_NOP;
		SDCK = 1;  
	}

	DELAY_NOP;
	SCS = 1;
	return datain;
}

