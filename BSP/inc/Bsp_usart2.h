/**
  ******************************************************************************
  * 
  * 用于接收机数据接收测试
  *
  * 注：第一版步兵车主控板接收机通道为usart2，PA2/PA3；
  *     第二版步兵车主控板接收机通道为usart2，PD5/PD6；
  * 
  * 
  *	创建时间：2016年09月09日
  * 最后更新：2016年10月05日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#ifndef __BSP_USART2_H__
#define __BSP_USART2_H__



/*Bsp_Usart2 Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "misc.h" 
#include <stdio.h>



/*Bsp_Usart2 Param Define*/
extern u8 sbus_rx_buffer[18];//为接收数据开辟的内存空间



/*Bsp_Usart2 Function Define*/
void USART2_Init(u32 Baud, u8 NvicPP, u8 NvicSP);//接收机接收初始化配置



#endif









