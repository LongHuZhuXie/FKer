/*********************************************************************************************************************
 * @file       		misc.c
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 ********************************************************************************************************************/

#include "misc.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置中断向量表地址
//  @param      vtor            新的中断向量表地址
//  @return     void
//  @since      v1.0
//  Sample usage:               write_vtor(0x1fff0000);;  
//-------------------------------------------------------------------------------------------------------------------
void write_vtor(int vtor)
{
    // Write the VTOR with the new value
    SCB->VTOR = vtor;
}

/**
  * @brief  Configures the SysTick clock source.
  * @param  SysTick_CLKSource: specifies the SysTick clock source.
  *   This parameter can be one of the following values:
  *     @arg SysTick_CLKSource_HCLK_Div4: system clock divided by 4 selected as SysTick clock source.
  *     @arg SysTick_CLKSource_HCLK: system clock selected as SysTick clock source.
  * @retval None
  */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* Check the parameters */
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div4;
  }
}
