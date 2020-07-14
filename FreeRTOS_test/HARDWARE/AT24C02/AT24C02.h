#ifndef __AT24C02_H
#define	__AT24C02_H

#include "common.h"

#define AT24C02_Address		0xA0

void AT24C02_Init(void);
uint8_t AT24C02_Check(void);  												//检查器件
uint8_t AT24C02_ReadOneByte(uint16_t ReadAddr);								//指定地址读取一个字节
void AT24C02_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);			//指定地址写入一个字节
void AT24C02_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);	//指定地址开始写入指定长度的数据
uint32_t AT24C02_ReadLenByte(uint16_t ReadAddr,uint8_t Len);				//指定地址开始读取指定长度数据
void AT24C02_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);//从指定地址开始写入指定长度的数据
void AT24C02_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   //从指定地址开始读出指定长度的数据

#endif
