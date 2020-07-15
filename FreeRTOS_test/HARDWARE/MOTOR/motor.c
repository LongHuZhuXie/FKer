/*********************************************************************************************************************
 * @file       		motor.c
 * @version    		V1.0
 * @Software 		IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-15
 * @note			���PWM
 *					Peripheral		MCU
 *					R_PWM1			PD4(TM0 ch4)
 *					R_PWM2			PD5(TM0 ch5)
 *					L_PWM1			PD6(TM0 ch6)
 *					L_PWM2			PD7(TM0 ch7)
 ********************************************************************************************************************/

#include "motor.h"

/* ����PWM�ṹ�� */
__Motor_PWM Motor_PWM = {0,0,0,0};

/************************************************************************
 *	@brief	����PWMռ�ձ�
 *	@param	��
 *	@return	��
 *	@note	�������ĸ�������ֵ�ٵ��ô˺���
 ***********************************************************************/
void Set_Motor_PWM(void)
{
	if(Motor_PWM.left_pwm1 >= 0)
	{
		FTM0->CONTROLS[4].CnV = (uint16_t)(Motor_PWM.left_pwm1 * 2.251f);
		FTM0->CONTROLS[5].CnV = (uint16_t)(0);
	}
	else
	{
		FTM0->CONTROLS[4].CnV = (uint16_t)(0);
		FTM0->CONTROLS[5].CnV = (uint16_t)(-(Motor_PWM.left_pwm1) * 2.251f);
	}
	if(Motor_PWM.right_pwm1 >= 0)
	{
		FTM0->CONTROLS[6].CnV = (uint16_t)(Motor_PWM.right_pwm1 * 2.251f);
		FTM0->CONTROLS[7].CnV = (uint16_t)(0);
	}
	else
	{
		FTM0->CONTROLS[6].CnV = (uint16_t)(0);
		FTM0->CONTROLS[7].CnV = (uint16_t)(-(Motor_PWM.right_pwm1) * 2.251f);
	}
}

/************************************************************************
 *	@brief	ȫ����������ʼ��
 *	@param	��
 *	@return	��
 *	@note	��
 ***********************************************************************/
void Motor_PWM_Init(void)
{
	gpio_init(L_PWM1_Pin,GPO,0);
	gpio_init(L_PWM2_Pin,GPO,0);
	gpio_init(R_PWM1_Pin,GPO,0);
	gpio_init(R_PWM2_Pin,GPO,0);
	Motor_PWM.left_pwm1  = 320;
	Motor_PWM.left_pwm2  = 0;
	Motor_PWM.right_pwm1 = 320;
	Motor_PWM.right_pwm2 = 0;
	ftm_pwm_init(ftm0,L_PWM1_Channel,Motor_PWM_Frequence,Motor_PWM.left_pwm1);
	ftm_pwm_init(ftm0,L_PWM2_Channel,Motor_PWM_Frequence,Motor_PWM.left_pwm2);
	ftm_pwm_init(ftm0,R_PWM1_Channel,Motor_PWM_Frequence,Motor_PWM.right_pwm1);
	ftm_pwm_init(ftm0,R_PWM2_Channel,Motor_PWM_Frequence,Motor_PWM.right_pwm2);
}

/*--------------------END--------------------*/



















