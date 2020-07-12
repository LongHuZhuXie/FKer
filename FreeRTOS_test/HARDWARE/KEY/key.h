/*********************************************************************************************************************
 * @file       		key.h
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					KEY						Pin
 *								Key_Left			PA6
 *								Key_Right			PA9
 *								Key_Up				PA7
 *								Key_Dowm			PA11
 *								Key_Mid				PA10
 ********************************************************************************************************************/

#ifndef __KEY_H
#define __KEY_H

#include "common.h"

#define Key_Left			A7
#define Key_Right			A11
#define Key_Up				A9
#define Key_Down			A6
#define Key_Mid				A10

void KEY_Init(void);
uint8_t KEY_Scan(void);

#endif

/*--------------------END--------------------*/



































