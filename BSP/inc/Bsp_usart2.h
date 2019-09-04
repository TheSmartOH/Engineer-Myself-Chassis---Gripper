/**
  ******************************************************************************
  * 
  * ���ڽ��ջ����ݽ��ղ���
  *
  * ע����һ�沽�������ذ���ջ�ͨ��Ϊusart2��PA2/PA3��
  *     �ڶ��沽�������ذ���ջ�ͨ��Ϊusart2��PD5/PD6��
  * 
  * 
  *	����ʱ�䣺2016��09��09��
  * �����£�2016��10��05��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __BSP_USART2_H__
#define __BSP_USART2_H__



/*Bsp_Usart2 Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "misc.h" 
#include <stdio.h>



/*Bsp_Usart2 Param Define*/
extern u8 sbus_rx_buffer[18];//Ϊ�������ݿ��ٵ��ڴ�ռ�



/*Bsp_Usart2 Function Define*/
void USART2_Init(u32 Baud, u8 NvicPP, u8 NvicSP);//���ջ����ճ�ʼ������



#endif









