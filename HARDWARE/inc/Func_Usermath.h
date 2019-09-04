/**
  ******************************************************************************
  * @file    usermath.h
  * @author  �㿭��
  * @version V0.0.0
  * @date    2016.10.06
  * @brief   ��Ҫ����һЩ��ѧ����ĺ���,�����ṩ�����ӿڣ����Խ���ȡ���������ݽ���
  *			�洢������
  *  
  ******************************************************************************
  * @attention
  *			
  ******************************************************************************  
  */ 
#ifndef _USERMATH_H_
#define	_USERMATH_H_

#include "stm32f4xx.h"
#include "math.h"
#include "arm_math.h"//STM32F4xx�ɿ���FPU������


#define FABS(x) fabs(x)
#define	ABS(x)  ((x)>0?(x):-(x))
#define ARRY_PARAM_INIT  {1,0,10,0,0,0}

typedef struct ArryParam{
	u8 isFirst;	
	u16 length;//��ʾʵʱ�ĳ���
	u16 size;//��ʾ�̶��ߴ�
	s16 currentData;//��ʾ���µ�ֵ
	s16 lastData;//��ʾ���һ��ֵ	
	s16 s16_param[10];
}_ArryParamStruct;



void ArryParam_Init(_ArryParamStruct *variable);

s16 AvgFilter(s16 *param,u8 length);
void PutParam(_ArryParamStruct *variable, s16 param);
void ClearArry(_ArryParamStruct *variable);
s16 GetAvgData(_ArryParamStruct *variable);//�õ�ƽ��ֵ


#endif


