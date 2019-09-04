/**
  ******************************************************************************
  * 
  * CAN����������������stm32f4xxϵ�е�Ƭ��
  * ���⺯�����ṩ�ײ�������
  * 
  * ע��2017.06.30�Ա��ļ��������޸ģ�
  *     1������֮ǰ�ĳ�����룬���������豸�������ݡ�ID���ͣ�
  *     2��ɾ��ԭ�еĳ����ļ���������ӱ��ļ����ɣ�
  * 
  *
  *	����ʱ�䣺2016��07��01��
  * �����£�2017��07��09��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__



/*Bsp_Can Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_can.h"
#include "misc.h" 

typedef union
{
	u8 		u8_temp[2];
	u16 	u16_temp;
}Formatrans;



/*Bsp_Can Param Define*/
extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;
extern CanRxMsg GripperRxMessage;


/*Bsp_Can Function Define*/
void CAN1_Init(u8 NvicPP, u8 NvicSP);//��ʼ������
void CAN2_Init(u8 NvicPP, u8 NvicSP);//��ʼ������

void CAN_SendMail(CAN_TypeDef* CANx, u32 ID, u8* Data);//CAN���߷������ݱ���
void CAN1_SendChassisMail(s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB);//���͵��̿�������
u8 CAN1_SendMBoardMail(s16 MBoard1H, s16 MBoard1L, s16 MBoard2H, s16 MBoard2L);//ͨ��can1���߷������İ�������
void CAN2_SendChassisMail(u32 ID,s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB);
void Send_Remote(void);
u8 CAN1_SendManifold(u8 SendDate[]);

void Cloud_SendControlMail(s16 CloudYaw, s16 CloudPitch);
#endif




