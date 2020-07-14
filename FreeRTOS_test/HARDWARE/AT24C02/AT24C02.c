#include "AT24C02.h"
//#include "MK60_i2c.h"
#include "SEEKFREE_IIC.h"

static void Delay_ms(uint16_t ms)
{
	uint16_t i, j, k;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 100; j++)
			for(k = 0; k < 100; k++);
}

void AT24C02_Init(void)
{
	//i2c_init(i2c0, 100*1000);
	IIC_init();
}

//��AT24C02ָ����ַ����һ������
//ReadAddr: ��ʼ�����ĵ�ַ  
//����ֵ  : ����������
uint8_t AT24C02_ReadOneByte(uint16_t addr)
{
	uint8_t temp = 0;
    temp = simiic_read_reg(AT24C02_Address, addr,IIC);
	return temp;
}
//��AT24C02ָ����ַд��һ������
//WriteAddr  : д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite: Ҫд�������
void AT24C02_WriteOneByte(uint16_t addr, uint8_t data)
{
    simiic_write_reg(AT24C02_Address, addr, data);
	Delay_ms(2);
}

//��AT24C02�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  : ��ʼд��ĵ�ַ  
//DataToWrite: ���������׵�ַ
//Len        : Ҫд�����ݵĳ���2,4
void AT24C02_WriteLenByte(uint16_t addr, uint32_t data, uint8_t len)
{
	uint8_t t;
	for(t = 0; t < len; t++)
		AT24C02_WriteOneByte(addr+t, (data>>(8*t)) & 0xff);
} 

//��AT24C02�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   : ��ʼ�����ĵ�ַ 
//����ֵ     : ����
//Len        : Ҫ�������ݵĳ���2,4
uint32_t AT24C02_ReadLenByte(uint16_t addr, uint8_t len)
{  	
	uint8_t t;
	uint32_t temp = 0;
	for(t = 0; t < len; t++)
	{
		temp <<= 8;
		temp += AT24C02_ReadOneByte(addr+len-t-1);
	}
	return temp;
}
//���AT24C02�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1: ���ʧ��
//����0: ���ɹ�
uint8_t AT24C02_Check(void)
{
	uint8_t temp;
	temp = AT24C02_ReadOneByte(255);	//����ÿ�ο�����д
	if(temp == 0X55)	return 0;
	else								//�ų���һ�γ�ʼ�������
	{
		AT24C02_WriteOneByte(255, 0X55);
	    temp = AT24C02_ReadOneByte(255);
		if(temp == 0X55)	return 0;
	}
	return 1;
}

//��AT24C02�����ָ����ַ��ʼ����ָ������������
//ReadAddr : ��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  : ���������׵�ַ
//NumToRead: Ҫ�������ݵĸ���
void AT24C02_Read(uint16_t addr,uint8_t *buf,uint16_t num)
{
	while(num)
	{
		*buf++=AT24C02_ReadOneByte(addr++);	
		num--;
	}
}  
//��AT24C02�����ָ����ַ��ʼд��ָ������������
//WriteAddr : ��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   : ���������׵�ַ
//NumToWrite: Ҫд�����ݵĸ���
void AT24C02_Write(uint16_t addr,uint8_t *buf,uint16_t num)
{
	while(num--)
	{
		AT24C02_WriteOneByte(addr,*buf);
		addr++;
		buf++;
	}
}

