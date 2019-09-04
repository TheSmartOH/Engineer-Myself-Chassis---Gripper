#include "Bsp_beep.h"
#include "stdio.h"
#include "delay.h"
/*
 ******************************************************************************************************************************
 *	名    字：BEEP_Init()
 *	参    数：void
 *	返回值  ：void
 *	功能描述：蜂鸣器PWM输出初始化
 *	建立时间：2017.04.21
 *	备    注：每次修改时请标注修改理由（要写明是什么，为什么，不要写怎么做），时间，姓名
 *			1.蜂鸣器使用PB4引脚，tim3_ch1
 *
 *
 ******************************************************************************************************************************
*/
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(BEEP_TIM_CLOCK,ENABLE);  	//TIM时钟使能    
	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLOCK, ENABLE); 	//使能PORT时钟	

	GPIO_PinAFConfig(BEEP_GPIO_PORT,BEEP_GPIO_AF_PIN,BEEP_GPIO_AF); //GPIO复用为定时器

	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStructure);
	  
	TIM_TimeBaseStructure.TIM_Prescaler=1680-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(BEEP_TIMx,&TIM_TimeBaseStructure);//初始化定时器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC1Init(BEEP_TIMx, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM

	TIM_OC1PreloadConfig(BEEP_TIMx, TIM_OCPreload_Enable);  //使能TIM在CCR1上的预装载寄存器

	TIM_ARRPreloadConfig(BEEP_TIMx,ENABLE);//ARPE使能 

	TIM_Cmd(BEEP_TIMx, ENABLE);  //使能TIM14
	BEEP_OUTPUT = 0;//刚初始化时不让蜂鸣器响
}

/*
 ******************************************************************************************************************************
 *	名    字：PlayMusic()
 *	参    数：void
 *	返回值  ：void
 *	功能描述：蜂鸣器播放音乐
 *	建立时间：2017.04.22
 *	备    注：每次修改时请标注修改理由（要写明是什么，为什么，不要写怎么做），时间，姓名
 *			1.每调用一次就以一个频率播放，循环调用则就可以播放出音乐
 *			2.两次播放之间必须有时间间隔，否则听不出频率变化
 *			3.目前两次播放间隔时100ms
 *
 ******************************************************************************************************************************
*/
////7个音符组成了美妙的音乐
////		1  2  3  4  5  6  7
////低   Do re mi fa so la si do
//static const int melody1[] = {0,261,293,329,349,391,440,493};

//中   Do re mi fa so la si do
static const int melody2[] ={0,523,587,659,698,783,880,987};

////高
//static const int melody3[] ={0,1046,1174,1318,1369,1567,1760,1975};
////歌曲（两只老虎，虽然听起来并不像）
//const static u8 Song[]={1,2,3,1,0,
//						1,2,3,1,0,
//						3,4,5,0,
//						3,4,5,0,
//						5,6,5,4,3,1,0,
//						5,6,5,4,3,1,0,
//						1,5,1,0,
//						1,5,1,0};
////节拍
//const static u8 Beat[]={2,2,2,2,2,
//						2,2,2,2,2,
//						2,2,3,2,
//						2,2,3,2,
//						1,2,2,1,2,2,2,
//						1,2,2,1,2,2,2,
//						2,2,2,2,
//						2,2,2,2};


void PlayMusic(void)
{
	u16 temp = 0;
	static u16 time_stamp = 0;//,last_time = 0;
//	static u8 song=0;
	time_stamp++;
/**********************音乐1*************************************/
//	u16 temp = 0;
//	temp=100000/melody2[Song[song]];//改变频率	
//	BEEP_ARR = temp;
//	if(time_stamp-last_time == Beat[song]){
//		song++;//遍历歌曲
//		last_time=time_stamp;
//		if(song >= 32)song = 0;
//	}
//	BEEP_OUTPUT = temp/2;	
/**********************音乐1---end*************************************/
	if(time_stamp > 7){
		BEEP_ARR = 20;
		BEEP_OUTPUT = 0;
		return;
	}

	temp=100000/melody2[time_stamp%7+1];//改变频率	
	BEEP_ARR = temp;	
	BEEP_OUTPUT = temp/2;
	
}

void MintSound(u8 soundSort)
{
	switch(soundSort){
//		case CLAWINITSOUND:
	}
}



extern void Delay_ms(u16 nms);
void BEEP_PlayMusic(void)
{
		u8 i=0;
    BEEP_Init();
    for(i=0;i<7;i++)
    {
        PlayMusic();
        delay_xms(100);
    }
    BEEP_Close();
}



void BEEP_Close(void)
{
	BEEP_OUTPUT = 0;
}


