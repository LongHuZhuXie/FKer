/*********************************************************************************************************************
 * @file       	camera.h
 * @version			V1.0
 * @Software 		MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-25
 * @note				Camera								PIN
 *							��λ����(D0-D7)				PE0-PE7
 *							�����ж�(PCLK)				PE9
 *							���ж�(VSY)						PE11
 *							���ж�(HREF)					PE10
 *							51��Ƭ��TX						PC16
 *							51��Ƭ��RX						PC17
 ********************************************************************************************************************/

#ifndef __CAMERA_H
#define __CAMERA_H

#include "common.h"
#include "MK60_uart.h"

#define COL     184//ͼ���� ��Χ1-752     K60�ɼ���������188
#define ROW     50//ͼ��߶�	��Χ1-480
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



/* ����ͷ����ö�� */
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
    
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;

extern uint8 Image_Finish_Flag;  //һ��ͼ��ɼ���ɱ�־λ
extern uint8 image[ROW][COL];      //�ڲ�ʹ��
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
void Mid_Filtering(uint8 *data);		//�����˲�

#endif
