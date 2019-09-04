/** 
 ******************************************************************************
  * 
  * USART4���ڿ⣬��ͨ����ʹ�ô���4���Գ���
  * ʹ��������PC�˴��ڼ�����ͨ�ŵĶ˿�
  * 
  * 
  *	����ʱ�䣺2018��03��27��
  * �����£�
  * �� �� �ˣ����ͽ�
  * 
  ******************************************************************************
***/



#include "Bsp_usart4.h"
#include "Driver_Remote.h"

/**
  * @brief	:usart4���߳�ʼ��
  * @note   :
  * @param	:Baud,������
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
	usart.USART_BaudRate   = Baud;//����������
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
  * @brief	:printf����
  * @note   :����printf�������룬�û�ֱ�ӵ���printf
  * @param	:NULL!
  * @return	:void
  *
  * @date   :2018/03/27
  * @design :
  **/
//#if USART_PRINTF_EN_ID == 4
    ////�������ڿ⺯���о����д˴��룬��ʹ�����ʹ�ã�
    int fputc(int chda, FILE *f)
    {
        USART_SendData(UART4, (u8)chda);
        while(!(UART4->SR&USART_FLAG_TXE));
        return chda;
    }
//#endif



/**
  * @brief	:uart4����һ���ַ�
  * @note   :
  * @param	:Schar,�����͵��ַ�
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
  * @brief	:�������ͷ�char��������(��printf���ͣ�)
  * @note   :���ۺ����������ݣ����������ð��ֽڷָ���ȫ���͵�˼·����ԭ�����ͣ�
			 ������ϸ˵����
			 ����һ��float������data��data���ڴ���ռ��4���ֽ�(��sizeof(data)����ȷ��)
			 ����Ϊ��
			 USART_3_SendData((char *)(&data), sizeof(data));
  * @param	:*Data,��char��������
			  Size,Data���ڴ���ռ���ڴ泤��
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
//                                 �жϷ������
//////////////////////////////////////////////////////////////////////////////////

/**
  * @brief	:
  * @note   :--����3�жϣ����ô���ͨ��ͨ��
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
          Res[number] =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
//		printf("%d=%d\r\n",number,Res[number]);
		      number++;
		if(number==18){
			DBusDataDec(Res);
			number=0;
		}
        
    }
}




///**
//  * @brief	:���͵�ɽ����λ����PC����ʾ����(char������)
//  * @note   :
//  * @param	:*CharData,��������(float������) 
//			  Length  ,Ҫ��ʾ�Ĳ��γ���(������λ��������)
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
//  * @brief	:���͵�ɽ����λ����PC����ʾ����(int������)
//  * @note   :
//  * @param	:*IntData,��������(int������) 
//			  Length ,Ҫ��ʾ�Ĳ��γ���(������λ��������)
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
//  * @brief	:���͵�ɽ����λ����PC����ʾ����(float������)
//  * @note   :
//  * @param	:*FloatData,��������(float������) 
//			  Length   ,Ҫ��ʾ�Ĳ��γ���(������λ��������)
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

