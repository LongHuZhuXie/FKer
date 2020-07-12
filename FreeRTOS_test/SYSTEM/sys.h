/*********************************************************************************************************************
 * @file       		sys.h
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					状态机
 ********************************************************************************************************************/
 
#ifndef __SYS_H
#define __SYS_H

#include "headfile.h"
#include "delay.h"
#include "display.h"

/*System Define*/
#define TICK_PER_SECOND 1000 //1ms(小周期) * 1000 = 总周期

/*System Function*/
void SysTick_Init(void); 				//系统定时器初始化
uint32 SysTick_GetTick(void);   //获取系统当前时间
uint32_t Getsystime_us(void);		//获取时间
void System_Init(void);					//系统初始化
void Scheduler_Setup(void);			//设置任务
void Scheduler_Run(void);				//循环任务

#endif
