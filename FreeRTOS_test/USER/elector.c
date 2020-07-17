#include "elector.h"
#include "MyTask.h"
volatile Pid_struct direction;
volatile Pid_struct speed;
#define MSPDR 320
#define MSPDL 320
void D_PID_initial(float P , float I, float D)
{
	direction.error = direction.error_L = direction.error_L_L = 0;
	direction.P = P;
	direction.I = I;
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
//	float LengthR = 12.0f-(12.0f*R_pce);
//	float LengthL = 12.0f-(12.0f*L_pce);
	//float R3p = tanf((R_pce-0.5f)*3.14159f/2.0);
	float LengthR = 221.16f*powf(R_pce,5) - 551.01*powf(R_pce,4) + 524.24*powf(R_pce,3) - 236.31*powf(R_pce,2) + 56.744*R_pce - 0.8573;
	float LengthL = 221.16f*powf(L_pce,5) - 551.01*powf(L_pce,4) + 524.24*powf(L_pce,3) - 236.31*powf(L_pce,2) + 56.744*L_pce - 0.8573;
	//printf("L=%f  R=%f\n",LengthL,LengthR);
	//printf("Rp3 = %f\n", LengthR);
	//printf("SL=%d SR=%d\n",Speed_L,Speed_R);
		if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300){}
		else
		{
			Motor_PWM.left_pwm1 = MSPDL;
			Motor_PWM.right_pwm1 = MSPDR;
		}
	if(ADC_Data.L1 > ADC_Data.R1 && ADC_Data.L1 >300)
	{
		direction.error = LengthL;
		//D_PID_initial(2*(LengthL/10.0),0,0.1);
		if (LengthL < 12.0f  && LengthL > 5.5f)
		{
			Speed(0, LengthL);
		}
				if (LengthL < 6)
		{
			direction.P=1.53f*(LengthL/10.0);
		}
		else if(LengthL > 6 && Speed_L < 270&&Speed_R < 270){direction.P=1.97f*(LengthL/10.0);}
		else{direction.P=2.27f*(LengthL/10.0);Motor_PWM.left_pwm1 = -1;Motor_PWM.right_pwm1 = -1;}
	}
	else if(ADC_Data.L1 < ADC_Data.R1 && ADC_Data.R1>300)
	{
		//D_PID_initial(3*(LengthR/10.0),0,0.1);
		direction.error = -LengthR;
		if (LengthR < 12.0f  && LengthR > 5.5f)
		{
			Speed(1, LengthR);
		}
				if (LengthR < 6)
		{
			direction.P=1.53f*(LengthR/10.0);
		}
		else if(LengthR > 6 && Speed_L < 270&&Speed_R < 270){direction.P=1.97f*(LengthR/10.0);}
		else{direction.P=2.27f*(LengthR/10.0);Motor_PWM.left_pwm1 = -1;Motor_PWM.right_pwm1 = -1;}
	}
	Set_Motor_PWM();
	duty = 150+(direction.P*direction.error+direction.D*(direction.error - 2*direction.error_L+ direction.error_L_L));
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

void Speed(char LR, float Length)
{
	static char comfirm=0;
	float r=0;
	float prc=0;
	if (comfirm == LR)
	{}
	else{
		comfirm = LR;
		speed.error = speed.error_L = speed.error_L_L = 0;
		}
	if (LR)//срв╙
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;

		r = 50.0f+(Length - 5.5f)*9.28f;
		prc = r/(r+0.1f);
	  speed.error = Speed_R - prc*Speed_L;
		Motor_PWM.right_pwm1 = MSPDR - speed.P*speed.error+speed.D*(speed.error - 2*speed.error_L + speed.error_L_L); 
		if(Motor_PWM.right_pwm1<0)Motor_PWM.right_pwm1=0;
	}
	else
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;
		
		r = 50.0f+(Length - 5.5f)*10.28f;
		prc = r/(r+0.2f);
	  speed.error = Speed_L - prc*Speed_R;
		Motor_PWM.left_pwm1 =  MSPDL - speed.P*speed.error+speed.D*(speed.error -   speed.error_L);
		if(Motor_PWM.left_pwm1<0)Motor_PWM.left_pwm1=0;
	}
}
