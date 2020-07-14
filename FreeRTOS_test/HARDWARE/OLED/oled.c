/*********************************************************************************************************************
 * @file       		oled.c
 * @brief      		0.96OLED函数库
 * @version    		V1.0
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		M66FX1M0VLQ18
 * @date       		2019-12-13
 * @note	
 *					OLED接线定义：
 *					------------------------------------
 *					SPI:
 *					OLED	    MCU
 *   				D0          D12
 *    				D1          D13
 *    				RES         D14
 *   				DC          D15
 *					------------------------------------ 
 ********************************************************************************************************************/

#include "oled.h"
#include "font.h"
#include "MK60_gpio.h"

#define XLevelL			0x00
#define XLevelH			0x10
#define XLevel			((XLevelH&0x0F)*16+XLevelL)
#define Max_Column		128
#define Max_Row			64
#define	Brightness		0xCF
#define X_WIDTH 		128
#define Y_WIDTH 		64

#define OLED_SCL(x)  gpio_set (OLED_SCL_PIN	, x)
#define OLED_SDA(x)  gpio_set (OLED_SDA_PIN	, x)
#define OLED_RST(x)  gpio_set (OLED_RST_PIN	, x)
#define OLED_DC(x)   gpio_set (OLED_DC_PIN	, x)
#define OLED_CS(x)   gpio_set (OLED_CS_PIN  , x)


//内部使用用户无需调用
void OLED_WrDat(uint8 data)
{
	uint8 i=8;
	
	OLED_DC(1);
	OLED_SCL(0);
	while(i--)
	{
	  	if(data&0x80){OLED_SDA(1);}
	  	else{OLED_SDA(0);}
	  	OLED_SCL(1);
		
	  	OLED_SCL(0);
	  	data<<=1;
	}
}

//内部使用用户无需调用
void OLED_Wr6Dat(uint8 data)
{
	uint8 i=6;
	
	OLED_DC(1);
	OLED_SCL(0);
	while(i--)
	{
	 	if(data&0x80){OLED_SDA(1);}
	 	else{OLED_SDA(0);}
	 	OLED_SCL(1);
	 	OLED_SCL(0);
	 	data<<=1;
	}
}

//内部使用用户无需调用
void OLED_WrCmd(uint8 cmd)
{
	uint8 i=8;
	
	OLED_DC(0);
	OLED_SCL(0);;
	
	while(i--)
	{
		if(cmd&0x80){OLED_SDA(1);}
		else{OLED_SDA(0);}
		OLED_SCL(1);
		
		OLED_SCL(0);
		cmd<<=1;
	}
  
}

