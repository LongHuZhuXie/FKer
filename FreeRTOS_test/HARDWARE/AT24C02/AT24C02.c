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

//在AT24C02指定地址读出一个数据
//ReadAddr: 开始读数的地址  
//返回值  : 读到的数据
uint8_t AT24C02_ReadOneByte(uint16_t addr)
{
	uint8_t temp = 0;
    temp = simiic_read_reg(AT24C02_Address, addr,IIC);
	return temp;
}
//在AT24C02指定地址写入一个数据
//WriteAddr  : 写入数据的目的地址
//DataToWrite: 要写入的数据
void AT24C02_WriteOneByte(uint16_t addr, uint8_t data)
{
    simiic_write_reg(AT24C02_Address, addr, data);
	Delay_ms(2);
}

//在AT24C02里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  : 开始写入的地址  
//DataToWrite: 数据数组首地址
//Len        : 要写入数据的长度2,4
void AT24C02_WriteLenByte(uint16_t addr, uint32_t data, uint8_t len)
{
	uint8_t t;
	for(t = 0; t < len; t++)
		AT24C02_WriteOneByte(addr+t, (data>>(8*t)) & 0xff);
} 

//在AT24C02里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   : 开始读出的地址 
//返回值     : 数据
//Len        : 要读出数据的长度2,4
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
//检查AT24C02是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1: 检测失败
//返回0: 检测成功
uint8_t AT24C02_Check(void)
{
	uint8_t temp;
	temp = AT24C02_ReadOneByte(255);	//避免每次开机都写
	if(temp == 0X55)	return 0;
	else								//排除第一次初始化的情况
	{
		AT24C02_WriteOneByte(255, 0X55);
	    temp = AT24C02_ReadOneByte(255);
		if(temp == 0X55)	return 0;
	}
	return 1;
}

//在AT24C02里面的指定地址开始读出指定个数的数据
//ReadAddr : 开始读出的地址 对24c02为0~255
//pBuffer  : 数据数组首地址
//NumToRead: 要读出数据的个数
void AT24C02_Read(uint16_t addr,uint8_t *buf,uint16_t num)
{
	while(num)
	{
		*buf++=AT24C02_ReadOneByte(addr++);	
		num--;
	}
}  
//在AT24C02里面的指定地址开始写入指定个数的数据
//WriteAddr : 开始写入的地址 对24c02为0~255
//pBuffer   : 数据数组首地址
//NumToWrite: 要写入数据的个数
void AT24C02_Write(uint16_t addr,uint8_t *buf,uint16_t num)
{
	while(num--)
	{
		AT24C02_WriteOneByte(addr,*buf);
		addr++;
		buf++;
	}
}

