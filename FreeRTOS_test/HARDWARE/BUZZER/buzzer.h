/*********************************************************************************************************************
 * @file       	buzzer.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-23
 * @note				Buzzer		PD11
 ********************************************************************************************************************/

#ifndef __BUZZER_H
#define __BUZZER_H

#define Buzzer_Pin	D11

/************************************************************************
 *	@brief	��������ʼ��
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Buzzer_Init(void);

/************************************************************************
 *	@brief	�򿪷�����
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Buzzer_ON(void);

/************************************************************************
 *	@brief	�رշ�����
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Buzzer_OFF(void);

#endif

/*--------------------END--------------------*/

























