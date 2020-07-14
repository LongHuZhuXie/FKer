/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_IIC.h
 * @brief      		模拟IIC函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note	
					模拟IIC接线定义
					------------------------------------ 
						SDA                 B3
						SCL                 B2
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _SEEKFREE_IIC_h
#define _SEEKFREE_IIC_h



#include "common.h"
#include "MK60_gpio.h"


#define SEEKFREE_SCL    B2                           //定义SCL引脚
#define SEEKFREE_SDA    B3                           //定义SDA引脚

typedef enum IIC       //DAC模块
{
    IIC,
    SCCB
} IIC_type;



void  IIC_start(void);
void  IIC_stop(void);
void  IIC_ack_main(uint8 ack_main);
void  send_ch(uint8 c);
uint8 read_ch(void);
void  simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat);
uint8 simiic_read_reg(uint8 dev_add, uint8 reg, IIC_type type);
void simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, IIC_type type);
void  IIC_init(void);











#endif

