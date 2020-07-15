/*********************************************************************************************************************
 * @file       		motor.h
 * @version    		V1.0
 * @Software 		IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note			µç»úPWM
 *					Peripheral		MCU
 *					L_PWM1			PD4(TM0 ch4)
 *					L_PWM2			PD5(TM0 ch5)
 *					R_PWM1			PD6(TM0 ch6)
 *					R_PWM2			PD7(TM0 ch7)
 ********************************************************************************************************************/
 
#ifndef __MOTOR_H
#define __MOTOR_H

#include "common.h"
#include "MK60_gpio.h"
#include "MK60_ftm.h"

#define Motor_PWM_Frequence		40000

#define L_PWM1_Pin		D4
#define L_PWM2_Pin		D5
#define R_PWM1_Pin		D6
#define R_PWM2_Pin		D7

#define L_PWM1_Channel		ftm_ch4
#define L_PWM2_Channel		ftm_ch5
#define R_PWM1_Channel		ftm_ch6
#define R_PWM2_Channel		ftm_ch7

typedef volatile struct	Motor_PWM_Type
{
	int right_pwm1;
	int right_pwm2;
	int left_pwm1;
	int left_pwm2;
} __Motor_PWM;

extern __Motor_PWM Motor_PWM;


void Motor_PWM_Init(void);
void Set_Motor_PWM(void);


#endif

/*--------------------END--------------------*/

























