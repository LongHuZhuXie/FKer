#ifndef _ELECTOR_H
#define _ELECTOR_H

#include "adc.h"
#include "steering.h"
#include "motor.h"
#include "stdio.h"
#include "decode.h"

/* DSP Liberary */
#include "arm_math.h"
#include "buzzer.h"
typedef struct SPE
{
	float P;
	float I;
	float D;
	float ID;
	float error;
	float error_L;
	float error_L_L;
	float error_T;
	float error_T_L;
	float error_T_L_L;
}Pid_struct;
extern char road_flag;
extern volatile Pid_struct direction;
extern volatile Pid_struct speed;
extern volatile Pid_struct RoundAb;
void D_PID_initial(float P , float I, float D);
void M_PID_initial(float P , float I, float D);
float Speed(char LR, uint8 duty);
void Direct(void);
uint8 Roundabout(char check ,char dire);
char RAcheck(char f1);
void M_start();
#endif


