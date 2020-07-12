/*********************************************************************************************************************
 * @file       	main.c
 * @version    	V3.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-24
 * @note				Based on FreeRTOS, with FPU and DSP Functions
 ********************************************************************************************************************/

/*System Liberary*/
#include "common.h"
#include "misc.h"
#include "isr.h"

/* Task System */
#include "MyTask.h"

/* Main Programme */
int main(void)
{
	/* NVIC Configue */
	NVIC_SetPriorityGrouping(3);
	/* Get System Clock */
	get_clk();
	/* Create Task List */
	Task_List_Init();
	/* Statr Task Schedule */
	vTaskStartScheduler();
}

/*--------------------END--------------------*/



















