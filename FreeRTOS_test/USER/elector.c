#include "elector.h"
#include "MyTask.h"
#define MIDMAX 4080
#define V_HL 239
#define V_HR 239
volatile Pid_struct direction;
volatile Pid_struct speed;
volatile Pid_struct RoundAb;
char road_flag=0;
int MSPDR =V_HL;
int MSPDL =V_HR;
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
	static float trh = 1.7f;
	float angle_error ;
	float contrast1;
	float contrast2;
	float rescontrast;
	

	
		if (ADC_Data.L1 < 300 && ADC_Data.MID < 300 && ADC_Data.R1<300){}
		else
		{
			Motor_PWM.left_pwm1 = MSPDL;
			Motor_PWM.right_pwm1 = MSPDR;
		}
		if((trh*MIDMAX < ADC_Data.MID && rab_L==0&&ADC_Data.LV>1.3f*MIDMAX&&ADC_Data.RV>MIDMAX*0.9f)&&ADC_Data.LV-ADC_Data.RV>0.7f*MIDMAX)
	{
		checkab = RAcheck(1);
		//printf("check\n");
		if(checkab==1){checkab_up = checkab;trh=1.6;}
	}
		else if((trh*MIDMAX < ADC_Data.MID && rab_L==0&&ADC_Data.LV>0.9f*MIDMAX&&ADC_Data.RV>MIDMAX*1.3f)&&ADC_Data.RV-ADC_Data.LV>0.7f*MIDMAX)
	{
		checkab = RAcheck(1);
		//printf("check\n");
		if(checkab==1){checkab_up = checkab;trh=1.6;}
	}
	else
	{
		checkab = RAcheck(0);
		if(checkab&&checkab_up)checkab_down= checkab;
	}
	if(ADC_Data.MID>MIDMAX)
	{
		ADC_Data.MID = MIDMAX;
	}
	if (checkab_up==1&&checkab_down==1)
	{
		start=1;
		checkab_up=0;
		checkab_down=0;
		times=0;
		trh=1.7;
		if(ADC_Data.LV>ADC_Data.RV)
		{
			dire=1;//左转
		}
		else dire=0;
	}
	if (ADC_Data.MIDV<0.2f*MIDMAX&&!rab_L)
	{
		//over_straith=1;
	}
	rab_L=Roundabout(start,dire);

	angle = atanf(1.2f*ADC_Data.MID/ADC_Data.MIDV);
	//angle_L = atanf(1.2f*ADC_Data.L1/ADC_Data.LV);
	//angle_R = atanf(1.2f*ADC_Data.R1/ADC_Data.RV);
	angle = 180.0f*angle/3.14159f;
	angle_error = 88.8f- angle;
	//printf("angle=%f\n  ,",angle);
	//printf("%f\n", contrast);
	contrast1 = (1.0f*ADC_Data.L1 -ADC_Data.MID)/((float)ADC_Data.L1+ADC_Data.MID)-(1.0f*ADC_Data.R1 -ADC_Data.MID)/((float)ADC_Data.R1+ADC_Data.MID);
	contrast2 = (1.0f*ADC_Data.L2 -ADC_Data.MID)/((float)ADC_Data.L2+ADC_Data.MID)-(1.0f*ADC_Data.R2 -ADC_Data.MID)/((float)ADC_Data.R2+ADC_Data.MID);	
	if(ADC_Data.L2 > ADC_Data.R2 && ADC_Data.L1 >300)
	{
//			if((ADC_Data.MID<MIDMAX*0.9&&ADC_Data.L1<ADC_Data.L2)||ADC_Data.L2>0.5*MIDMAX)
//		{
//			ADC_Data.L1 = ADC_Data.L2;
//		}
		if (contrast1>contrast2)
			rescontrast = 7*contrast1;
		else
			rescontrast = 7*contrast2;
		R_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.R1);
		LengthR = (12.0f*R_pce);
		L_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.L1);
		LengthL = (12.0f*L_pce);
		
		printf ("c1 = %.3f c2 = %.3f L = %.3f\n",7*contrast1, 7*contrast2, LengthL);
		printf ("rc = %.3f\n",rescontrast);
		direction.error_L_L = direction.error_L;
		direction.error_L = direction.error;
		//direction.error = LengthL;
		direction.error = rescontrast;
		
			direction.P = 0.158f*direction.error+0.525+0.002f*Speed_L;
			direction.D = 0.34*direction.P;
	if (ADC_Data.MID>0.7f*MIDMAX&&ADC_Data.MIDV>0.5f*MIDMAX)direction.error=0;
