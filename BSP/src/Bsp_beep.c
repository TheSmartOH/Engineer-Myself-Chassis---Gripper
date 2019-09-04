#include "Bsp_beep.h"
#include "stdio.h"
#include "delay.h"
/*
 ******************************************************************************************************************************
 *	��    �֣�BEEP_Init()
 *	��    ����void
 *	����ֵ  ��void
 *	����������������PWM�����ʼ��
 *	����ʱ�䣺2017.04.21
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�Ҫд����ʲô��Ϊʲô����Ҫд��ô������ʱ�䣬����
 *			1.������ʹ��PB4���ţ�tim3_ch1
 *
 *
 ******************************************************************************************************************************
*/
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(BEEP_TIM_CLOCK,ENABLE);  	//TIMʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLOCK, ENABLE); 	//ʹ��PORTʱ��	

	GPIO_PinAFConfig(BEEP_GPIO_PORT,BEEP_GPIO_AF_PIN,BEEP_GPIO_AF); //GPIO����Ϊ��ʱ��

	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStructure);
	  
	TIM_TimeBaseStructure.TIM_Prescaler=1680-1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(BEEP_TIMx,&TIM_TimeBaseStructure);//��ʼ����ʱ��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(BEEP_TIMx, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM

	TIM_OC1PreloadConfig(BEEP_TIMx, TIM_OCPreload_Enable);  //ʹ��TIM��CCR1�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(BEEP_TIMx,ENABLE);//ARPEʹ�� 

	TIM_Cmd(BEEP_TIMx, ENABLE);  //ʹ��TIM14
	BEEP_OUTPUT = 0;//�ճ�ʼ��ʱ���÷�������
}

/*
 ******************************************************************************************************************************
 *	��    �֣�PlayMusic()
 *	��    ����void
 *	����ֵ  ��void
 *	������������������������
 *	����ʱ�䣺2017.04.22
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�Ҫд����ʲô��Ϊʲô����Ҫд��ô������ʱ�䣬����
 *			1.ÿ����һ�ξ���һ��Ƶ�ʲ��ţ�ѭ��������Ϳ��Բ��ų�����
 *			2.���β���֮�������ʱ����������������Ƶ�ʱ仯
 *			3.Ŀǰ���β��ż��ʱ100ms
 *
 ******************************************************************************************************************************
*/
////7��������������������
////		1  2  3  4  5  6  7
////��   Do re mi fa so la si do
//static const int melody1[] = {0,261,293,329,349,391,440,493};

//��   Do re mi fa so la si do
static const int melody2[] ={0,523,587,659,698,783,880,987};

////��
//static const int melody3[] ={0,1046,1174,1318,1369,1567,1760,1975};
////��������ֻ�ϻ�����Ȼ������������
//const static u8 Song[]={1,2,3,1,0,
//						1,2,3,1,0,
//						3,4,5,0,
//						3,4,5,0,
//						5,6,5,4,3,1,0,
//						5,6,5,4,3,1,0,
//						1,5,1,0,
//						1,5,1,0};
////����
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
/**********************����1*************************************/
//	u16 temp = 0;
//	temp=100000/melody2[Song[song]];//�ı�Ƶ��	
//	BEEP_ARR = temp;
//	if(time_stamp-last_time == Beat[song]){
//		song++;//��������
//		last_time=time_stamp;
//		if(song >= 32)song = 0;
//	}
//	BEEP_OUTPUT = temp/2;	
/**********************����1---end*************************************/
	if(time_stamp > 7){
		BEEP_ARR = 20;
		BEEP_OUTPUT = 0;
		return;
	}

	temp=100000/melody2[time_stamp%7+1];//�ı�Ƶ��	
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


