#include "system.h"











int main(void)
{
	/*ϵͳ��ʼ��*/
	systemInit();
	startTask();
	vTaskStartScheduler();      //�����������
	while(1){}
}

