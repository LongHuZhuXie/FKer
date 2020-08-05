/*********************************************************************************************************************
 * @file       	MyTask.c
 * @version		V1.0
 * @Software 	MDK 5.28
 * @TargetCore	MK66FX
 * @date       	2019-12-23
 * @note		所有任务都写在此文件中
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
#include "oled.h"
#include "SEEKFREE_18TFT.h"
#include "display.h"
#include "motor.h"
#include "buzzer.h"
#include "usart.h"
#include "steering.h"
#include "decode.h"
#include "camera.h"
#include "adc.h"
#include "MK60_flash.h"
#include "Hall.h"

/* PID */
#include "elector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

/* Init Task */
TaskHandle_t Init_Task_Handler;
void Init_Task(void *pvParameters);

/************************************************************************
 *	@brief	外设初始化
 *	@param	pvParameters		传递给任务函数的参数
 *	@return	无
 *	@note	无
 ***********************************************************************/
void Init_Task(void *pvParameters)
{
	/* Peripheral Device Initilization */
	D_PID_initial(1.62,0.00,0.12 );
	M_PID_initial(0.98f,0.12,0.20f);
	NRF_Init(115200);					//初始化调试串口
//	printf("FlASH初始化···\r\n");		//初始化Flash
//	FLASH_Init();
	printf("霍尔元件初始化···\r\n");	
	Hall_Init();						//初始化霍尔元件
	printf("蜂鸣器初始化···\r\n");		
	Buzzer_Init();						//初始化蜂鸣器
	printf("按键初始化···\r\n");
	KEY_Init();							//初始化KEY
	printf("LED初始化···\r\n");
	LED_Init();							//初始化LED
	printf("OLED初始化···\r\n");
	OLED_Init();						//初始化OLED
	printf("LCD初始化...\r\n");
	//lcd_init();							//初始化LCD
	printf("ADC初始化···\r\n");				
	ADC_Init();							//初始化ADC
	printf("舵机初始化···\r\n");
	Steering_Init();					//初始化舵机PWM
	printf("电机初始化···\r\n");
	Motor_PWM_Init();					//初始化全桥驱动SPWM
	printf("编码器初始化···\r\n");
	Decode_Init();						//初始化编码器
	printf("摄像头初始化···\r\n");
	Camera_Init();					//初始化摄像头
	printf("初始化完成\r\n");
	vTaskDelete(Init_Task_Handler); 	//删除初始化任务
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
 *	@brief	ADC采集
 *	@param	无
 *	@return	无
 *	@note	无
 ***********************************************************************/
void ADC_Task(void *pvParameters)
{
	while(1)
	{
		Get_ADC_Data();
		if(!road_flag)
			M_start();
		else if(!stop_flag)
		{Direct();
			;
		}
		else
		{
			if(Speed_L>40)
			{
			Motor_PWM.left_pwm1=-150;
			Motor_PWM.right_pwm1=-150;
			}
			else
			{
				Motor_PWM.left_pwm1=5;
				Motor_PWM.right_pwm1=5;
			}
		  Set_Motor_PWM();
			Set_Steering_PWM(166);
		}
		//Direct_acr();
		//if(!Hall_Scan())		{printf("Finish\r\n");}
		vTaskDelay(10);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Camera Task */
TaskHandle_t Camera_Task_Handler;
void Camera_Task(void *pvParameters);

/************************************************************************
 *	@brief	无向上位机发送图像
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Camera_Task(void *pvParameters)
{
	int i;
	uint8 *data= (uint8 *)image;
	while(1)
	{
		THRESHOLD=180;
		if(Image_Finish_Flag)
		{
			//iteration_Thresholdfigure();
			Image_Binary();
			//printf("%d\n",THRESHOLD);
			
			//Image_recontract();
			for (i=0;i<50;i++)
			{
				Left_Add[i]=1;
				Right_Add[i]=1;
				width[i]=0;
			}
			First_linecope((uint8 *)image);
			for (i=ROW-2;i >= ROW-35;i--)
			{		//	if(!data[i*COL+Mid_Line[i+1]])
				//{
				//	Mid_Count=49-i;
				//	break;
				//}
				Traversal_Mid_Line(i,(uint8 *)image);
			}
			//Mid_Filtering((uint8 *)image);
			//Image_recontract();
			//Send_Image();
			Image_Finish_Flag = 0;
			vTaskSuspend(Camera_Task_Handler);
		}
		vTaskDelay(10);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Steering Task */
//TaskHandle_t Steering_Task_Handler;
//void Steering_Task(void *pvParameters);

///************************************************************************
// *	@brief	舵机
// *	@param	无
// *	@return	无
// *	@note	无
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
 *	@brief	编码器采集
 *	@param	无
 *	@return	无
 *	@note	无
 ***********************************************************************/
void Decode_Task(void *pvParameters)
{
	while(1)
	{
		Get_Decode_Data();
		vTaskDelay(5);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* KEY Task */
TaskHandle_t KEY_Task_Handler;
void KEY_Task(void *pvParameters);

/************************************************************************
 *	@brief	按键检测
 *	@param	无
 *	@return	无
 *	@note	无
 ***********************************************************************/
void KEY_Task(void *pvParameters)
{
	uint8_t KEY_Num = 0;
	static uint8 duty=150;
	while(1)
	{
		KEY_Num = KEY_Scan();
		if(KEY_Num == 1)
		{
			KEY_Num = 0;
			Set_Steering_PWM(duty);
			
		}
		if(KEY_Num == 2)
		{
			KEY_Num = 0;
			duty++;
		}
		if(KEY_Num == 3)
		{
			KEY_Num = 0;
			duty --;
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
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* LED Task */
TaskHandle_t LED_Task_Handler;
void LED_Task(void *pvParameters);

/************************************************************************
 *	@brief	LED闪烁
 *	@param	pvParameters		传递给任务函数的参数
 *	@return	无
 *	@note	无
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
 *	@brief	OLED显示
 *	@param	pvParameters		传递给任务函数的参数
 *	@return	无
 *	@note	无
 ***********************************************************************/
void OLED_Task(void *pvParameters)
{
	while(1)
	{
		OLED_Display();
		vTaskDelay(50);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Motor Task */
//TaskHandle_t Motor_Task_Handler;
//void Motor_Task(void *pvParameters);

/************************************************************************
 *	@brief	电机PWM输出
 *	@param	pvParameters		传递给任务函数的参数
 *	@return	无
 *	@note	无
 ***********************************************************************/
//void Motor_Task(void *pvParameters)
//{
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Start Task */
TaskHandle_t StartTask_Handler;
void Start_Task(void *pvParameters);

/************************************************************************
 *	@brief	开始任务
 *	@param	pvParameters		传递给任务函数的参数
 *	@return	无
 *	@note	创建其他任务
 ***********************************************************************/
void Start_Task(void *pvParameters)
{
	taskENTER_CRITICAL();	//进入临界区 
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
				(UBaseType_t   )1,
				(TaskHandle_t* )&LED_Task_Handler);
	/* Create OLED Task */
	xTaskCreate((TaskFunction_t)OLED_Task,
				(const char*   )"OLED",
				(uint16_t      )256,
				(void*         )NULL,
				(UBaseType_t   )2,
				(TaskHandle_t* )&OLED_Task_Handler);
	/* Create Camera Task*/
	xTaskCreate((TaskFunction_t)Camera_Task,
				(const char*   )"Camera",
				(uint16_t      )8000,
				(void*         )NULL,
				(UBaseType_t   )4,
				(TaskHandle_t* )&Camera_Task_Handler);	
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
				(uint16_t      )4096,
				(void*         )NULL,
				(UBaseType_t   )5,
				(TaskHandle_t* )&ADC_Task_Handler);
	/* Create EEPROM Task */
//	xTaskCreate((TaskFunction_t)EEPROM_Task,
//				(const char*   )"EEPROM",
//				(uint16_t      )5120,
//				(void*         )NULL,
//				(UBaseType_t   )1,
//				(TaskHandle_t* )&EEPROM_Task_Handler);
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 *	@brief	任务初始化
 *	@param	无
 *	@return	无
 *	@note		无
 ***********************************************************************/
void Task_List_Init(void)
{
	//创建开始任务
	xTaskCreate((TaskFunction_t)Start_Task,
				(const char*   )"Start_Task",
				(uint16_t      )128,
				(void*         )NULL,
				(UBaseType_t   )7,
				(TaskHandle_t* )&StartTask_Handler);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Static Create Task */
/* 空闲任务 */
static StaticTask_t	IDLE_TaskTCB;
static StackType_t 	IDLE_TaskStack[configTIMER_TASK_STACK_DEPTH];

/************************************************************************
 *	@brief	获取空闲任务的内存
 *	@param	*ppxIdleTaskTCBBuffer：		任务控制块的内存
 *	@param	*ppxIdleTaskStackBuffer：	任务的堆栈
 *	@param	*pulIdleTaskStackSize：		任务堆栈的大小
 *	@return	无
 *	@note	适配静态创建任务
 ***********************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t* 	*ppxIdleTaskTCBBuffer	 , 
								   StackType_t*		*ppxIdleTaskStackBuffer,
								   uint32_t			*pulIdleTaskStackSize	 )
{
	*ppxIdleTaskTCBBuffer 	= &IDLE_TaskTCB;
	*ppxIdleTaskStackBuffer = IDLE_TaskStack; 
	*pulIdleTaskStackSize	= configTIMER_TASK_STACK_DEPTH;
}

/* 定时器任务 */
static StaticTask_t	Timer_TaskTCB;
static StackType_t 	Timer_TaskStack[configMINIMAL_STACK_SIZE];

/************************************************************************
 *	@brief	获取定时器任务的堆栈
 *	@param	*ppxIdleTaskTCBBuffer：		任务控制块的内存
 *	@param	*ppxIdleTaskStackBuffer：	任务的堆栈
 *	@param	*pulIdleTaskStackSize：		任务堆栈的大小
 *	@return	无
 *	@note	适配静态创建任务
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



