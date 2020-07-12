/*********************************************************************************************************************
 * @file       		delay.c
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					×´Ì¬»ú
 ********************************************************************************************************************/

#include "delay.h"

void Delay_us(uint32_t us)
{
	uint32_t now;
	
	now = Getsystime_us();
	while (Getsystime_us() - now < us);
}

void Delay_ms(uint32_t ms)
{
	while (ms--)
		Delay_us(1000);
}
