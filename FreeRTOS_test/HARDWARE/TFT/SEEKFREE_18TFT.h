/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_18TFT.h
 * @brief      		TFT液晶函数库
 * @company         成都逐飞科技有限公司
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
						SCL                 D12
						SDA                 D13
						RES                 D14
						DC                  D15
						CS                  GND
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
	
					------------------------------------ 

 ********************************************************************************************************************/



#ifndef _SEEKFREE_18TFT_H
#define _SEEKFREE_18TFT_H

#include "common.h"


#define	TFT_X_MAX	128	//液晶X方宽度
#define TFT_Y_MAX	160 //液晶Y方宽度


#define DC_PIN		D15	//液晶命令位硬件定义		
#define REST_PIN	D14 //液晶复位引脚定义
#define DC(x)   	gpio_set(DC_PIN,x)
#define REST(x) 	gpio_set(REST_PIN,x)

//-------常用颜色在SEEKFREE_FONT.h文件中定义----------
//#define RED          	0xF800	//红色
//#define BLUE         	0x001F  //蓝色
//#define YELLOW       	0xFFE0	//黄色
//#define GREEN        	0x07E0	//绿色
//#define WHITE        	0xFFFF	//白色
//#define BLACK        	0x0000	//黑色 
//#define GRAY  		0X8430 	//灰色
//#define BROWN 		0XBC40 	//棕色
//#define PURPLE    	0XF81F	//紫色
//#define PINK    		0XFE19	//粉色

//定义写字笔的颜色
#define PENCOLOR PURPLE

//定义背景颜色
#define BGCOLOR	 WHITE


void lcd_init(void);
void dsp_single_colour(int color);
void lcd_showchar(uint16 x,uint16 y,uint8 dat);
void lcd_showstr(uint16 x,uint16 y,uint8 dat[]);
void lcd_showint8(uint16 x,uint16 y,int8 dat);
void lcd_showint16(uint16 x,uint16 y,int16 dat);
//void showimage(const unsigned char *p);
//void displayimage032(uint8 *p);
//void displayimage7725(unsigned char *p);



#endif
