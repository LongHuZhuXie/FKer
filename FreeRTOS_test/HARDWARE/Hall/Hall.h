#ifndef __HALL_H
#define __HALL_H

#include "common.h"

#define Hall_Port	GPIOA
#define Hall_Pin 	A24

void Hall_Init(void);
bool Hall_Scan(void);

#endif
