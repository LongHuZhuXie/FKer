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
#include "oled.h"
#include "SEEKFREE_18TFT.h"

#define ADC_MOD   1
//#define SPID_MOD  2
extern char Version_Data[5];

void OLED_Display(void)
{
	char Dis_Buf[30];

	lcd_showstr( 12,  0, (uint8 *)"MotherHunters");
	lcd_showstr( 12,  1, (uint8 *)"Version: V2.0");
	OLED_ShowString( 20,  -1, (uint8 *)"MotherHunters");
	sprintf(Dis_Buf,"Batter: %.1f%V", (float)ADC_Data.Batter*2.81*3.3/4096);
	OLED_ShowString( 0,  0, (uint8 *)Dis_Buf);
	lcd_showstr( 12,  2, (uint8 *)Dis_Buf);
	
	#if ADC_MOD
	sprintf(Dis_Buf,"L: %5d %5d", ADC_Data.L1, ADC_Data.L2);
	OLED_ShowString( 0,  1, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"M: %5d %5d", ADC_Data.MID, ADC_Data.MIDV);
	OLED_ShowString( 0,  2, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"R: %5d %5d", ADC_Data.R1, ADC_Data.R2);
	OLED_ShowString( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Left  Speed: %3d",Speed_L);
	OLED_ShowString( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Speed: %3d",Speed_R);
	OLED_ShowString( 0,  5, (uint8 *)Dis_Buf);
	
	sprintf(Dis_Buf,"L: %5d %5d", ADC_Data.L1, ADC_Data.L2);
	lcd_showstr( 2,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"M: %5d %5d", ADC_Data.MID, ADC_Data.MIDV);
	lcd_showstr( 2,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"R: %5d %5d", ADC_Data.R1, ADC_Data.R2);
	lcd_showstr( 2,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Left  Speed: %3d",Speed_L);
	lcd_showstr( 2,  6, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Speed: %3d",Speed_R);
	lcd_showstr( 2,  7, (uint8 *)Dis_Buf);
	#elif SPID_MOD 
	sprintf(Dis_Buf,"SP: %f ",direction.P);
	OLED_ShowStringOLED_ShowString( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"SI: %f ",direction.I);
	OLED_ShowStringOLED_ShowString( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"SD: %f ",direction.D);
	OLED_ShowStringOLED_ShowString( 0,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"ST: %d ",THRESHOLD);
	OLED_ShowStringOLED_ShowString( 0,  6, (uint8 *)Dis_Buf);
	#else
	sprintf(Dis_Buf,"Left  Duty:   %.1f%% ",Motor_PWM.left_pwm1/10.0f);
	OLED_ShowString( 0,  3, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Duty:   %.1f%% ",Motor_PWM.right_pwm1/10.0f);
	OLED_ShowString( 0,  4, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Left  Speed:  %d   ",Speed_L);
	OLED_ShowString( 0,  5, (uint8 *)Dis_Buf);
	sprintf(Dis_Buf,"Right Speed:  %d   ",Speed_R);
	OLED_ShowString( 0,  6, (uint8 *)Dis_Buf);
	#endif
}



