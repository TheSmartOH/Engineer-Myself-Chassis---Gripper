

#include "system.h"




void systemInit()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//设置系统中断优先级分组1
	delay_init(168);																	//初始化延时函数
	while(MPU_Init());//初始化Mpu6050	
	while(mpu_dmp_init());//内部运动引擎
	uart_init(115200);
	USART2_Init(100000, 1, 0);												//接收机初始化为1000000，10Byte/ms 
	CAN1_Init(1,0);
	CAN2_Init(2,0);
}












