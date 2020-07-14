#include "IIC.h"
#include "MK60_gpio.h"

#define SCL		B2
#define SDA		B3

//IO��������
#define SDA_IN()  gpio_ddr(SDA,GPI)
#define SDA_OUT() gpio_ddr(SDA,GPO)

//IO��������
#define IIC_SCL(x)	gpio_set(SCL,x) //SCL
#define IIC_SDA(x)	gpio_set(SDA,x) //SDA	 
#define READ_SDA()	gpio_get(SDA)  //����SDA 

static void Delay_us(unsigned char us) //�����ʱ
{
	uint8_t i = 10;
	while(us--)
	{
	    while(i--);
	}
}

//��ʼ��IIC
void IIC_Init(void)
{
	gpio_init(SCL,GPO,1);
	gpio_init(SDA,GPO,1);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA(1);
	Delay_us(1);	  	  
	IIC_SCL(1);
	Delay_us(5);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	Delay_us(5);
	IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
	Delay_us(2);
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL(1); 
	Delay_us(5);
	IIC_SDA(1);//����I2C���߽����ź�
	Delay_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA(1);Delay_us(1);	   
	IIC_SCL(1);Delay_us(1);	 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	Delay_us(2);
	IIC_SCL(1);
	Delay_us(2);
	IIC_SCL(0);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	Delay_us(2);
	IIC_SCL(1);
	Delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA(1);
		else
			IIC_SDA(0);
		txd<<=1; 	  
		Delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL(1);
		Delay_us(2); 
		IIC_SCL(0);	
		Delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        Delay_us(2);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA())receive++;   
		Delay_us(1); 
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