//		}
	}
	else if(ADC_Data.L2 < ADC_Data.R2 && ADC_Data.R1>300)
	{
//			if((ADC_Data.MID<MIDMAX*0.9&&ADC_Data.R1<ADC_Data.R2)||ADC_Data.R2>0.5*MIDMAX)
//		{
//			ADC_Data.R1 = ADC_Data.R2;
//		}
		if (contrast1<contrast2)
			rescontrast = 7*contrast1;
		else
			rescontrast = 7*contrast2;
		L_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.L1);
		LengthL = (12.0f*L_pce);
		R_pce = (1.0f/ADC_Data.MID)/(1.0f/ADC_Data.MID+1.0f/ADC_Data.R1);
		LengthR = (12.0f*R_pce);
		printf ("c1 = %.3f c2 = %.3f R = %.3f\n",7*contrast1, 7*contrast2, -LengthR);
		printf ("rc = %.3f\n",rescontrast);
		direction.error_L_L = direction.error_L;
		direction.error_L = direction.error;
		//direction.error = -LengthR;
			direction.error = rescontrast;
			//direction.P = 0.158f*(-direction.error)+0.525+0.002f*Speed_R;
			direction.P = 0.158f*(-direction.error)+0.525+0.002f*Speed_R;
			direction.D = 0.34*direction.P;

		if (ADC_Data.MID>0.7f*MIDMAX&&ADC_Data.MIDV>0.5f*MIDMAX)direction.error=0;
	}

	//if (angle > 84 && (direction.error<3.0&&direction.error>-3.0))direction.error=0;
	
	//printf("error= %f\n",direction.error);
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
		if (direction.error<150 && Speed_L>= MSPDL)
	{
		Speed(2, duty);//右转
	}
	else if (direction.error>150 && Speed_R >= MSPDR)
	{
		Speed(1, duty);//左转
	}
	else
	{
		Speed(0, angle);
	}
	printf("duty=%d\n", duty);
	if (checkab_up&&Speed_L>150&&Speed_R>150)
	{
		Motor_PWM.left_pwm1=-1.5f*V_HL;
		Motor_PWM.right_pwm1=-1.5f*V_HR;
	}
	else
	{
		MSPDR = V_HL;
		MSPDL = V_HR;
	}
	Set_Steering_PWM(duty);
	//printf("duty=%d\n",duty);
	Set_Motor_PWM();
}



float Speed(char LR, uint8 duty)
{
	static char comfirm=0;
	float angle=0;
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
	if (LR == 1)//左转
	{
		speed.error_L_L = speed.error_L;
		speed.error_L = speed.error;

//		r = 40.0f+Length;
//		prc = (r+20.0f)/r;
		angle = 0.110*(duty-150);
	  //speed.error = prc*Speed_R - Speed_L;
		speed.error = ((Speed_L+Speed_R)*155.0f*sin(angle/180.0*PI))/(2.0f*200.0f);

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
		angle = 0.110*(150-duty);
		speed.error_T_L_L = speed.error_T_L;
		speed.error_T_L = speed.error_T;
		
//		r = 40.0f+Length;
//		prc = (r+20.0f)/r;
	  //speed.error_T = prc*Speed_L - Speed_R;
		speed.error_T = ((Speed_L+Speed_R)*155.0f*sin(angle/180.0*PI))/(2.0f*200.0f);
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
	static char times=0;
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
	if (status == 1 && dire &&ADC_Data.L2>0.8*MIDMAX)
		start=1;
	else if(status == 1 && (!dire) &&ADC_Data.R2>0.8*MIDMAX)
		start=1;
	
		if (status==2)
	{
		if(ADC_Data.LV<MIDMAX*0.9f&&ADC_Data.RV<MIDMAX*0.9f)
		{
			printf("finish roundab\n");
			status = 0;
			start = 0;
			times = 0;
			Buzzer_OFF();
			return 0;
		}
		else
		{
			Buzzer_ON();
			return 1;
		}
	}
	
	if (status == 1 && start==1)
	{
		//printf("use\n");

			
		
		if(times!=6&&dire)
		{	
			if (ADC_Data.LV < MIDMAX)
				times++;
			Buzzer_ON();
				if (Speed_L>150)
		{
			Motor_PWM.left_pwm1=-1.8*V_HL;
			Motor_PWM.right_pwm1=-1.8*V_HR;
		}
			if(ADC_Data.L2>ADC_Data.L1)
			ADC_Data.L1 = ADC_Data.L2;
			
			if(ADC_Data.L1>MIDMAX)
			{
				ADC_Data.L1=MIDMAX;
			}
			//ADC_Data.R1=0;
			ADC_Data.R1=ADC_Data.R2;
			if(ADC_Data.LV>MIDMAX*0.5f)
			ADC_Data.MID = sqrtf((float)MIDMAX*MIDMAX-ADC_Data.L1*ADC_Data.L1);
			
			return 1;
		}
		else 	if(times!=6&&!dire)
		{	
				if (Speed_R>150)
		{
			Motor_PWM.left_pwm1=-1.8*V_HL;
			Motor_PWM.right_pwm1=-1.8*V_HR;
		}
			Buzzer_ON();
			if (ADC_Data.RV < MIDMAX)
				times++;
			if(ADC_Data.R2>ADC_Data.R1)
			ADC_Data.R1 = ADC_Data.R2;
			if(ADC_Data.R1>MIDMAX)
			{
				ADC_Data.R1=MIDMAX;
			}
		//	ADC_Data.L1=1;
			ADC_Data.L1 = ADC_Data.L2;
			if(ADC_Data.RV>MIDMAX*0.8f)
			ADC_Data.MID = sqrtf((float)MIDMAX*MIDMAX-ADC_Data.R1*ADC_Data.R1);
			
			return 1;
		}
		else status =2;
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

void M_start()
{
	if (ADC_Data.L2 < 0.2*MIDMAX)
	{
		Motor_PWM.left_pwm1=190;
		Motor_PWM.right_pwm1=190;
		Set_Steering_PWM(156);
		Set_Motor_PWM();
	}
	else
	{
		road_flag=1;
	}
}
	
