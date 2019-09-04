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



#include "Bsp_usart2.h"//包含接收机数据内存定义


/**
  * @brief	:遥控器接收初始化(usart2)
  * @note   :--遥控器接收，仅使用PD3(但PD5不能另作他用，否则会错误)！
			 --使用DMA(DMA1_Stream5)来接收数据
  * @param	:Baud  ,波特率
			 NvicPP,抢占优先级
			 NvicSP,响应优先级
  * @return	:void
  *
  * @date   :2016/09/09
  * @design :梁宗南(LJI)
  **/
u8 sbus_rx_buffer[18];//为接收数据开辟的内存空间
void USART2_Init(u32 Baud, u8 NvicPP, u8 NvicSP)
{
	GPIO_InitTypeDef	gpio;
	USART_InitTypeDef	usart;
	NVIC_InitTypeDef	nvic;	
    DMA_InitTypeDef		dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);
	
	/*usart gpio init config*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//PD5:TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);//PD6:RX	

	gpio.GPIO_Pin   = GPIO_Pin_2;
	gpio.GPIO_Mode  = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin   = GPIO_Pin_3;  
	gpio.GPIO_OType = GPIO_OType_OD;  
	gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOA, &gpio); 
	
	/*usart_dma init config*/
	DMA_Cmd(DMA1_Stream5, DISABLE);//先失能
	while(DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);//等待DMA可配置	

    DMA_DeInit(DMA1_Stream5);

    dma.DMA_Channel             = DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr  = (uint32_t)(&USART2->DR);
    dma.DMA_Memory0BaseAddr     = (uint32_t)sbus_rx_buffer;
	dma.DMA_DIR                 = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize          = 18;	
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode                = DMA_Mode_Circular;
    dma.DMA_Priority            = DMA_Priority_VeryHigh;
	
    //指定如果FIFO模式或直接模式将用于指定的流_不使能FIFO模式
	dma.DMA_FIFOMode            = DMA_FIFOMode_Disable;
    //指定了FIFO阈值水平 
	dma.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;
	//指定的Burst转移配置内存传输
    dma.DMA_MemoryBurst         = DMA_MemoryBurst_Single;
	//指定的Burst转移配置外围转移
    dma.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
	
    DMA_Init(DMA1_Stream5, &dma);
	DMA_Cmd(DMA1_Stream5, ENABLE);//再使能	

	/*dma channel nvic config*/
	nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
	nvic.NVIC_IRQChannelSubPriority        = NvicSP;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
	
	/*usart init config*/
	usart.USART_BaudRate   = Baud;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits   = USART_StopBits_1;
	usart.USART_Parity     = USART_Parity_Even;//偶校验 
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2, &usart); 
		
	/*usart receive and transmit interrupt config*/
	USART_Cmd(USART2, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);//usart2 挂接dma
}









