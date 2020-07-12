/*********************************************************************************************************************
 * @file       	usart.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note		
 ********************************************************************************************************************/

#ifndef __USART_H
#define __USART_H

#include "common.h"


/*----NRF Define----*/
#define NRF_TX		C14
#define NRF_RX		C15
#define NRF_RTS		C18
#define NRF_CMD		C19
//////////////////////



void NRF_Init(uint32_t baud);

#endif

/*--------------------END--------------------*/















