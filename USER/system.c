

#include "system.h"




void systemInit()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//����ϵͳ�ж����ȼ�����1
	delay_init(168);																	//��ʼ����ʱ����
	while(MPU_Init());//��ʼ��Mpu6050	
	while(mpu_dmp_init());//�ڲ��˶�����
	uart_init(115200);
	USART2_Init(100000, 1, 0);												//���ջ���ʼ��Ϊ1000000��10Byte/ms 
	CAN1_Init(1,0);
	CAN2_Init(2,0);
}












