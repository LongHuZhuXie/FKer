/*********************************************************************************************************************
 * @file       		sys.c
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					״̬��
 ********************************************************************************************************************/

#include "sys.h"

/*Private Variables*/
unsigned int duty = 0;

/*System Initialization*/
void System_Init(void)
{
	get_clk();													//��ȡ����Ƶ����Ϣ�����ں������ģ��Ĳ�������
	SysTick_Init();											//��ʼ��ϵͳʱ��
	uart_init(DEBUG_PORT,DEBUG_BAUD);   //��ʼ�����Դ���
	OLED_Init();												//��ʼ��OLED
	LED_Init();													//��ʼ��LED
	KEY_Init();													//��ʼ������
	Scheduler_Setup();									//���������б�
}

/*------------------Tasks List------------------*/
static void Loop_1000Hz(void)//1msִ��һ��
{
}
static void Loop_500Hz(void) //2msִ��һ��
{
}
static void Loop_200Hz(void) //5msִ��һ��
{
}
static void Loop_100Hz(void) //10msִ��һ��
{
}
static void Loop_50Hz(void)  //20msִ��һ��
{
}
static void Loop_20Hz(void)  //50msִ��һ��
{
}
static void Loop_10Hz(void)  //100msִ��һ��
{
	/*OLED*/
	OLED_Display();
}
static void Loop_2Hz(void)   //500msִ��һ��
{
	LED_Flash();
}
/*------------------Task List END------------------*/

/*System Variables*/
uint32 systime_ms;
volatile uint32 sysTickUptime = 0;

#define TICK_US (1000000 / TICK_PER_SECOND)

/*Task Structure*/
typedef struct Sched_Task
{
	void (*task_func)(void); 	//����ָ��
	uint16 rate_hz;		 		 		//�����ִ��Ƶ��
	uint16 interval_ticks; 		//�����ִ������
	uint32 last_run;		   		//��һ��ִ�и������ʱ��
} Create_Task;

/*SysTick Initialization*/
void SysTick_Init(void)
{
	uint32 cnts;

	cnts = (uint32)core_clk_mhz * 1000000 / TICK_PER_SECOND;
	cnts = cnts;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

/*SysTick IRQHandle*/
void SysTick_Handler(void)
{
	sysTickUptime++;	//����ʱʹ�õ�ʱ��
	systime_ms++;			//ϵͳ��ǰ��ʱ��
}

/*��ȡ��ȷʱ��*/
uint32_t Getsystime_us(void)
{
	register uint32_t ms;
	uint32_t value;

	do
	{
		ms = sysTickUptime;
		value = ms * TICK_US + ((SysTick->LOAD - SysTick->VAL) / SysTick->LOAD) * TICK_US;
	} while (ms != sysTickUptime);

	return value;
}

/*��ȡ��ǰ��ϵͳʱ��*/
uint32 SysTick_GetTick(void)
{
	return systime_ms;
}

/*�����б�ṹ��*/
static Create_Task sched_tasks[] =
	{
		{Loop_1000Hz, 1000, 0, 0},
		{ Loop_500Hz,  500, 0, 0},
		{ Loop_200Hz,  200, 0, 0},
		{ Loop_100Hz,  100, 0, 0},
		{  Loop_50Hz,   50, 0, 0},
		{  Loop_20Hz,   20, 0, 0},
		{  Loop_10Hz,   10, 0, 0},
		{   Loop_2Hz,    2, 0, 0},
};

/*������������*/
#define TASK_NUM (sizeof(sched_tasks) / sizeof(Create_Task))

void Scheduler_Setup(void)
{
	uint8 index = 0;

	for (index = 0; index < TASK_NUM; index++)
	{
		//����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
		if (sched_tasks[index].interval_ticks < 1) //�������Ϊ1ms
			sched_tasks[index].interval_ticks = 1;
	}
}

//��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
void Scheduler_Run(void)
{
	uint8 index = 0;
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��
	for (index = 0; index < TASK_NUM; index++)
	{
		//��ȡϵͳ�ĵ�ǰʱ�䣬��λmS
		uint32 tnow = SysTick_GetTick();
		//�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
		if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//�����̵߳�ִ��ʱ�䣬������һ���ж�
			sched_tasks[index].last_run = tnow;
			//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
			sched_tasks[index].task_func();
		}
	}
}
