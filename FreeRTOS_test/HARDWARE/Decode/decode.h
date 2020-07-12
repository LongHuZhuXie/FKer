/*********************************************************************************************************************
 * @file       	decode.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	M66FX1M0VLQ18
 * @date       	2019-12-24
 * @note		
 ********************************************************************************************************************/

#ifndef __DECODE_H
#define __DECODE_H

#include "common.h"

extern volatile int16 Speed_L, Speed_R;

void Decode_Init(void);
void Get_Decode_Data(void);

#endif
