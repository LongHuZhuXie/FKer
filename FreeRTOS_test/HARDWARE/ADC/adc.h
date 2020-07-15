/*********************************************************************************************************************
 * @file       	adc.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note				电磁头			MCU
 *							最右				ADC_RIGHT1
 *							中靠右			ADC_RIGHT2
 *							中间				ADC_MID
 *							中靠左			ADC_LEFT2
 *							最左				ADC_LEFT1
 ********************************************************************************************************************/

#ifndef __ADC_H
#define __ADC_H

#include "common.h"

//#define ADC_RIGHT1	ADC1_SE23
#define ADC_RIGHT2		ADC0_SE16
#define ADC_MID			ADC0_SE17
//#define ADC_LEFT2		ADC0_SE23
#define ADC_LEFT1		ADC0_SE18


typedef volatile struct __ADC_Data_Type
{
	uint16 L1;
//	uint16 L2;
	uint16 MID;
	uint16 R1;
//	uint16 R2;
} __ADC_Data;

extern __ADC_Data ADC_Data;

void ADC_Init(void);
void Get_ADC_Data(void);

#endif
