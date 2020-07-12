/*********************************************************************************************************************
 * @file       	adc.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note	
 ********************************************************************************************************************/
#include "motor.h"
#include "adc.h"
#include "MK60_adc.h"

__ADC_Data ADC_Data = {0,0,0};

void ADC_Init(void)
{
	adc_init(ADC_LEFT1);
//	adc_init(ADC_LEFT2);
	adc_init(ADC_MID);
//	adc_init(ADC_RIGHT1);
	adc_init(ADC_RIGHT2);
}

void Get_ADC_Data(void)
{
	ADC_Data.L1 = adc_once(ADC_LEFT1, ADC_16bit);
	ADC_Data.R1 = adc_once(ADC_RIGHT2, ADC_16bit);
	ADC_Data.MID = adc_once(ADC_MID, ADC_16bit);
	if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300)
	{
		Motor_PWM.left_pwm1 = 0;
		Motor_PWM.right_pwm1 = 0;
		Motor_PWM.left_pwm2 = 0;
		Motor_PWM.right_pwm2 = 0;
	}
	Set_Motor_PWM();
}
