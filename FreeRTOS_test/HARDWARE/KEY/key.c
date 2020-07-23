/*********************************************************************************************************************
 * @file       	key.c
 * @version    	V1.0
 * @Software 	IAR 8.3 or MDK 5.24
 * @TargetCore	MK66FX
 * @date       	2019-12-13
 * @note		KEY				Pin
 *				Key_Left		A6
 *				Key_Right		A9
 *				Key_Up			A19
 *				Key_Dowm		A11
 *				Key_Mid			A10
 ********************************************************************************************************************/
 
#include "key.h"
#include "MK60_gpio.h"

#define UP			1
#define DOWN		2
#define LEFT		3
#define RIGHT		4
#define MID			5

void KEY_Init(void)
{
	gpio_init(Key_Left,GPI,1);
	gpio_init(Key_Right,GPI,1);
	gpio_init(Key_Up,GPI,1);
	gpio_init(Key_Down,GPI,1);
	gpio_init(Key_Mid,GPI,1);
}

uint8_t KEY_Scan(void)
{
	if(!gpio_get(Key_Up))
	{
		while(!gpio_get(Key_Up));
		return UP;
	}
	else if(!gpio_get(Key_Down))
	{
		while(!gpio_get(Key_Down));
		return DOWN;
	}
	else if(!gpio_get(Key_Left))
	{
		while(!gpio_get(Key_Left));
		return LEFT;
	}
	else if(!gpio_get(Key_Right))
	{
		while(!gpio_get(Key_Right));
		return RIGHT;
	}
	else if(!gpio_get(Key_Mid))
	{
		while(!gpio_get(Key_Mid));
		return MID;
	}
	else 													return 0;
}

/*--------------------END--------------------*/



























