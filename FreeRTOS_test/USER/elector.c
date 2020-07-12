#include "elector.h"
volatile Pid_struct direction;
volatile Pid_struct speed;

void D_PID_initial(float P , float I, float D)
{
	direction.error = direction.error_L = direction.error_L_L = 0;
	direction.P = P;
	direction.I = I;
	direction.D = D;
}

void M_PID_initial(float P , float I, float D)
{
	speed.error = speed.error_L = speed.error_L_L = 0;
	speed.P = P;
	speed.I = I;
	speed.D = D;
}

void Direct(void)
{
	uint8 duty;
	direction.error_L_L = direction.error_L;
	direction.error_L = direction.error;
	float L_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.L1);
	float R_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.R1);

		if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300){}
		else
		{
			Motor_PWM.left_pwm1 = 360;
			Motor_PWM.right_pwm1 = 360;
		}
	if(ADC_Data.L1 > ADC_Data.R1 && ADC_Data.L1 >300)
	{
		direction.error = L_pce;
		if (L_pce > 0.5f)
		{
			Motor_PWM.left_pwm1 = 0.6f*300;
		}
	}
	else if(ADC_Data.L1 < ADC_Data.R1 && ADC_Data.R1>300)
	{
		direction.error = -R_pce;
		if (R_pce > 0.5f)
		{
			Motor_PWM.right_pwm1 = 0.6f*300;
		}
	}
	Set_Motor_PWM();
	duty = 150+(direction.P*direction.error+direction.D*(direction.error - direction.error_L + direction.error_L_L));
	if (duty > 166)
	{
		duty = 166;
	}
	if (duty < 134)
	{
		duty =134;
	}
	Set_Steering_PWM(duty);
}

void Direct_acr(void)
{
	uint8 duty;
	float L_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.L1);
	float R_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.R1);
	float power_L = (ADC_Data.MID - ADC_Data.L1)/ADC_Data.MID;
	float power_R = (ADC_Data.MID - ADC_Data.R1)/ADC_Data.MID;
	if(power_L<-1)
	{
		power_L = -1;
	}
	if(power_R<-1)
	{
		power_R = -1;
	}
	power_L = 1-power_L;
	power_R = 1-power_R;
		if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300){}
		else
		{
			Motor_PWM.left_pwm1 = 320;
			Motor_PWM.right_pwm1 = 320;
		}
	float l=0;
	if(ADC_Data.L1 > ADC_Data.R1 && ADC_Data.L1 >300)
	{
		l = L_pce;
		duty = 150+((atanf(L_pce*13.2f/5.7f)/3.14159f*180)/2.8125f)*5.0f*power_L;
		//if ((atanf(L_pce)/3.14159f*180)>42.8)duty+=1;
	}
	else if(ADC_Data.L1 < ADC_Data.R1 && ADC_Data.R1>300)
	{
		l = R_pce;
		duty = 150-((atanf(R_pce*13.2f/5.7f)/3.14159f*180)/2.8125f)*5.0f*power_R;
		//if((atanf(R_pce)/3.14159f*180)>42.8)duty-=1;
	}
	float p_cos;
	float p_sin;
	float temp;
	arm_sqrt_f32(12*12-l*l,&temp);
	arm_sin_cos_f32(90.0f-(atanf(L_pce)*3.14159f*180),&p_sin,&p_cos);
//	printf("%f \r\n", (0.5f*temp)/p_cos);//,1.0f/l,atanf(L_pce)*3.14159f*180);
	Set_Motor_PWM();
	if (duty > 166)
	{
		duty = 166;
	}
	if (duty < 134)
	{
		duty =134;
	}
	Set_Steering_PWM(duty);
}