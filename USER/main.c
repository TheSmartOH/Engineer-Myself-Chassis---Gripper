#include "system.h"











int main(void)
{
	/*系统初始化*/
	systemInit();
	startTask();
	vTaskStartScheduler();      //开启任务调度
	while(1){}
}

