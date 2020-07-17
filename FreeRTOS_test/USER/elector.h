#ifndef _ELECTOR_H
#define _ELECTOR_H

#include "adc.h"
#include "steering.h"
#include "motor.h"
#include "stdio.h"
#include "decode.h"

/* DSP Liberary */
#include "arm_math.h"

typedef struct SPE
{
	float P;
	float I;
	float D;
	float ID;
	float error;
	float error_L;
	float error_L_L;
}Pid_struct;

extern volatile Pid_struct direction;
extern volatile Pid_struct speed;
void Direct_acr(void);
void D_PID_initial(float P , float I, float D);
void M_PID_initial(float P , float I, float D);
float Speed(char LR, float Length);
void Direct(void);
#endif


