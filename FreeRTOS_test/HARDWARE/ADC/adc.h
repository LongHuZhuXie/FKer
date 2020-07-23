/*********************************************************************************************************************
 * @file       	adc.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note		电磁头			MCU
 *				右横			ADC_RIGHT1
 *				右竖			ADC_RIGHT2
 *				中间横			ADC_MID
 *				中间竖			ADC_MID
 *				左横			ADC_LEFT1
 *				左竖			ADC_LEFT2
 * 电池1:3
 ********************************************************************************************************************/

#ifndef __ADC_H
#define __ADC_H

#include "common.h"

#define ADC_RIGHT1		ADC0_SE17
#define ADC_RIGHT2		ADC0_SE14
#define ADC_MID			ADC0_SE18
#define ADC_MIDV		ADC0_SE9
#define ADC_LEFT1		ADC0_SE15
#define ADC_LEFT2		ADC0_SE8
//#define ADC_BaseH		ADC0_SE15
//#define ADC_BaseV		ADC0_SE8
#define ADC_BATTER		ADC0_SE11


typedef volatile struct __ADC_Data_Type
{
	uint16 L1;
	uint16 L2;
	uint16 MID;
	uint16 MIDV;
	uint16 R1;
	uint16 R2;
	uint16 BaseV;
	uint16 BaseH;
	uint16 Batter;
} __ADC_Data;

extern __ADC_Data ADC_Data;

void ADC_Init(void);
void Get_ADC_Data(void);

#endif
