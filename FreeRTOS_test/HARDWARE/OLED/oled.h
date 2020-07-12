/*********************************************************************************************************************
 * @file       		oled.h
 * @brief      		0.96OLED函数库
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.26
 * @TargetCore		M66FX1M0VLQ18
 * @date       		2019-12-13
 * @note	
 *					OLED接线定义：
 *					------------------------------------
 *					SPI:
 *					OLED	    	MCU
 *   				D0          D12
 *    			D1          D13
 *    			RES         D14
 *   				DC          D15
 *					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _OELD_H
#define _OELD_H

#include "common.h"

/* OLED引脚定义 */	 
#define  OLED_SCL_PIN	D12
#define  OLED_SDA_PIN	D13
#define  OLED_RST_PIN	D14
#define  OLED_DC_PIN	D15
#define  OLED_CS_PIN	D10


void Dly_ms(uint16 ms);
void OLED_Init(void);
void OLED_Fill(uint8 dat);
void OLED_WrDat(uint8 data);
void OLED_Wr6Dat(uint8 data);
void OLED_PutPixel(uint8 x,uint8 y);
void OLED_ClrPixel(uint8 x,uint8 y);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_HEXACSII(uint16 hex,uint8* Print);
void OLED_Print_Num(uint8 x, uint8 y, uint16 num);
void OLED_Print_Num1(uint8 x, uint8 y, int16 num);
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);

#endif
