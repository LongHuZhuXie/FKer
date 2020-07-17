/*********************************************************************************************************************
 * @file       		display.c
 * @brief      		OLEDÏÔÊ¾º¯Êý
 * @version    		V1.0
 * @Software 		IAR 8.3 or MDK 5.26
 * @TargetCore		M66FX1M0VLQ18
 * @date       		2019-12-13
 ********************************************************************************************************************/

#include "display.h"
#include "motor.h"
#include "decode.h"
#include "adc.h"
#include "elector.h"
#include "camera.h"

#define ADC_MOD   1
//#define SPID_MOD  2
extern char Version_Data[5];

void OLED_Display(void)
{
	char Dis_Buf[30];
	
	OLED_P6x8Str( 20,  0, (uint8 *)"M66FX1M0VLQ18");
	sprintf(Dis_Buf,"Version:   V2.0");
	OLED_P6x8Str( 0,  1, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Core Frequence:%dMHz",(int)core_clk_mhz);
	OLED_P6x8Str( 0,  2, (uint8 *)Dis_Buf);
	
	#if ADC_MOD
	sprintf(Dis_Buf,"ADC_LEFT:   %d   ",ADC_Data.L1);
	OLED_P6x8Str( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"ADC_MID:    %d   ",ADC_Data.MID);
	OLED_P6x8Str( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"ADC_RIGHT:  %d   ",ADC_Data.R1);
	OLED_P6x8Str( 0,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Left  Speed:  %d   ",Speed_L);
	OLED_P6x8Str( 0,  6, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Speed:  %d   ",Speed_R);
	OLED_P6x8Str( 0,  7, (uint8 *)Dis_Buf);
	#elif SPID_MOD 
	sprintf(Dis_Buf,"SP: %f ",direction.P);
	OLED_P6x8Str( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"SI: %f ",direction.I);
	OLED_P6x8Str( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"SD: %f ",direction.D);
	OLED_P6x8Str( 0,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"ST: %d ",THRESHOLD);
	OLED_P6x8Str( 0,  6, (uint8 *)Dis_Buf);
	#else
	sprintf(Dis_Buf,"Left  Duty:   %.1f%% ",Motor_PWM.left_pwm1/10.0f);
	OLED_P6x8Str( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Duty:   %.1f%% ",Motor_PWM.right_pwm1/10.0f);
	OLED_P6x8Str( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Left  Speed:  %d   ",Speed_L);
	OLED_P6x8Str( 0,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Speed:  %d   ",Speed_R);
	OLED_P6x8Str( 0,  6, (uint8 *)Dis_Buf);
	#endif
}



