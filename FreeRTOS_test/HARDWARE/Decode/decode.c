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
	ftm_quad_init(ftm1);//��ʼ��ftm1Ϊ��������	���Ŷ�Ӧ�鿴MK60DN10_port_cfg.h
	ftm_quad_init(ftm2);//��ʼ��ftm2Ϊ��������	���Ŷ�Ӧ�鿴MK60DN10_port_cfg.h
}


void Get_Decode_Data(void)
{
//	Speed_L = ftm_quad_get(ftm2);
//	Speed_R = ftm_quad_get(ftm1);
//	ftm_quad_clean(ftm1);		//���ftm1���������ֵ
//	ftm_quad_clean(ftm2);		//���ftm2���������ֵ
	Speed_L = FTM2->CNT;
	Speed_R = -(FTM1->CNT);
	FTM1->CNT = 0;
	FTM2->CNT = 0;
}

