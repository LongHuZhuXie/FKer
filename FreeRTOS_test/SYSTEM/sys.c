/*********************************************************************************************************************
 * @file       		sys.c
 * @version    		V1.0
 * @Software 			IAR 8.3 or MDK 5.24
 * @TargetCore		MK66FX
 * @date       		2019-12-13
 * @note					状态机
 ********************************************************************************************************************/

#include "sys.h"

/*Private Variables*/
unsigned int duty = 0;

/*System Initialization*/
void System_Init(void)
{
	get_clk();													//获取各个频率信息，便于后面各个模块的参数设置
	SysTick_Init();											//初始化系统时钟
	uart_init(DEBUG_PORT,DEBUG_BAUD);   //初始化调试串口
	OLED_Init();												//初始化OLED
	LED_Init();													//初始化LED
	KEY_Init();													//初始化按键
	Scheduler_Setup();									//创建任务列表
}

/*------------------Tasks List------------------*/
static void Loop_1000Hz(void)//1ms执行一次
{
}
static void Loop_500Hz(void) //2ms执行一次
{
}
static void Loop_200Hz(void) //5ms执行一次
{
}
static void Loop_100Hz(void) //10ms执行一次
{
}
static void Loop_50Hz(void)  //20ms执行一次
{
}
static void Loop_20Hz(void)  //50ms执行一次
{
}
static void Loop_10Hz(void)  //100ms执行一次
{
	/*OLED*/
	OLED_Display();
}
static void Loop_2Hz(void)   //500ms执行一次
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
	void (*task_func)(void); 	//函数指针
	uint16 rate_hz;		 		 		//任务的执行频率
	uint16 interval_ticks; 		//任务的执行周期
	uint32 last_run;		   		//上一次执行该任务的时间
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
	sysTickUptime++;	//供延时使用的时间
	systime_ms++;			//系统当前的时间
}

/*获取精确时间*/
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

/*获取当前的系统时间*/
uint32 SysTick_GetTick(void)
{
	return systime_ms;
}

/*任务列表结构体*/
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

/*计算任务数量*/
#define TASK_NUM (sizeof(sched_tasks) / sizeof(Create_Task))

void Scheduler_Setup(void)
{
	uint8 index = 0;

	for (index = 0; index < TASK_NUM; index++)
	{
		//计算每个任务的延时周期数
		sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
		if (sched_tasks[index].interval_ticks < 1) //最短周期为1ms
			sched_tasks[index].interval_ticks = 1;
	}
}

//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Scheduler_Run(void)
{
	uint8 index = 0;
	//循环判断所有线程，是否应该执行
	for (index = 0; index < TASK_NUM; index++)
	{
		//获取系统的当前时间，单位mS
		uint32 tnow = SysTick_GetTick();
		//进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
		if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//更新线程的执行时间，用于下一次判断
			sched_tasks[index].last_run = tnow;
			//执行线程函数，使用的是函数指针
			sched_tasks[index].task_func();
		}
	}
}
