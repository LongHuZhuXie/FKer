/*********************************************************************************************************************
 * @file       		common.c
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 ********************************************************************************************************************/

#include "common.h"

#define EX_REF_CLK  			 50 	//定义外部参考时钟为16MHz

uint32 	mcgout_clk_mhz	= 180;	//默认MCG输出频率180MHz
uint32 		core_clk_mhz 	= 180;	//默认核心工作频率180MHz
uint32 		 bus_clk_mhz 	=  90;	//默认外设总线频率180MHz

void get_clk(void)
{
	/* MCG输出频率 = 晶振频率 × 3.6(36/5/2) */
	mcgout_clk_mhz 	= EX_REF_CLK * ((MCG->C6 & MCG_C6_VDIV0_MASK) + 16) / ((MCG->C5 & MCG_C5_PRDIV0_MASK) + 1)/2;
	/* 核心频率 = MCG输出频率(1分频) */
	core_clk_mhz 	= mcgout_clk_mhz / ((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);
	/* 核心频率 = MCG输出频率/2(2分频) */
	bus_clk_mhz 	= mcgout_clk_mhz / (((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV2_SHIFT) & 0x0f) + 1);
}
