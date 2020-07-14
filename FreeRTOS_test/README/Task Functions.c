
///////////////////////////////////////////////////////////////////////////////////////////////
/* 1.创建任务 */
/************************************************************************
 *	@brief		动态创建任务
 *
 * 	@param 		pvTaskCode: Pointer to the task entry function.  Tasks
 * 	must be implemented to never return (i.e. continuous loop).
 *
 * 	@param 		pcName: A descriptive name for the task.  This is mainly
 *used to facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN -
 *default is 16.
 *
 *	@param 		usStackDepth: The size of the task stack specified as
 *the number of variables the stack can hold - not the number of bytes.  For
 *example, if the stack is 16 bits wide and usStackDepth is defined as 100, 200
 *bytes will be allocated for stack storage.
 *
 * 	@param 		pvParameters: Pointer that will be used as the parameter
 *for the task being created.
 *
 * 	@param 		uxPriority: The priority at which the task should run.
 *Systems that include MPU support can optionally create tasks in a privileged
 *(system) mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
 * 	example, to create a privileged task at priority 2 the uxPriority
 *parameter should be set to ( 2 | portPRIVILEGE_BIT ).
 *
 * 	@param 		pvCreatedTask: Used to pass back a handle by which the
 *created task can be referenced.
 *
 * 	@return		pdPASS: if the task was successfully created and added
 *to a ready list, otherwise an error code defined in the file projdefs.h
 *
 *	@note			xTaskCreate() can only be used to create a task
 *that has unrestricted access to the entire microcontroller memory map. Systems
 *that include MPU support can alternatively create an MPU constrained task
 *using xTaskCreateRestricted().
 ***********************************************************************/
// BaseType_t xTaskCreate(	TaskFunction_t	pxTaskCode
// ,
// const char
// * const pcName				, const uint16_t
// usStackDepth				, void * const pvParameters
// , UBaseType_t 		uxPriority ,
// TaskHandle_t * 	const pxCreatedTask);

/************************************************************************
 *	@brief	静态创建任务
 *
 * 	@param 	pvTaskCode: Pointer to the task entry function.  Tasks
 * 	must be implemented to never return (i.e. continuous loop).
 *
 *	@param pcName: A descriptive name for the task.  This is mainly used to
 * 	facilitate debugging.  The maximum length of the string is defined by
 * 	configMAX_TASK_NAME_LEN in FreeRTOSConfig.h.
 *
 * 	@param 	ulStackDepth The size of the task stack specified as the number
 *of variables the stack can hold - not the number of bytes.  For example, if
 * 	the stack is 32-bits wide and ulStackDepth is defined as 100 then 400
 *bytes will be allocated for stack storage.
 *
 * 	@param 	pvParameters Pointer that will be used as the parameter for the
 *task being created.
 *
 * 	@param 	uxPriority The priority at which the task will run.
 *
 * 	@param 	pxStackBuffer Must point to a StackType_t array that has at
 *least ulStackDepth indexes - the array will then be used as the task's stack,
 * 	removing the need for the stack to be allocated dynamically.
 *
 * 	@param 	pxTaskBuffer Must point to a variable of type StaticTask_t,
 *which will then be used to hold the task's data structures, removing the need
 *for the memory to be allocated dynamically.
 *
 * 	@return If neither pxStackBuffer or pxTaskBuffer are NULL, then the task
 *will be created and pdPASS is returned.  If either pxStackBuffer or
 *pxTaskBuffer are NULL then the task will not be created and
 * 	errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY is returned.
 *
 *	@note		无
 ***********************************************************************/
// TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
//									const
// char
//* const pcName,
// const uint32_t ulStackDepth,
// void * const pvParameters,
// UBaseType_t
// uxPriority,
// StackType_t
// * const puxStackBuffer, StaticTask_t * const pxTaskBuffer )
// PRIVILEGED_FUNCTION;

//这是我修改的
//这是我第二次修改的

