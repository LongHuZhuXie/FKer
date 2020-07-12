/*********************************************************************************************************************
 * @file       		MK60_systick.h
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 ********************************************************************************************************************/
 
 

#ifndef _MK60_systick_h
#define _MK60_systick_h


#include "MK60_gpio.h"
#include "common.h"

void	systick_delay(uint32 time);
void	systick_delay_ms(uint32 ms);
void	systick_timing(uint32 time);
uint32	systick_getval(void);


#endif
