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



#include "Bsp_usart4.h"
#include "Driver_Remote.h"

/**
  * @brief	:usart4总线初始化
  * @note   :
  * @param	:Baud,波特率
  * @return	:void
  *
  * @date   :2018/03/27
  * @design :
  **/
void UART4_Init(u32 Baud)
{
	GPIO_InitTypeDef	gpio;
	USART_InitTypeDef	usart;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
 
	/*usart gpio init config*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);//PB10:TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);//PB11:RX	

	gpio.GPIO_Pin   = GPIO_Pin_0;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin   = GPIO_Pin_1; 
	gpio.GPIO_OType = GPIO_OType_OD;  
	gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOA, &gpio); 

	/*usart init config*/
	usart.USART_BaudRate   = Baud;//波特率设置
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits   = USART_StopBits_1;
	usart.USART_Parity     = USART_Parity_No; 
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(UART4, &usart); 
							
	/*usart receive and transmit interrupt config*/
	USART_Cmd(UART4, ENABLE);
}

/**
  * @brief	:printf重载
  * @note   :调用printf函数必须，用户直接调用printf
  * @param	:NULL!
  * @return	:void
  *
  * @date   :2018/03/27
  * @design :
  **/
//#if USART_PRINTF_EN_ID == 4
    ////各个串口库函数中均含有此代码，视使用情况使用！
    int fputc(int chda, FILE *f)
    {
        USART_SendData(UART4, (u8)chda);
        while(!(UART4->SR&USART_FLAG_TXE));
        return chda;
    }
//#endif



/**
  * @brief	:uart4发送一个字符
  * @note   :
  * @param	:Schar,待发送的字符
  * @return	:void
  *
  * @date   :2016/04/01
  * @design :
  **/
void UART4_SendChar(u8 Schar)
{
	USART_SendData(UART4, Schar);
	while(!(UART4->SR&USART_FLAG_TXE));
	
//	while(!USART_GetFlagStatus(UART4, USART_FLAG_TXE));
}



/**
  * @brief	:完整发送非char类型数据(非printf发送！)
  * @note   :无论何种类型数据，本函数采用按字节分割完全发送的思路，即原样发送！
			 举例详细说明：
			 发送一个float型数据data，data在内存中占用4个字节(由sizeof(data)函数确定)
			 调用为：
			 USART_3_SendData((char *)(&data), sizeof(data));
  * @param	:*Data,非char类型数据
			  Size,Data在内存中占用内存长度
  * @return	:void
  *
  * @date   :2016/11/06
  * @design :
  **/
void UART4_SendData(char *Data, u8 Size)
{
	u8 inc=0;
	
	for(inc=0;inc<Size;inc++)
	{
		UART4_SendChar(*(Data+inc));
	}
}





//////////////////////////////////////////////////////////////////////////////////
//                                 中断服务程序
//////////////////////////////////////////////////////////////////////////////////

/**
  * @brief	:
  * @note   :--串口3中断，备用串口通信通道
  * @date   :2016/12/18
  * @design :
  **/
	u8 Res[18];
    u8 number=0;
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){        
//        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
//        USART_ClearFlag(UART4, USART_FLAG_RXNE);
          Res[number] =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
//		printf("%d=%d\r\n",number,Res[number]);
		      number++;
		if(number==18){
			DBusDataDec(Res);
			number=0;
		}
        
    }
}




///**
//  * @brief	:发送到山外上位机在PC端显示波形(char型数据)
//  * @note   :
//  * @param	:*CharData,波形数据(float型数据) 
//			  Length  ,要显示的波形长度(方便上位机端设置)
//  * @return	:void
//  *
//  * @date   :2016/11/10
//  * @design :
//  **/
//void shanwai_displaywave0(s8 *CharData, u8 Length)
//{
//	u8 i=0;
//	
//	UART4_SendChar(0x03);
//	UART4_SendChar(0xFC);
//	
//	for(i=0;i<Length;i++)
//	{
//		UART4_SendChar(*(CharData+i));
//	}

//	UART4_SendChar(0xFC);
//	UART4_SendChar(0x03);
//}



///**
//  * @brief	:发送到山外上位机在PC端显示波形(int型数据)
//  * @note   :
//  * @param	:*IntData,波形数据(int型数据) 
//			  Length ,要显示的波形长度(方便上位机端设置)
//  * @return	:void
//  *
//  * @date   :2016/11/10
//  * @design :
//  **/
void shanwai_displaywave1(s16 *IntData, u8 Length)
{
	u8 i=0;
	
	UART4_SendChar(0x03);
	UART4_SendChar(0xFC);
	
	for(i=0;i<Length;i++)
	{
		UART4_SendData((char *)(IntData+i), sizeof(*(IntData+i)));
	}

	UART4_SendChar(0xFC);
	UART4_SendChar(0x03);
}



///**
//  * @brief	:发送到山外上位机在PC端显示波形(float型数据)
//  * @note   :
//  * @param	:*FloatData,波形数据(float型数据) 
//			  Length   ,要显示的波形长度(方便上位机端设置)
//  * @return	:void
//  *
//  * @date   :2016/11/10
//  * @design :
//  **/
float FloatWaveData[10];
void shanwai_displaywave2(float *FloatData, u8 Length)
{
	u8 i=0;
	
	UART4_SendChar(0x03);
	UART4_SendChar(0xFC);
	
	for(i=0;i<Length;i++)
	{
		UART4_SendData((char *)(FloatData+i), sizeof(*(FloatData+i)));
	}

	UART4_SendChar(0xFC);
	UART4_SendChar(0x03);
}

