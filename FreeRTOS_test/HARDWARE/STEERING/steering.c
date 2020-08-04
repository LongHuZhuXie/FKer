/*********************************************************************************************************************
 * @file       	steering.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note				���PWM			PE12
 ********************************************************************************************************************/

#include "steering.h"

uint16_t Steering_PWM = 156;

/************************************************************************
 *	@brief	���PWM��ʼ��
 *	@param	��
 *	@return	��
 *	@note		Timer3 Channe7 100Hz 15%
 ***********************************************************************/
void Steering_Init(void)
{
	gpio_init(Steering_Pin, GPO, 0);
	ftm_pwm_init(ftm3,Steering_PWM_Channel,Steering_PWM_Frequence,Steering_PWM);
}

/************************************************************************
 *	@brief	���ö��PWMռ�ձ�
 *	@param	duty: 	ռ�ձ� => (��)[ 16.6 , 14.6 ](��)
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Set_Steering_PWM(uint8_t duty)
{
	if((duty >= 134) && (duty <= 166))
	{
//		ftm_pwm_duty(ftm3, Steering_PWM_Channel, duty);
		FTM3->CONTROLS[7].CnV = (uint16_t)(duty * 56.251f);
	}
}
