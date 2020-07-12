/*********************************************************************************************************************
 * @file       	decode.c
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	M66FX1M0VLQ18
 * @date       	2019-12-24
 * @note		
 ********************************************************************************************************************/

#include "decode.h"
#include "MK60_ftm.h"

volatile int16 Speed_L = 0, Speed_R = 0;

void Decode_Init(void)
{
	ftm_quad_init(ftm1);//初始化ftm1为正交解码	引脚对应查看MK60DN10_port_cfg.h
	ftm_quad_init(ftm2);//初始化ftm2为正交解码	引脚对应查看MK60DN10_port_cfg.h
}


void Get_Decode_Data(void)
{
//	Speed_L = ftm_quad_get(ftm2);
//	Speed_R = ftm_quad_get(ftm1);
//	ftm_quad_clean(ftm1);		//清除ftm1正交解码的值
//	ftm_quad_clean(ftm2);		//清除ftm2正交解码的值
	Speed_L = FTM2->CNT;
	Speed_R = -(FTM1->CNT);
	FTM1->CNT = 0;
	FTM2->CNT = 0;
}

