/*********************************************************************************************************************
 * @file       	steering.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note				舵机PWM			PE12
 ********************************************************************************************************************/

#ifndef __STEERING_H
#define __STEERING_H

#include "MK60_ftm.h"
#include "MK60_gpio.h"

extern uint16_t Steering_PWM;

#define Steering_Pin							E12
#define Steering_PWM_Frequence		100
#define Steering_PWM_Channel			ftm_ch7

/************************************************************************
 *	@brief	舵机PWM初始化
 *	@param	无
 *	@return	无
 *	@note		Timer3 Channe7 100Hz 15%
 ***********************************************************************/
void Steering_Init(void);

/************************************************************************
 *	@brief	设置舵机PWM占空比
 *	@param	duty: 	占空比 => (左)[ 16.6 , 13.4 ](右)
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Set_Steering_PWM(uint8_t duty);

#endif
