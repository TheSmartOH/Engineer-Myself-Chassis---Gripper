/**
  ******************************************************************************
  * @file    Bsp_Beep.h
  * @author  �㿭��
  * @version V0.0.0
  * @date    2017.04.18 rkq
  * @brief   ��Ҫ�����������������ӿ�
  *  
  ******************************************************************************
  * @attention
  *			1.������ʹ��PB4���ţ�tim3_ch1
  *			2.Ŀǰ֧�ֵ���Ŀ���٣��д���������
  *			
  ******************************************************************************  
  */ 
#ifndef _BSP_BEEP_H_
#define _BSP_BEEP_H_
#include "stm32f4xx.h"


#define	BEEP_TIM_CLOCK			RCC_APB2Periph_TIM11
#define	BEEP_GPIO_CLOCK			RCC_AHB1Periph_GPIOB
#define	BEEP_GPIO_PORT			GPIOB
#define	BEEP_GPIO_AF_PIN		GPIO_PinSource9
#define	BEEP_GPIO_AF			GPIO_AF_TIM11
#define	BEEP_GPIO_PIN			GPIO_Pin_9
#define	BEEP_TIMx				TIM11
#define	BEEP_OUTPUT				BEEP_TIMx->CCR1
#define BEEP_ARR				BEEP_TIMx->ARR

#define CLAWINITSOUND			0

void BEEP_Init(void);
void PlayMusic(void);
void BEEP_Close(void);
void BEEP_PlayMusic(void);



#endif



