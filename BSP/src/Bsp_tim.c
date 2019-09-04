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



#include "Bsp_tim.h"//�����������ݴ洢����


/**
  * @brief	:tim6���ó�ʼ��
  * @note   :--���ڳ���������ִ�ж�ʱ��
  * @param	:NvicPP, ��ռ���ȼ�
			 NvicSP, ��Ӧ���ȼ�
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void TIM6_Init(u8 NvicPP, u8 NvicSP)
{
	NVIC_InitTypeDef	nvic;	
	
    /*Tim_2 nvic config*/
	nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
	nvic.NVIC_IRQChannelSubPriority        = NvicSP;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

    /*Tim use for timer init config*/		
	RCC->APB1ENR |= 1<<4;	    //tim6ʱ��ʹ��

	TIM6->ARR = TIM_6_ARR;		//�Զ���װֵ
	TIM6->PSC = TIM_6_PSC - 1;  //Ԥ��Ƶ
	
    /*Tim enable config*/		
	TIM6->DIER |= 1<<0;		    //��������ж�
	TIM6->CR1  |= 0x01;		    //tim6ʹ��

    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}



/**
  * @brief	:tim2���ó�ʼ��
  * @note   :--����ϵͳ���м�ʱ��
  * @param	:void
  * @return	:void
  *
  * @date   :2017/06/26
  * @design :
  **/
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef tim;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    tim.TIM_Prescaler       = 84 - 1;//1M ��ʱ��  	 
    tim.TIM_Period          = 0xFFFFFFFF;
    tim.TIM_ClockDivision   = TIM_CKD_DIV1;	
    tim.TIM_CounterMode     = TIM_CounterMode_Up; 
    
    TIM_ARRPreloadConfig(TIM2, ENABLE);	
    TIM_TimeBaseInit(TIM2, &tim);
    
    TIM_Cmd(TIM2,ENABLE);	
}


u32 Get_Time_Micros(void)
{
	return TIM2->CNT;
}


/*
 ******************************************************************************************************************************
 *	��    �֣�TIM_7_InitConfig()
 *	��    ����u8 pp,u8 sp
 *	����ֵ  ��void
 *	������������¼����ʱ��
 *	����ʱ�䣺2018.03.30
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�Ҫд����ʲô��Ϊʲô����Ҫд��ô������ʱ�䣬����
 *			�ڸö�ʱ�ж�����Ҫ�������ǳ�����Ҫִ������
 *
 ******************************************************************************************************************************
*/


void TIM_7_HeatRecover(u8 pp, u8 sp)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = TIM_7_ARR; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM_7_PSC;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��7�����ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pp; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=sp; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}