//内部使用用户无需调用
void OLED_DLY_ms(uint16 ms)
{
	uint16 a;
	while(ms)
	{
	 	a=16000;
	 	while(a--);
	 	ms--;
	}
	return;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示坐标设置
//  @param      x			x轴坐标设置
//  @param      y			y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Set_Pos(uint8 x, uint8 y)
{
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd((x&0x0f)|0x01);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清屏函数
//  @param      bmp_data	填充颜色选着(0x00 or 0xff)
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Fill(uint8 bmp_data)
{
	uint8 y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)	OLED_WrDat(bmp_data); 
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init(void)
{
	gpio_init (OLED_SCL_PIN, GPO,1);
	gpio_init (OLED_SDA_PIN, GPO,0);
	gpio_init (OLED_RST_PIN, GPO,1);
	gpio_init (OLED_DC_PIN , GPO,1);

	gpio_init (OLED_CS_PIN , GPO,0);
	port_init_NoAlt (OLED_SCL_PIN, PULLUP );
	port_init_NoAlt (OLED_SDA_PIN, PULLUP );
	port_init_NoAlt (OLED_RST_PIN, PULLUP );
	port_init_NoAlt (OLED_DC_PIN , PULLUP );
	port_init_NoAlt (OLED_CS_PIN , PULLUP );

	OLED_SCL(1);
	OLED_CS(0); //使能信号端，拉低时正常使用
	OLED_RST(0);
	OLED_DLY_ms(50);
	OLED_RST(1);
	
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏
	OLED_Set_Pos(0,0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED点亮一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_PutPixel(uint8 x,uint8 y)
{
	uint8 data1;  //data1当前点的数据
	
	OLED_Set_Pos(x,y);
	data1 = 0x01<<(y%8);
	OLED_WrCmd(0xb0+(y>>3));
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x00);
	OLED_WrDat(data1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清除一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_ClrPixel(uint8 x,uint8 y)
{
	uint8 data1;  //data1当前点的数据
	
	OLED_Set_Pos(x,y);
	data1 = 0x00<<(y%8);
	OLED_WrCmd(0xb0+(y>>3));
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x00);
	OLED_WrDat(data1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[])
{
	uint8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
	  	c =ch[j]-32;
	  	if(x>126){x=0;y++;}
	  	OLED_Set_Pos(x,y);
	  	for(i=0;i<6;i++)	OLED_WrDat(F6x8[c][i]);
	  	x+=6;
	  	j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[])
{
	uint8 c=0,i=0,j=0;
	
	while (ch[j]!='\0')
	{
	  	c =ch[j]-32;
	  	if(x>120){x=0;y++;}
		
	  	OLED_Set_Pos(x,y);
	  	for(i=0;i<8;i++)	OLED_WrDat(F8X16[c*16+i]);
	  	  
	  	OLED_Set_Pos(x,y+1);
	  	for(i=0;i<8;i++)	OLED_WrDat(F8X16[c*16+i+8]);
	  	  
	  	x+=8;
	  	j++;
	}
}


//内部使用用户无需调用
void OLED_HEXACSII(uint16 hex,uint8* Print)
{
	uint8 hexcheck ;
	uint8 TEMP ;
	TEMP = 6 ;
	Print[TEMP ]='\0';
	while(TEMP)
	{
	  	TEMP -- ;
	  	hexcheck  =  hex%10 ;
	  	hex   /=10 ;
	  	Print[TEMP ]  = hexcheck + 0x30 ;
	}
  
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示无符号数(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      num         无符号数
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Print_Num(uint8 x, uint8 y, uint16 num)
{
	uint8 *ch1,ch[7];
	
	OLED_HEXACSII(num,ch);
	ch1 =&ch[1];
	while(*ch1 != '\0')
	{
		OLED_P8x16Str(x, y, ch1);	//显示数字
	    x += 8;
	   	ch1++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示有符号数(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      num         有符号数
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void OLED_Print_Num1(uint8 x, uint8 y, int16 num)
{
	uint8_t *ch1,ch[7];
	if(num<0)   {num = -num;OLED_P6x8Str(x, y, (uint8 *)"-");}
	else         OLED_P6x8Str(x, y, (uint8 *)" ");
	x+=6;
	OLED_HEXACSII(num,ch);
	ch1 =&ch[1];
	while(*ch1 != '\0')
	{
		OLED_P6x8Str(x, y, ch1);	//显示数字
		x += 6;
		ch1++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示图像
//  @param      high  		图像高度
//  @param      width 		图像宽度
//  @param      *p    		图像地址（数组）
//  @param      value 		二值化阀值
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value)
{
    int16 i,j;
    int16 temp,temp1;
    uint8 dat;
    
    
    temp1 = high%8;
    if(temp1 == 0) temp = high/8;
    else           temp = high/8+1;
    
 
    for(i=0; i<temp; i++)
    {
        OLED_Set_Pos(0,i);
        for(j=0; j<width; j++)
        {
            dat = 0;
            if( i<(temp-1) || !temp1 || temp1>=1)dat |= (*(p+i*8*width+j+width*0) > value? 1: 0)<<0;
            if( i<(temp-1) || !temp1 || temp1>=2)dat |= (*(p+i*8*width+j+width*1) > value? 1: 0)<<1;
            if( i<(temp-1) || !temp1 || temp1>=3)dat |= (*(p+i*8*width+j+width*2) > value? 1: 0)<<2;
            if( i<(temp-1) || !temp1 || temp1>=4)dat |= (*(p+i*8*width+j+width*3) > value? 1: 0)<<3;
            if( i<(temp-1) || !temp1 || temp1>=5)dat |= (*(p+i*8*width+j+width*4) > value? 1: 0)<<4;
            if( i<(temp-1) || !temp1 || temp1>=6)dat |= (*(p+i*8*width+j+width*5) > value? 1: 0)<<5;
            if( i<(temp-1) || !temp1 || temp1>=7)dat |= (*(p+i*8*width+j+width*6) > value? 1: 0)<<6;
            if( i<(temp-1) || !temp1 || temp1>=8)dat |= (*(p+i*8*width+j+width*7) > value? 1: 0)<<7;
           
            OLED_WrDat(dat);
        }
    }
}
