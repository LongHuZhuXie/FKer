/*********************************************************************************************************************
 * @file       	steering.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note				舵机PWM			PE12
 ********************************************************************************************************************/

#include "steering.h"

uint16_t Steering_PWM = 156;

/************************************************************************
 *	@brief	舵机PWM初始化
 *	@param	无
 *	@return	无
 *	@note		Timer3 Channe7 100Hz 15%
 ***********************************************************************/
void Steering_Init(void)
{
	gpio_init(Steering_Pin, GPO, 0);
	ftm_pwm_init(ftm3,Steering_PWM_Channel,Steering_PWM_Frequence,Steering_PWM);
}

/************************************************************************
 *	@brief	设置舵机PWM占空比
 *	@param	duty: 	占空比 => (左)[ 16.6 , 14.6 ](右)
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Set_Steering_PWM(uint8_t duty)
{
	if((duty >= 134) && (duty <= 166))
	{
//		ftm_pwm_duty(ftm3, Steering_PWM_Channel, duty);
		FTM3->CONTROLS[7].CnV = (uint16_t)(duty * 56.251f);
	}
}
