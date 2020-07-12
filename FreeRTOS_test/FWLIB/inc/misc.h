/*********************************************************************************************************************
 * @file       		misc.h
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 ********************************************************************************************************************/


#ifndef _misc_h
#define _misc_h

#include "common.h"

/** @defgroup SysTick_clock_source 
  * @{
  */

#define SysTick_CLKSource_HCLK_Div4    ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)
#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SysTick_CLKSource_HCLK) || \
                                       ((SOURCE) == SysTick_CLKSource_HCLK_Div4)

#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级，优先级范围0-15


#define EnableInterrupts                __enable_irq()              //使能全部中断
#define DisableInterrupts               __disable_irq()             //禁止全部中断

void write_vtor(int vtor);                                          //设置中断向量表的地址
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);						//设置系统时钟分频

#endif
