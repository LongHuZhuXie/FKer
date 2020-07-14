#ifndef __AT24C02_H
#define	__AT24C02_H

#include "common.h"

#define AT24C02_Address		0xA0

void AT24C02_Init(void);
uint8_t AT24C02_Check(void);  												//�������
uint8_t AT24C02_ReadOneByte(uint16_t ReadAddr);								//ָ����ַ��ȡһ���ֽ�
void AT24C02_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);			//ָ����ַд��һ���ֽ�
void AT24C02_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t AT24C02_ReadLenByte(uint16_t ReadAddr,uint8_t Len);				//ָ����ַ��ʼ��ȡָ����������
void AT24C02_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24C02_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   //��ָ����ַ��ʼ����ָ�����ȵ�����

#endif
