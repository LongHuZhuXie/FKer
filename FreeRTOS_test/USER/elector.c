#include "elector.h"
#include "MyTask.h"
volatile Pid_struct direction;
volatile Pid_struct speed;
volatile Pid_struct RoundAb;
#define MSPDR 234
#define MSPDL 234
#define MIDMAX 5000
void D_PID_initial(float P , float I, float D)
{
	direction.error = direction.error_L = direction.error_L_L = 0;
	direction.P = P;
	direction.I = I;
 }

void M_PID_initial(float P , float I, float D)
{
	speed.error_T_L_L=speed.error_T_L=speed.error_T=speed.error = speed.error_L = speed.error_L_L = 0;
	speed.P = P;
	speed.ID = I;
	speed.D = D;
	RoundAb.P = 10.5;
	RoundAb.D = 1;
	RoundAb.error=0;
	RoundAb.error_L=0;
	RoundAb.error_L_L=0;
}

void Direct(void)
{
	uint8 duty;
	float percent_in;
	direction.error_L_L = direction.error_L;
	direction.error_L = direction.error;
	static char times = 0;
	float L_pce ;
	float R_pce ;
	float LengthR ;
	float LengthL ;
	float res;
	float over_straith=0 ;
	float angle ;
	float angle_L;
	float angle_R;
	static char rf =0;
	static char checkab_up=0;
	static char checkab_down=0;
	char checkab;
	static char rab_L=0;
	char start=0;
	static char dire = 0;

	float angle_error ;
		if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300){}
		else
		{
			Motor_PWM.left_pwm1 = MSPDL;
			Motor_PWM.right_pwm1 = MSPDR;
		}
		if(1.75f*MIDMAX < ADC_Data.MID && rab_L==0)
	{
		checkab = RAcheck(1);
		printf("check\n");
		if(checkab==1){checkab_up = checkab;}
		else
		{
			times+=1;
			if (times>1)
			{
				times=1;
			}
		}
	}
	else
	{
		checkab = RAcheck(0);
		if(checkab&&checkab_up&&times>=0)checkab_down= checkab;
	}
	if(ADC_Data.MID>MIDMAX)
	{
		ADC_Data.MID = MIDMAX;
	}
	if (checkab_up==1)
	{
		start=1;
		checkab_up=0;
		checkab_down=0;
		times=0;
		if(ADC_Data.LV>ADC_Data.RV)
		{
			dire=1;//×ó×ª
		}
		else dire=0;
	}
	if (ADC_Data.MID>MIDMAX*1.65)
	{
		over_straith=1;
	}
	rab_L=Roundabout(start,dire);
	L_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.L1);
	R_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.R1);
	LengthR = (12.0f*R_pce);
	LengthL = (12.0f*L_pce);
	angle = atanf(1.2f*ADC_Data.MID/ADC_Data.MIDV);
	//angle_L = atanf(1.2f*ADC_Data.L1/ADC_Data.LV);
	//angle_R = atanf(1.2f*ADC_Data.R1/ADC_Data.RV);
	angle = 180.0f*angle/3.14159f;
	//angle_L = 180.0f*angle_L/3.14159f;
	//angle_R = 180.0f*angle_R/3.14159f;
	//printf("angle_L=%.2f ,angle_MID=%.2f,angle_R=%.2f\n",angle_L,angle,angle_R);
	angle_error = 88.8f- angle;
	if(ADC_Data.L1 > ADC_Data.R1 && ADC_Data.L1 >300)
	{
		direction.error_L_L = direction.error_L;
		direction.error_L = direction.error;
		direction.error = LengthL;
			if(angle_error > 32&&!rab_L)
		{
			direction.error = angle_error*0.4f;
		}
		direction.P = 0.15f*direction.error+0.265;
		direction.D = 0.12f*direction.P;
		if (angle<79 && Speed_L>= MSPDL)
		{
			Speed(2, angle);
		}
		else
		{
			Speed(0, angle);
		}
		//printf("error= %f\n",direction.error);
		if (ADC_Data.MID>0.78f*MIDMAX&&ADC_Data.MIDV>0.95f*MIDMAX)direction.error=0;
	}
	else if(ADC_Data.L1 < ADC_Data.R1 && ADC_Data.R1>300)
	{
		direction.error_L_L = direction.error_L;
		direction.error_L = direction.error;
		direction.error = -LengthR;
		if(angle_error > 32 && !rab_L)
		{
			direction.error = -angle_error*0.4f;
		}
		direction.P = 0.15f*(-direction.error)+0.265;
		direction.D = 0.12*direction.P;
		if (angle<79 && Speed_R >= MSPDR)
		{
			Speed(1, angle);
		}
		else
		{
			Speed(0, angle);
		}
		//direction.error = (88.0f-angle);
		//direction.error = -direction.error;
		if (ADC_Data.MID>0.78f*MIDMAX&&ADC_Data.MIDV>0.95f*MIDMAX)direction.error=0;
	}
	if (angle > 83 && direction.error<3)direction.error=0;
	
	if(over_straith==1&&rab_L==0)
	{
		direction.error = 0;
	}
	res = 150+(direction.P*direction.error + direction.I*(direction.error-direction.error_L)+
	direction.D*(direction.error - 2*direction.error_L+ direction.error_L_L));
	
	//printf("direcerror=%f \n",direction.error);
	if (res > 166)
	{
		res = 166;
	}
	if (res < 134)
	{
		res =134;
	}
	duty = res;
	Set_Steering_PWM(duty);
	Set_Motor_PWM();
}

