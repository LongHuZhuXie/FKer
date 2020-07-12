/*********************************************************************************************************************
 * @file       	steering.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note				���PWM			PE12
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
 *	@brief	���PWM��ʼ��
 *	@param	��
 *	@return	��
 *	@note		Timer3 Channe7 100Hz 15%
 ***********************************************************************/
void Steering_Init(void);

/************************************************************************
 *	@brief	���ö��PWMռ�ձ�
 *	@param	duty: 	ռ�ձ� => (��)[ 16.6 , 13.4 ](��)
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Set_Steering_PWM(uint8_t duty);

#endif
