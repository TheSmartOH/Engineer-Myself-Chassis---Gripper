/** 
 ******************************************************************************
  * 
  * USART4串口库，可通过此使用串口4调试程序
  * 使用蓝牙与PC端串口监视器通信的端口
  * 
  * 
  *	创建时间：2018年03月27日
  * 最后更新：
  * 创 建 人：高焱杰
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
void UART4_Init(u32 Baud);//初始化配置
void UART4_SendChar(u8 Schar);//发送单一字符
void UART4_SendData(char *Data, u8 Size);//完整发送非char类型数据(非printf发送！)

//void shanwai_displaywave0(s8 *CharData, u8 Length);//发送到山外上位机在PC端显示波形(char型数据)
//void shanwai_displaywave1(s16 *IntData, u8 Length);//发送到山外上位机在PC端显示波形(int型数据)
//void shanwai_displaywave2(float *FloatData, u8 Length);//发送到山外上位机在PC端显示波形(float型数据)

#endif









