/**
  ******************************************************************************
  * @file    usermath.h
  * @author  茹凯琪
  * @version V0.0.0
  * @date    2016.10.06
  * @brief   主要包含一些数学计算的函数,对外提供函数接口，可以将读取回来的数据进行
  *			存储，计算
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
#include "arm_math.h"//STM32F4xx可开启FPU！！！


#define FABS(x) fabs(x)
#define	ABS(x)  ((x)>0?(x):-(x))
#define ARRY_PARAM_INIT  {1,0,10,0,0,0}

typedef struct ArryParam{
	u8 isFirst;	
	u16 length;//表示实时的长度
	u16 size;//表示固定尺寸
	s16 currentData;//表示最新的值
	s16 lastData;//表示最后一个值	
	s16 s16_param[10];
}_ArryParamStruct;



void ArryParam_Init(_ArryParamStruct *variable);

s16 AvgFilter(s16 *param,u8 length);
void PutParam(_ArryParamStruct *variable, s16 param);
void ClearArry(_ArryParamStruct *variable);
s16 GetAvgData(_ArryParamStruct *variable);//得到平均值


#endif


