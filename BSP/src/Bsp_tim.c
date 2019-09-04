/**
  ******************************************************************************
  * 
  * TIM驱动，本部分适用于stm32f4xx系列单片机
  * 本库函数包括程序定时
  * 
  * 
  *	创建时间：2016年08月27日
  * 最后更新：2016年10月05日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#include "Bsp_tim.h"//包含捕获数据存储定义


/**
  * @brief	:tim6配置初始化
  * @note   :--用于程序轮周期执行定时！
  * @param	:NvicPP, 抢占优先级
			 NvicSP, 响应优先级
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
	RCC->APB1ENR |= 1<<4;	    //tim6时钟使能

	TIM6->ARR = TIM_6_ARR;		//自动重装值
	TIM6->PSC = TIM_6_PSC - 1;  //预分频
	
    /*Tim enable config*/		
	TIM6->DIER |= 1<<0;		    //允许更新中断
	TIM6->CR1  |= 0x01;		    //tim6使能

    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}



/**
  * @brief	:tim2配置初始化
  * @note   :--用于系统运行计时！
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
    
    tim.TIM_Prescaler       = 84 - 1;//1M 的时钟  	 
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
 *	名    字：TIM_7_InitConfig()
 *	参    数：u8 pp,u8 sp
 *	返回值  ：void
 *	功能描述：记录供电时间
 *	建立时间：2018.03.30
 *	备    注：每次修改时请标注修改理由（要写明是什么，为什么，不要写怎么做），时间，姓名
 *			在该定时中断中主要包含的是程序主要执行流程
 *
 ******************************************************************************************************************************
*/


void TIM_7_HeatRecover(u8 pp, u8 sp)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = TIM_7_ARR; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM_7_PSC;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器7更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pp; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=sp; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}




