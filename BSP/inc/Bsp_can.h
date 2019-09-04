/**
  ******************************************************************************
  * 
  * CAN驱动，本部分用于stm32f4xx系列单片机
  * 本库函数仅提供底层驱动！
  * 
  * 注：2017.06.30对本文件进行了修改：
  *     1、兼容之前的程序代码，增添任意设备任意数据、ID发送；
  *     2、删除原有的程序文件，重新添加本文件即可；
  * 
  *
  *	创建时间：2016年07月01日
  * 最后更新：2017年07月09日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
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
void CAN1_Init(u8 NvicPP, u8 NvicSP);//初始化配置
void CAN2_Init(u8 NvicPP, u8 NvicSP);//初始化配置

void CAN_SendMail(CAN_TypeDef* CANx, u32 ID, u8* Data);//CAN总线发送数据报文
void CAN1_SendChassisMail(s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB);//发送地盘控制数据
u8 CAN1_SendMBoardMail(s16 MBoard1H, s16 MBoard1L, s16 MBoard2H, s16 MBoard2L);//通过can1总线发送中心板电机报文
void CAN2_SendChassisMail(u32 ID,s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB);
void Send_Remote(void);
u8 CAN1_SendManifold(u8 SendDate[]);

void Cloud_SendControlMail(s16 CloudYaw, s16 CloudPitch);
#endif