void Direct_acr(void)
{

}

float Speed(char LR, float Length)
{
	static char comfirm=0;
	float r=0;
	float prc=0;
	float res;
	float rest;
	if (comfirm == LR)
	{}
	else{
		comfirm = LR;
		speed.error = speed.error_L = speed.error_L_L = 0;
		speed.error_T = speed.error_T_L = speed.error_T_L_L = 0;
		}
	if (LR == 1)//ÓÒ×ª
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;

		r = 40.0f+Length;
		prc = (r+20.0f)/r;
	  speed.error = prc*Speed_R - Speed_L;
				if (speed.error < 4)
		{
			speed.error=0;
		}
		res = speed.P*speed.error+speed.D*(speed.error - speed.error_L)+speed.ID*(speed.error - 2*speed.error_L+speed.error_L_L); 
		
		Motor_PWM.left_pwm1+=res;
		
		speed.error_T_L_L = speed.error_T_L;
		speed.error_T_L = speed.error_T;
		speed.error_T = MSPDR-Speed_R;
		rest = speed.P*speed.error_T+speed.D*(speed.error_T - speed.error_T_L)+speed.ID*(speed.error_T - 2*speed.error_T_L+speed.error_T_L_L);
		
		Motor_PWM.right_pwm1+=rest;
	}
	else if(LR == 2)
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;
		
		r = 40.0f+Length;
		prc = (r+20.0f)/r;
	  speed.error = prc*Speed_L - Speed_R;
		if (speed.error < 4)
		{
			speed.error=0;
		}
		res = speed.P*speed.error_T+speed.D*(speed.error_T - speed.error_T_L)+speed.ID*(speed.error_T - 2*speed.error_T_L+speed.error_T_L_L);
		
		Motor_PWM.right_pwm1+=res; 
		
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;
		speed.error = MSPDL-Speed_L;
		rest = speed.P*speed.error+speed.D*(speed.error - speed.error_L)+speed.ID*(speed.error - 2*speed.error_L+speed.error_L_L);
		
		Motor_PWM.left_pwm1+=rest;
	}
	else
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;
		speed.error = MSPDL-Speed_L;
		speed.error_T_L_L = speed.error_T_L;
		speed.error_T_L = speed.error_T;
		speed.error_T = MSPDR-Speed_R;
		res = speed.P*speed.error_T+speed.D*(speed.error_T - speed.error_T_L)+speed.ID*(speed.error_T - 2*speed.error_T_L+speed.error_T_L_L);
		rest = speed.P*speed.error+speed.D*(speed.error - speed.error_L)+speed.ID*(speed.error - 2*speed.error_L+speed.error_L_L);
		Motor_PWM.right_pwm1+=res;
		Motor_PWM.left_pwm1+=rest;
	}
	//printf ("LS=%d RS=%d LP=%f RP=%f\n", Speed_L,Speed_R,rest,res);
}

uint8 Roundabout(char check ,char dire)
{
	static char status=0;
	static char start=0;
	static char ROL;
	float res;
	float rest;
	uint8 duty;
	RoundAb.error_L_L=RoundAb.error_L;
	RoundAb.error_L = RoundAb.error;


	if (check)
	{
		if(status!=2)
			status = 1;
	}
	if (status == 1 && dire &&ADC_Data.L2<MIDMAX)
		start=1;
	else if(status == 1 && (!dire) &&ADC_Data.R2<MIDMAX)
		start=1;

	if (status == 1 && start==1)
	{
		printf("use\n");
		if(ADC_Data.L2>ADC_Data.L1)
		ADC_Data.L1 = ADC_Data.L2;

			
		if(ADC_Data.R2>ADC_Data.R1)
		ADC_Data.R1 = ADC_Data.R2;
		
		if(ADC_Data.L2<1.4f*MIDMAX&&dire)
		{	
			
			if(ADC_Data.L1>MIDMAX)
			{
				ADC_Data.L1=MIDMAX;
			}
			ADC_Data.R1=0;
			if(ADC_Data.MID>ADC_Data.L1)
			ADC_Data.MID = sqrtf((float)MIDMAX*MIDMAX-ADC_Data.L1*ADC_Data.L1);
			
			return 0;
		}
		else 	if(ADC_Data.R2<1.4f*MIDMAX&&!dire)
		{	
			if(ADC_Data.R1>MIDMAX)
			{
				ADC_Data.R1=MIDMAX;
			}
			ADC_Data.L1=1;
			if(ADC_Data.MID>ADC_Data.R1)
			ADC_Data.MID = sqrtf((float)MIDMAX*MIDMAX-ADC_Data.R1*ADC_Data.R1);
			
			return 0;
		}
		else status =2;
	}
	if (status==2)
	{
		if(ADC_Data.MID<MIDMAX && ADC_Data.L2 < MIDMAX&&ADC_Data.R2<MIDMAX)
		{
			status = 0;
			start = 0;
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

char RAcheck(char f1)
{
	static char up_side=0;
	if(up_side == f1)
	{return 0;}
	else
	{
		up_side = f1;
		if(f1==1)
		{
		printf("check it up\n");
		}
		else
		{
		printf("check it down\n");
		}
		return 1;

	}
}
