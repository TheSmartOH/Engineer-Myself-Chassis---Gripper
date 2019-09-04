#include "system.h"

//开始任务
TaskHandle_t  StartTaskHandler;
void start_task(void* pvParameters);

//底盘任务
TaskHandle_t  ChassisHandler;
void Chassis_task(void* pvParameters);

//云台任务
TaskHandle_t  CloudHandler;
void Cloud_task(void* pvParameters);

//夹爪任务
TaskHandle_t  GripperHandler;
void Gripper_task(void* pvParameters);

void startTask(void)
{
	xTaskCreate(start_task, "START_TASK", 300, NULL, 1, &StartTaskHandler);	/*创建起始任务*/
}
void start_task(void* pvParameters)
{
	taskENTER_CRITICAL();	/*进入临界区*/
	//创建底盘任务
	xTaskCreate(Chassis_task, "Chassis_task", 512, NULL, 3, &ChassisHandler);
	//创建云台任务
	xTaskCreate(Cloud_task, "Cloud_task", 512, NULL, 2, &CloudHandler);
	//创建MPU6050任务
//	xTaskCreate(Gripper_task, "Gripper_task",256, NULL, 2, &GripperHandler);
	
	vTaskDelete(StartTaskHandler);//删除开始任务
	taskEXIT_CRITICAL();	/*退出临界区*/							
}

void Chassis_task(void* pvParameters)
{
	while(1)
	{
		ChassisControl();
		vTaskDelay(10);
	}
	
}
void Cloud_task(void* pvParameters)
{
	for(;;)
	{
		Cloud_Control();
		vTaskDelay(10);
	}
}

void Gripper_task(void* pvParameters)
{
	for(;;)
	{
		Gripper_Driver();
		vTaskDelay(10);
	}
	
	
}
//空闲任务钩子函数
void vApplicationIdleHook(void)
{
	
	
}









/***************   如果使用静态任务   ***************/  




#if ( configSUPPORT_STATIC_ALLOCATION == 1)
//空闲任务任务堆栈
static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
//空闲任务控制块
static StaticTask_t IdleTaskTCB;

//定时器服务任务堆栈
static StackType_t TimerTaskStack[configTIMER_TASK_STACK_DEPTH];
//定时器服务任务控制块
static StaticTask_t TimerTaskTCB;

//获取空闲任务地任务堆栈和任务控制块内存，因为本例程使用的
//静态内存，因此空闲任务的任务堆栈和任务控制块的内存就应该
//有用户来提供，FreeRTOS提供了接口函数vApplicationGetIdleTaskMemory()
//实现此函数即可。
//ppxIdleTaskTCBBuffer:任务控制块内存
//ppxIdleTaskStackBuffer:任务堆栈内存
//pulIdleTaskStackSize:任务堆栈大小
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
	*ppxIdleTaskStackBuffer=IdleTaskStack;
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

//获取定时器服务任务的任务堆栈和任务控制块内存
//ppxTimerTaskTCBBuffer:任务控制块内存
//ppxTimerTaskStackBuffer:任务堆栈内存
//pulTimerTaskStackSize:任务堆栈大小
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&TimerTaskTCB;
	*ppxTimerTaskStackBuffer=TimerTaskStack;
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}

#endif


