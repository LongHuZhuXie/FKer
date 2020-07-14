#include "IIC.h"
#include "MK60_gpio.h"

#define SCL		B2
#define SDA		B3

//IO方向设置
#define SDA_IN()  gpio_ddr(SDA,GPI)
#define SDA_OUT() gpio_ddr(SDA,GPO)

//IO操作函数
#define IIC_SCL(x)	gpio_set(SCL,x) //SCL
#define IIC_SDA(x)	gpio_set(SDA,x) //SDA	 
#define READ_SDA()	gpio_get(SDA)  //输入SDA 

static void Delay_us(unsigned char us) //大概延时
{
	uint8_t i = 10;
	while(us--)
	{
	    while(i--);
	}
}

//初始化IIC
void IIC_Init(void)
{
	gpio_init(SCL,GPO,1);
	gpio_init(SDA,GPO,1);
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);
	Delay_us(1);	  	  
	IIC_SCL(1);
	Delay_us(5);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	Delay_us(5);
	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
	Delay_us(2);
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL(1); 
	Delay_us(5);
	IIC_SDA(1);//发送I2C总线结束信号
	Delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA(1);
		else
			IIC_SDA(0);
		txd<<=1; 	  
		Delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		Delay_us(2); 
		IIC_SCL(0);	
		Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


