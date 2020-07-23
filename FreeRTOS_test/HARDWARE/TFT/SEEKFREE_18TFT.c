/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_18TFT.c
 * @brief      		TFT液晶函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note		
					1.8 TFT液晶接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SCL                 A15
						SDA                 A16
						RES                 B17
						DC                  B16
						CS                  A14
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
	
					------------------------------------ 

 ********************************************************************************************************************/



#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FONT.h"
#include "MK60_spi.h"


void tft_delay(long t)
{
    while(t--);
}

void  Lcd_WriteIndex(uint8 dat)			//写命令
{	
    DC(0);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);		

}

void Lcd_WriteData(uint8 dat)			//写数据
{
    DC(1);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);
}

void  LCD_WriteData_16Bit(uint16 dat)	//向液晶屏写一个16位数据
{
    uint8 h,l;
    h = dat >> 8;
    l = (uint8)dat;
    
    DC(1);
    spi_mosi(spi2,SPI_PCS0,&h,&h,1); 	//写入高8位数据
    spi_mosi(spi2,SPI_PCS0,&l,&l,1);	//写入低8位数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶坐标设置
//  @param      x_start     	坐标x方向的起点
//  @param      y_start     	坐标y方向的起点
//  @param      x_end       	坐标x方向的终点
//  @param      y_end       	坐标y方向的终点
//  @return     void
//  @since      v1.0
//  Sample usage:               Lcd_SetRegion(0,0,10,10);// x、y的起点都是0，终点都是10
//-------------------------------------------------------------------------------------------------------------------
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_end+1);	

	Lcd_WriteIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               dsp_single_colour(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void dsp_single_colour(int color)
{
 	uint8 i,j;
	Lcd_SetRegion(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
 	for (i=0;i<TFT_Y_MAX;i++)
    	for (j=0;j<TFT_X_MAX;j++)
        	LCD_WriteData_16Bit(color);
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{	
    (void)spi_init(spi2, SPI_PCS0, MASTER ,25*1000*1000);//硬件SPI初始化

    gpio_init(DC_PIN,GPO,0);
    gpio_init(REST_PIN,GPO,0);
    
    REST(0);
    tft_delay(999999);
    REST(1);
    tft_delay(999999);
    Lcd_WriteIndex(0x11);
    tft_delay(999999);

    Lcd_WriteIndex(0xB1); 
    Lcd_WriteData(0x01); 
    Lcd_WriteData(0x2C); 
    Lcd_WriteData(0x2D); 
    
    Lcd_WriteIndex(0xB2); 
    Lcd_WriteData(0x01); 
    Lcd_WriteData(0x2C); 
    Lcd_WriteData(0x2D); 
    
    Lcd_WriteIndex(0xB3); 
    Lcd_WriteData(0x01); 
    Lcd_WriteData(0x2C); 
    Lcd_WriteData(0x2D); 
    Lcd_WriteData(0x01); 
    Lcd_WriteData(0x2C); 
    Lcd_WriteData(0x2D); 
    
    Lcd_WriteIndex(0xB4);
    Lcd_WriteData(0x07); 

    Lcd_WriteIndex(0xC0); 
    Lcd_WriteData(0xA2); 
    Lcd_WriteData(0x02); 
    Lcd_WriteData(0x84); 
    Lcd_WriteIndex(0xC1); 
    Lcd_WriteData(0xC5); 
    
    Lcd_WriteIndex(0xC2); 
    Lcd_WriteData(0x0A); 
    Lcd_WriteData(0x00); 
    
    Lcd_WriteIndex(0xC3); 
    Lcd_WriteData(0x8A); 
    Lcd_WriteData(0x2A); 
    Lcd_WriteIndex(0xC4); 
    Lcd_WriteData(0x8A); 
    Lcd_WriteData(0xEE); 
    
    Lcd_WriteIndex(0xC5);
    Lcd_WriteData(0x0E); 
    
    Lcd_WriteIndex(0x36);
    Lcd_WriteData(0xC0); 
	Lcd_WriteData(0xC8); 

    Lcd_WriteIndex(0xe0); 
    Lcd_WriteData(0x0f); 
    Lcd_WriteData(0x1a); 
    Lcd_WriteData(0x0f); 
    Lcd_WriteData(0x18); 
    Lcd_WriteData(0x2f); 
    Lcd_WriteData(0x28); 
    Lcd_WriteData(0x20); 
    Lcd_WriteData(0x22); 
    Lcd_WriteData(0x1f); 
    Lcd_WriteData(0x1b); 
    Lcd_WriteData(0x23); 
    Lcd_WriteData(0x37); 
    Lcd_WriteData(0x00); 	
    Lcd_WriteData(0x07); 
    Lcd_WriteData(0x02); 
    Lcd_WriteData(0x10); 
    
    Lcd_WriteIndex(0xe1); 
    Lcd_WriteData(0x0f); 
    Lcd_WriteData(0x1b); 
    Lcd_WriteData(0x0f); 
    Lcd_WriteData(0x17); 
    Lcd_WriteData(0x33); 
    Lcd_WriteData(0x2c); 
    Lcd_WriteData(0x29); 
    Lcd_WriteData(0x2e); 
    Lcd_WriteData(0x30); 
    Lcd_WriteData(0x30); 
    Lcd_WriteData(0x39); 
    Lcd_WriteData(0x3f); 
    Lcd_WriteData(0x00); 
    Lcd_WriteData(0x07); 
    Lcd_WriteData(0x03); 
    Lcd_WriteData(0x10);  
    
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00+2);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x80+2);
    
    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00+3);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x80+3);
    
    Lcd_WriteIndex(0xF0); 
    Lcd_WriteData(0x01); 
    Lcd_WriteIndex(0xF6);
    Lcd_WriteData(0x00); 
    
    Lcd_WriteIndex(0x3A);
    Lcd_WriteData(0x05); 
    
    
    Lcd_WriteIndex(0x29);

    dsp_single_colour(0xffff);//全白
}

