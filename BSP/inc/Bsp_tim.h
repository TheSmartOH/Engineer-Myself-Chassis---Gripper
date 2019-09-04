/**
  ******************************************************************************
  * 
  * TIM������������������stm32f4xxϵ�е�Ƭ��
  * ���⺯����������ʱ
  * 
  * 
  *	����ʱ�䣺2016��08��27��
  * �����£�2016��10��05��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __BSP_TIM_H__
#define __BSP_TIM_H__



/*Bsp_Tim Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h" 



/*Bsp_Tim's Gpio Connect Define
  Tim_2:
		chn_1:
		chn_2:
		chn_3:PA2	PB10
		chn_4:PA3	PB11
  Tim_3:
		chn_1:PA6	PC6
		chn_2:PA7	PC7
		chn_3:PB0	PC8		
		chn_4:PB1	PC9
  Tim_4:
		chn_1:PB6	PD12
		chn_2:PB7	PD13
		chn_3:PB8	PD14
		chn_4:PB9	PD15
  Tim_5:
		chn_1:PA0
		chn_2:PA1
		chn_3:PA2
		chn_4:PA3
  Tim_9:
		chn_1:PA2	PE5
		chn_2:PA3	PE6
  Tim_12:
		chn_1:PB14
		chn_2:PB15
  Tim_13:
		chn_1:PA6
  Tim_14:
		chn_1:PA7
		chn_2:PA8
*/



/*Bsp_Tim Command Define*/
#define TIM_6_ARR				(1000-1)//tim6Ԥװ��ֵ(1ms)
#define TIM_6_PSC				(84-1)	

#define	TIM_7_ARR			2000-1	//TIM_7Ԥװ��ֵ(1KHz)
#define	TIM_7_PSC			84-1	//TIM_7Ԥ��Ƶ


/*Bsp_Tim Function Define*/
void TIM6_Init(u8 NvicPP, u8 NvicSP);//���ڳ���������ִ�ж�ʱ
void TIM2_Init(void);//����ϵͳ���м�ʱ
u32 Get_Time_Micros(void);
void TIM_7_HeatRecover(u8 pp, u8 sp);



#endif









