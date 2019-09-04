/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 
#include "system.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  接收遥控器数据
  * @param  None
  * @retval None
  */
void DMA1_Stream5_IRQHandler(void)
{
    #if IS_TEST_INTERRUPT_EN
        printf("dma irq！\n");
    #endif
	
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET)
    {
//        GlobalFlag |= 1<<3;//接收机接收正常 1
   //喂狗放在接收机接收中断中！
			DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
			DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
      DBusDataDec(sbus_rx_buffer);//解析接收机数据！
      if(GetDbusData())
				 ;
         //    IWDG_Feed();//喂狗！
    }     
}
/**
  * @brief	:
  * @note   :CAN1电机：底盘电机及pitch，yaw电机
  * @date   :2016/08/27
  * @design :
  **/
void CAN1_RX0_IRQHandler(void)
{   	
    #if IS_TEST_INTERRUPT_EN
        printf("can1 irq！\n");
    #endif
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
//		GlobalFlag |= 1<<0;//can1通讯正常
			CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
			CAN_ClearFlag(CAN1, CAN_FLAG_FF0); 
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
//			CAN_Receive(CAN1, CAN_FIFO0, &GripperRxMessage);
		switch(RxMessage.StdId)
		{
			case CHASSIS_LF_MOTOR_CAN_ID:
			{
        CanMonitor |= 1<<0;//3510 - LF电机连接正常
				SetTyreSpeedFdb(TYRE_LF,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));

				break;
			}

			case CHASSIS_LB_MOTOR_CAN_ID:
			{
        CanMonitor |= 1<<1;//3510 - LB电机连接正常
				SetTyreSpeedFdb(TYRE_LB,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));
////				SetGripperPositionFdb(0, (RxMessage.Data[0]<<8 | RxMessage.Data[1]));
////				Get_Countturn(0);
				break;
			}
            
			case CHASSIS_RF_MOTOR_CAN_ID:
			{
        CanMonitor |= 1<<2;//3510 - RF电机连接正常 
				SetTyreSpeedFdb(TYRE_RF,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));

				break;
			}
            
			case CHASSIS_RB_MOTOR_CAN_ID:
			{
        CanMonitor |= 1<<3;//3510 - RB电机连接正常 
				SetTyreSpeedFdb(TYRE_RB,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));				
				break;
			}			
            
			case WHEEL_LEFT_ID: 		
			{
        CanMonitor |= 1<<4;
        SetTyreSpeedFdb(TYRE_LC,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));				
				break;
			}
			
			case WHEEL_RIGHT_ID: 
			{
        CanMonitor |= 1<<5;
				SetTyreSpeedFdb(TYRE_RC,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));				
				break; 
			}
		}
	}
}


void CAN2_RX0_IRQHandler(void) 
{   	
    #if IS_TEST_INTERRUPT_EN
        printf("can2 irq！\n");
    #endif
    if(CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
    {
			GlobalFlag |= 1<<1;//can2通讯正常

			CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
			CAN_ClearFlag(CAN2, CAN_FLAG_FF0); 
					
			CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);		
			CAN_Receive(CAN2, CAN_FIFO0, &GripperRxMessage);			
			switch(RxMessage.StdId)
			{
				case Getting_Bullet_Motor:
				{
					CanMonitor |= 1<<8;
	//			  motor_SpeedFdb(Get_Bullet_Motor,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));
	//				motor_PositionFdb(Get_Bullet_Motor,(s16)(RxMessage.Data[0]<<8 | RxMessage.Data[1]));
	//        Message[0] =(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
					break;
				}
				case Shifting_Bullet_Front:
				{
					CanMonitor |= 1<<9;
					SetGripperSpeedFdb(0,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));
//					SetGripperPositionFdb(0, (RxMessage.Data[0]<<8 | RxMessage.Data[1]));
	//				motor_SpeedFdb(Shift_Bullet_Front,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));
	//				motor_PositionFdb(Shift_Bullet_Front,(s16)(RxMessage.Data[0]<<8 | RxMessage.Data[1]));
					break;	
				}
				case Shifting_Bullet_Right:
				{
					CanMonitor |= 1<<10;
	//				motor_SpeedFdb(Shift_Bullet_Right,(s16)(RxMessage.Data[2]<<8 | RxMessage.Data[3]));
	//				motor_PositionFdb(Shift_Bullet_Right,(s16)(RxMessage.Data[0]<<8 | RxMessage.Data[1]));
					break;	
				}
			}
			
			
			
		
	}
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
