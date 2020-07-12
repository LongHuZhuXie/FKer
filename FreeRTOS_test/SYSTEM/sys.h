/*********************************************************************************************************************
 * @file       		sys.h
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					״̬��
 ********************************************************************************************************************/
 
#ifndef __SYS_H
#define __SYS_H

#include "headfile.h"
#include "delay.h"
#include "display.h"

/*System Define*/
#define TICK_PER_SECOND 1000 //1ms(С����) * 1000 = ������

/*System Function*/
void SysTick_Init(void); 				//ϵͳ��ʱ����ʼ��
uint32 SysTick_GetTick(void);   //��ȡϵͳ��ǰʱ��
uint32_t Getsystime_us(void);		//��ȡʱ��
void System_Init(void);					//ϵͳ��ʼ��
void Scheduler_Setup(void);			//��������
void Scheduler_Run(void);				//ѭ������

#endif
