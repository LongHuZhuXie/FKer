/*********************************************************************************************************************
 * @file       	MyTask.c
 * @version		V1.0
 * @Software 	MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-23
 * @note		��������д�ڴ��ļ���
 ********************************************************************************************************************/
/* Task System */
#include "MyTask.h"

/* C Standard Liberary */
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

/* Peripheral Device */
#include "key.h"
#include "led.h"
#include "display.h"
#include "motor.h"
#include "buzzer.h"
#include "usart.h"
#include "steering.h"
#include "decode.h"
#include "camera.h"
#include "adc.h"
#include "MK60_flash.h"

/* PID */
#include "elector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

/* Init Task */
TaskHandle_t Init_Task_Handler;
void Init_Task(void *pvParameters);

/************************************************************************
 *	@brief	�����ʼ��
 *	@param	pvParameters		���ݸ��������Ĳ���
 *	@return	��
 *	@note	��
 ***********************************************************************/
void Init_Task(void *pvParameters)
{
	/* Peripheral Device Initilization */
	D_PID_initial(0.9,0,0.34);
	M_PID_initial(0.88f,0,0.17f);
	NRF_Init(115200);					//��ʼ�����Դ���
	printf("FlASH��ʼ��������\r\n");
	FLASH_Init();						//��ʼ��AT24C02
	printf("������ʼ��������\r\n");
	KEY_Init();							//��ʼ��KEY
	printf("LED��ʼ��������\r\n");
	LED_Init();							//��ʼ��LED
	printf("OLED��ʼ��������\r\n");
	OLED_Init();						//��ʼ��OLED
	printf("ADC��ʼ��������\r\n");				
	ADC_Init();							//ADC��ʼ��
	printf("�����ʼ��������\r\n");
	Steering_Init();					//��ʼ�����PWM
	printf("�����ʼ��������\r\n");
	Motor_PWM_Init();					//��ʼ��ȫ������SPWM
	printf("��������ʼ��������\r\n");
	Decode_Init();						//��ʼ��������
	printf("����ͷ��ʼ��������\r\n");
	//Camera_Init();						//��ʼ������ͷ
	printf("��ʼ�����\r\n");
	vTaskDelete(Init_Task_Handler); 	//ɾ����ʼ������
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Flash Task */
TaskHandle_t EEPROM_Task_Handler;
void EEPROM_Task(void *pvParameters);

/************************************************************************
 *	@brief	Flash
 *	@param	?
 *	@return	?
 *	@note	?
 ***********************************************************************/
void EEPROM_Task(void *pvParameters)
{
	while(1)
	{
		char str[20];
		Flash_WriteBytes(10,0,(uint8_t*)"MotherHunter\r\n",15);
		Flash_ReadBytes(10,0,(uint8_t*)str,15);
		printf("%s", str);
		vTaskDelete(EEPROM_Task_Handler);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* ADC Task */
TaskHandle_t ADC_Task_Handler;
void ADC_Task(void *pvParameters);

/************************************************************************
 *	@brief	ADC�ɼ�
 *	@param	��
 *	@return	��
 *	@note	��
 ***********************************************************************/
void ADC_Task(void *pvParameters)
{
	while(1)
	{
		Get_ADC_Data();
		Direct();
		//Direct_acr();
		vTaskDelay(5);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Camera Task */
TaskHandle_t Camera_Task_Handler;
void Camera_Task(void *pvParameters);

/************************************************************************
 *	@brief	������λ������ͼ��
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Camera_Task(void *pvParameters)
{

	while(1)
	{
		if(Image_Finish_Flag)
		{
			//Image_Binary();
			//Send_Image();
			Image_Finish_Flag = 0;
			//vTaskSuspend(Camera_Task_Handler);
		}
		vTaskDelay(10);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Steering Task */
//TaskHandle_t Steering_Task_Handler;
//void Steering_Task(void *pvParameters);

///************************************************************************
// *	@brief	���
// *	@param	��
// *	@return	��
// *	@note	��
// ***********************************************************************/
//void Steering_Task(void *pvParameters)
//{
//	while(1)
//	{
//		Set_Steering_PWM(150);
//		vTaskDelay(1000);
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Decode Task */
TaskHandle_t Decode_Task_Handler;
void Decode_Task(void *pvParameters);

/************************************************************************
 *	@brief	�������ɼ�
 *	@param	��
 *	@return	��
 *	@note	��
 ***********************************************************************/
void Decode_Task(void *pvParameters)
{
	while(1)
	{
		Get_Decode_Data();
		vTaskDelay(10);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* KEY Task */
TaskHandle_t KEY_Task_Handler;
void KEY_Task(void *pvParameters);

/************************************************************************
 *	@brief	�������
 *	@param	��
 *	@return	��
 *	@note	��
 ***********************************************************************/
void KEY_Task(void *pvParameters)
{
	uint8_t KEY_Num = 0;
	
	while(1)
	{
		KEY_Num = KEY_Scan();
		if(KEY_Num == 1)
		{
			KEY_Num = 0;
		}
		if(KEY_Num == 2)
		{
			KEY_Num = 0;
		}
		if(KEY_Num == 3)
		{
			KEY_Num = 0;
		}
		if(KEY_Num == 4)
		{
			KEY_Num = 0;
		}
		if(KEY_Num == 5)
		{
			Motor_PWM.right_pwm1 = 0;
			Motor_PWM.left_pwm1 = 0;
			Set_Motor_PWM();
		}
		vTaskDelay(100);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* LED Task */
TaskHandle_t LED_Task_Handler;
void LED_Task(void *pvParameters);

/************************************************************************
 *	@brief	LED��˸
 *	@param	pvParameters		���ݸ��������Ĳ���
 *	@return	��
 *	@note	��
 ***********************************************************************/
void LED_Task(void *pvParameters)
{
	while(1)
	{
		LED_Flash();
		vTaskDelay(250);
		LED_Flash();
		vTaskDelay(250);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* OLED Task */
TaskHandle_t OLED_Task_Handler;
void OLED_Task(void *pvParameters);

/************************************************************************
 *	@brief	OLED��ʾ
 *	@param	pvParameters		���ݸ��������Ĳ���
 *	@return	��
 *	@note	��
 ***********************************************************************/
void OLED_Task(void *pvParameters)
{
	while(1)
	{
		OLED_Display();
		vTaskDelay(100);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Motor Task */
//TaskHandle_t Motor_Task_Handler;
//void Motor_Task(void *pvParameters);

/************************************************************************
 *	@brief	���PWM���
 *	@param	pvParameters		���ݸ��������Ĳ���
 *	@return	��
 *	@note	��
 ***********************************************************************/
//void Motor_Task(void *pvParameters)
//{
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Start Task */
TaskHandle_t StartTask_Handler;
void Start_Task(void *pvParameters);

/************************************************************************
 *	@brief	��ʼ����
 *	@param	pvParameters		���ݸ��������Ĳ���
 *	@return	��
 *	@note	������������
 ***********************************************************************/
void Start_Task(void *pvParameters)
{
	taskENTER_CRITICAL();	//�����ٽ��� 
	/* Create Init Task */
	xTaskCreate((TaskFunction_t)Init_Task,
				(const char*   )"Init",
				(uint16_t      )256,
				(void*         )NULL,
				(UBaseType_t   )6,
				(TaskHandle_t* )&Init_Task_Handler);
	/* Create LED Task */
	xTaskCreate((TaskFunction_t)LED_Task,
				(const char*   )"LED",
				(uint16_t      )24,
				(void*         )NULL,
				(UBaseType_t   )2,
				(TaskHandle_t* )&LED_Task_Handler);
	/* Create OLED Task */
	xTaskCreate((TaskFunction_t)OLED_Task,
				(const char*   )"OLED",
				(uint16_t      )256,
				(void*         )NULL,
				(UBaseType_t   )2,
				(TaskHandle_t* )&OLED_Task_Handler);
	/* Create KEY Task */
	xTaskCreate((TaskFunction_t)KEY_Task,
				(const char*   )"KEY",
				(uint16_t      )24,
				(void*         )NULL,
				(UBaseType_t   )1,
				(TaskHandle_t* )&KEY_Task_Handler);
	/* Create Decode Task */
	xTaskCreate((TaskFunction_t)Decode_Task,
				(const char*   )"Decode",
				(uint16_t      )128,
				(void*         )NULL,
				(UBaseType_t   )5,
				(TaskHandle_t* )&Decode_Task_Handler);

	/* Create ADC Task */
	xTaskCreate((TaskFunction_t)ADC_Task,
				(const char*   )"ADC",
				(uint16_t      )256,
				(void*         )NULL,
				(UBaseType_t   )5,
				(TaskHandle_t* )&ADC_Task_Handler);
	/* Create EEPROM Task */
	xTaskCreate((TaskFunction_t)EEPROM_Task,
				(const char*   )"EEPROM",
				(uint16_t      )5120,
				(void*         )NULL,
				(UBaseType_t   )1,
				(TaskHandle_t* )&EEPROM_Task_Handler);
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 *	@brief	�����ʼ��
 *	@param	��
 *	@return	��
 *	@note		��
 ***********************************************************************/
void Task_List_Init(void)
{
	//������ʼ����
	xTaskCreate((TaskFunction_t)Start_Task,
				(const char*   )"Start_Task",
				(uint16_t      )128,
				(void*         )NULL,
				(UBaseType_t   )7,
				(TaskHandle_t* )&StartTask_Handler);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Static Create Task */
/* �������� */
static StaticTask_t	IDLE_TaskTCB;
static StackType_t 	IDLE_TaskStack[configTIMER_TASK_STACK_DEPTH];

/************************************************************************
 *	@brief	��ȡ����������ڴ�
 *	@param	*ppxIdleTaskTCBBuffer��		������ƿ���ڴ�
 *	@param	*ppxIdleTaskStackBuffer��	����Ķ�ջ
 *	@param	*pulIdleTaskStackSize��		�����ջ�Ĵ�С
 *	@return	��
 *	@note	���侲̬��������
 ***********************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t* 	*ppxIdleTaskTCBBuffer	 , 
								   StackType_t*		*ppxIdleTaskStackBuffer,
								   uint32_t			*pulIdleTaskStackSize	 )
{
	*ppxIdleTaskTCBBuffer 	= &IDLE_TaskTCB;
	*ppxIdleTaskStackBuffer = IDLE_TaskStack; 
	*pulIdleTaskStackSize	= configTIMER_TASK_STACK_DEPTH;
}

/* ��ʱ������ */
static StaticTask_t	Timer_TaskTCB;
static StackType_t 	Timer_TaskStack[configMINIMAL_STACK_SIZE];

/************************************************************************
 *	@brief	��ȡ��ʱ������Ķ�ջ
 *	@param	*ppxIdleTaskTCBBuffer��		������ƿ���ڴ�
 *	@param	*ppxIdleTaskStackBuffer��	����Ķ�ջ
 *	@param	*pulIdleTaskStackSize��		�����ջ�Ĵ�С
 *	@return	��
 *	@note	���侲̬��������
 ***********************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t*	*ppxTimerTaskTCBBuffer	,
																		StackType_t *	*ppxTimerTaskStackBuffer,
																		uint32_t 			*pulTimerTaskStackSize	)
{
	*ppxTimerTaskTCBBuffer   = &Timer_TaskTCB;
	*ppxTimerTaskStackBuffer = Timer_TaskStack; 
	*pulTimerTaskStackSize	 = configMINIMAL_STACK_SIZE;
}

/*--------------------END--------------------*/



