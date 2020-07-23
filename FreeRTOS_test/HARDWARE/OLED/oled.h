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
 *					OLED	    MCU
 *   				D0          A25
 *    				D1          A26
 *    				RES         A28
 *   				DC          A29
 *   				CS          A27
 *					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _OELD_H
#define _OELD_H

#include "common.h"
#include "MK60_gpio.h"

#define SIZE 12
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

/* OLED引脚定义 */	 
#define  OLED_SCL_PIN	A25
#define  OLED_SDA_PIN	A26
#define  OLED_RST_PIN	A28
#define  OLED_DC_PIN	A29
#define  OLED_CS_PIN	A27

//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  	gpio_set(OLED_CS_PIN, 0)
#define OLED_CS_Set()  	gpio_set(OLED_CS_PIN, 1)

#define OLED_RST_Clr() 	gpio_set(OLED_RST_PIN, 0)
#define OLED_RST_Set() 	gpio_set(OLED_RST_PIN, 1)

#define OLED_DC_Clr() 	gpio_set(OLED_DC_PIN, 0)
#define OLED_DC_Set() 	gpio_set(OLED_DC_PIN, 1)

#define OLED_SCLK_Clr() gpio_set(OLED_SCL_PIN, 0)
#define OLED_SCLK_Set() gpio_set(OLED_SCL_PIN, 1)

#define OLED_SDIN_Clr() gpio_set(OLED_SDA_PIN, 0)
#define OLED_SDIN_Set() gpio_set(OLED_SDA_PIN, 1)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif
