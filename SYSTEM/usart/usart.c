#include "sys.h"
#include "usart.h"	
#include "system.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_UART4_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
	UART4->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_UART4_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOA9����ΪUART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA10����ΪUART4
	
	//UART4�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(UART4, USART_FLAG_TC);
	
#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//UART4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void UART4_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART4);//(UART4->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
} 





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


#endif	

 



