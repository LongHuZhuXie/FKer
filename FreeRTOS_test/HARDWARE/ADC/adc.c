/*********************************************************************************************************************
 * @file       	adc.c
 * @version		V1.0
 * @Software 	MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note	
 ********************************************************************************************************************/

#include "adc.h"
#include "MK60_adc.h"
#include "motor.h"
#include "myTask.h"

__ADC_Data ADC_Data;

void ADC_Init(void)
{
	adc_init(ADC_LEFT1);
	adc_init(ADC_LEFT2);
	adc_init(ADC_MID);
	adc_init(ADC_MIDV);
	adc_init(ADC_RIGHT1);
	adc_init(ADC_RIGHT2);
	adc_init(ADC_BATTER);
	adc_init(ADC_BaseH);
	adc_init(ADC_BaseV);
}

void Get_ADC_Data(void)
{
	ADC_Data.L1 = adc_once(ADC_LEFT1, ADC_16bit);
	ADC_Data.R1 = adc_once(ADC_RIGHT1, ADC_16bit);
	ADC_Data.MID = adc_once(ADC_MID, ADC_16bit);
	ADC_Data.L2 = adc_once(ADC_LEFT2, ADC_16bit);
	ADC_Data.R2 = adc_once(ADC_RIGHT2, ADC_16bit);
	ADC_Data.MIDV = adc_once(ADC_MIDV, ADC_16bit);
	ADC_Data.Batter = adc_once(ADC_BATTER, ADC_12bit);
	ADC_Data.RV = adc_once(ADC_BaseH,ADC_16bit);
	ADC_Data.LV = adc_once(ADC_BaseV,ADC_16bit);
	if(ADC_Data.MID < 300&&ADC_Data.L1<400 &&ADC_Data.R1<400)
	{
		Motor_PWM.left_pwm1 = 0;
		Motor_PWM.right_pwm1 = 0;
		Motor_PWM.left_pwm2 = 0;
		Motor_PWM.right_pwm2 = 0;
		Set_Motor_PWM();
//	vTaskSuspendAll();
//	while(1)
//		printf("out\r\n");
	}
}
