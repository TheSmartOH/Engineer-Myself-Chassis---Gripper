/** 
 ******************************************************************************
  * 
  * USART4���ڿ⣬��ͨ����ʹ�ô���4���Գ���
  * ʹ��������PC�˴��ڼ�����ͨ�ŵĶ˿�
  * 
  * 
  *	����ʱ�䣺2018��03��27��
  * �����£�
  * �� �� �ˣ����ͽ�
  * 
  ******************************************************************************
***/



#ifndef __BSP_USART4_H__
#define __BSP_USART4_H__



/*Bsp_Usart3 Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "misc.h" 
#include <stdio.h>



/*Bsp_Usart3 Function Define*/
void UART4_Init(u32 Baud);//��ʼ������
void UART4_SendChar(u8 Schar);//���͵�һ�ַ�
void UART4_SendData(char *Data, u8 Size);//�������ͷ�char��������(��printf���ͣ�)

//void shanwai_displaywave0(s8 *CharData, u8 Length);//���͵�ɽ����λ����PC����ʾ����(char������)
//void shanwai_displaywave1(s16 *IntData, u8 Length);//���͵�ɽ����λ����PC����ʾ����(int������)
//void shanwai_displaywave2(float *FloatData, u8 Length);//���͵�ɽ����λ����PC����ʾ����(float������)

#endif