void lcd_showchar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;
	for(i=0; i<16; i++)
	{
		Lcd_SetRegion(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	LCD_WriteData_16Bit(PENCOLOR);
			else			LCD_WriteData_16Bit(BGCOLOR);
			temp>>=1;
		}
	}
}


void lcd_showstr(uint16 x,uint16 y,uint8 dat[])
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		lcd_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}

void lcd_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		lcd_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	lcd_showchar(x,y*16,' ');
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
	
}

void lcd_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
	
}

void lcd_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		lcd_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	lcd_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

void lcd_showuint16(uint16 x,uint16 y,uint16 dat)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示40*40 QQ图片
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:          
//-------------------------------------------------------------------------------------------------------------------
//void showimage(const unsigned char *p) 
//{
//  	int i,j,k; 
//	unsigned char picH,picL;
//	dsp_single_colour(WHITE); //清屏  
//	
//	for(k=0;k<4;k++)
//	{
//	   	for(j=0;j<3;j++)
//		{	
//			Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);		//坐标设置
//		    for(i=0;i<40*40;i++)
//			 {	
//			 	picL=*(p+i*2);	//数据低位在前
//				picH=*(p+i*2+1);				
//				LCD_WriteData_16Bit(picH<<8|picL);  						
//			 }	
//		 }
//	}		
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:              
//-------------------------------------------------------------------------------------------------------------------
//void displayimage032(uint8 *p) 
//{
//    int i,j; 
//	
//    uint16 color = 0;
//	uint16 temp = 0;
//	
//	for(i=0;i<120;i++)
//    {
//        Lcd_SetRegion(i,0,i,160-1);		//坐标设置
//        for(j=0;j<160;j++)
//        {	
//			temp = *(p + i*ROW/120*COL + (COL-1)-j*(COL-1)/(160-1));//读取像素点
//			
//			color=(0x001f&((temp)>>3))<<11;
//            color=color|(((0x003f)&((temp)>>2))<<5);
//            color=color|(0x001f&((temp)>>3));
//			LCD_WriteData_16Bit(color);
//        }
//    }
//}

