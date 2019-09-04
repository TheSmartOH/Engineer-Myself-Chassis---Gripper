#include "system.h"

//��ʼ����
TaskHandle_t  StartTaskHandler;
void start_task(void* pvParameters);

//��������
TaskHandle_t  ChassisHandler;
void Chassis_task(void* pvParameters);

//��̨����
TaskHandle_t  CloudHandler;
void Cloud_task(void* pvParameters);

//��צ����
TaskHandle_t  GripperHandler;
void Gripper_task(void* pvParameters);

void startTask(void)
{
	xTaskCreate(start_task, "START_TASK", 300, NULL, 1, &StartTaskHandler);	/*������ʼ����*/
}
void start_task(void* pvParameters)
{
	taskENTER_CRITICAL();	/*�����ٽ���*/
	//������������
	xTaskCreate(Chassis_task, "Chassis_task", 512, NULL, 3, &ChassisHandler);
	//������̨����
	xTaskCreate(Cloud_task, "Cloud_task", 512, NULL, 2, &CloudHandler);
	//����MPU6050����
//	xTaskCreate(Gripper_task, "Gripper_task",256, NULL, 2, &GripperHandler);
	
	vTaskDelete(StartTaskHandler);//ɾ����ʼ����
	taskEXIT_CRITICAL();	/*�˳��ٽ���*/							
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
//���������Ӻ���
void vApplicationIdleHook(void)
{
	
	
}









/***************   ���ʹ�þ�̬����   ***************/  




#if ( configSUPPORT_STATIC_ALLOCATION == 1)
//�������������ջ
static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
//����������ƿ�
static StaticTask_t IdleTaskTCB;

//��ʱ�����������ջ
static StackType_t TimerTaskStack[configTIMER_TASK_STACK_DEPTH];
//��ʱ������������ƿ�
static StaticTask_t TimerTaskTCB;

//��ȡ��������������ջ��������ƿ��ڴ棬��Ϊ������ʹ�õ�
//��̬�ڴ棬��˿�������������ջ��������ƿ���ڴ��Ӧ��
//���û����ṩ��FreeRTOS�ṩ�˽ӿں���vApplicationGetIdleTaskMemory()
//ʵ�ִ˺������ɡ�
//ppxIdleTaskTCBBuffer:������ƿ��ڴ�
//ppxIdleTaskStackBuffer:�����ջ�ڴ�
//pulIdleTaskStackSize:�����ջ��С
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
	*ppxIdleTaskStackBuffer=IdleTaskStack;
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

//��ȡ��ʱ����������������ջ��������ƿ��ڴ�
//ppxTimerTaskTCBBuffer:������ƿ��ڴ�
//ppxTimerTaskStackBuffer:�����ջ�ڴ�
//pulTimerTaskStackSize:�����ջ��С
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&TimerTaskTCB;
	*ppxTimerTaskStackBuffer=TimerTaskStack;
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}

#endif


