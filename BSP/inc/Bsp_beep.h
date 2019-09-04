/**
  ******************************************************************************
  * @file    Bsp_Beep.h
  * @author  茹凯琪
  * @version V0.0.0
  * @date    2017.04.18 rkq
  * @brief   主要包含蜂鸣器的驱动接口
  *  
  ******************************************************************************
  * @attention
  *			1.蜂鸣器使用PB4引脚，tim3_ch1
  *			2.目前支持的曲目较少，有待后期增加
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



