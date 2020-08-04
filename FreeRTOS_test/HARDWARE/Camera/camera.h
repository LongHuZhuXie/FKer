/*********************************************************************************************************************
 * @file       	camera.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note				Camera								PIN
 *							八位数据(D0-D7)				PE0-PE7
 *							像素中断(PCLK)				PE9
 *							场中断(VSY)						PE11
 *							行中断(HREF)					PE10
 *							51单片机TX						PC16
 *							51单片机RX						PC17
 ********************************************************************************************************************/

#ifndef __CAMERA_H
#define __CAMERA_H

#include "common.h"
#include "MK60_uart.h"

#define COL     184//图像宽度 范围1-752     K60采集不允许超过188
#define ROW     50//图像高度	范围1-480
//#define THRESHOLD 120
extern uint8 THRESHOLD;
extern uint8 Left_Add[50];
extern uint8 Right_Add[50];
extern uint8 width[50];
extern uint8 stop_flag;
extern uint8 Mid_Count;
extern uint8 Right_Line[50];
extern uint8 Mid_Line[50];
extern uint8 width_Min;



/* 摄像头命令枚举 */
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数
    
    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令
    
    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;

extern uint8 Image_Finish_Flag;  //一场图像采集完成标志位
extern uint8 image[ROW][COL];      //内部使用
extern uint8 Left_Line[50];




void MT9V032_Uart_Receive(void);
void VSYNC(void);
void Send_Image(void);
void Row_Finished(void);
void Camera_Init(void);
void Image_Binary();
void Image_recontract();
void Traversal_Mid_Line(uint8 i, uint8 *data);
void First_linecope(uint8 * data);
void iteration_Thresholdfigure();
void Mid_Filtering(uint8 *data);		//中线滤波

#endif
