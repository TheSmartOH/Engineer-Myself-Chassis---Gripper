/**
  ******************************************************************************
  * 
  * CAN驱动，本部分用于stm32f4xx系列单片机
  * 本库函数仅提供底层驱动！
  * 
  * 注：2017.06.30对本文件进行了修改：
  *     1、兼容之前的程序代码，增添任意设备任意数据、ID发送；
  *     2、删除原有的程序文件，重新添加本文件即可；
  * 
  *
  *	创建时间：2016年07月01日
  * 最后更新：2017年07月09日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#include "Bsp_can.h"//包含can发送接收报文定义


CanRxMsg RxMessage;
CanTxMsg TxMessage;

/**
  * @brief	:can1总线初始化
  * @note   :--频率为1MHz，相关配置系数请不要修改！
  * @param	:NvicPP, 抢占优先级
			 NvicSP, 响应优先级
  * @return	:void
  *
  * @date   :2016/07/01
  * @design :
  **/
void CAN1_Init(u8 NvicPP, u8 NvicSP)
{
    CAN_InitTypeDef			can;
    CAN_FilterInitTypeDef	filter;
    GPIO_InitTypeDef		gpio;
	NVIC_InitTypeDef  		nvic;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	 
    /*can bus init config*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);//PA11:RX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);//PA12:TX

	gpio.GPIO_Pin  = GPIO_Pin_0
				   | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &gpio);

	CAN_DeInit(CAN1);
    CAN_StructInit(&can);

    /*can bus init config*/
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = DISABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
	
    //CAN BaudRate 42/(1+9+4)/3=1Mbps
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1  = CAN_BS1_9tq;
    can.CAN_BS2  = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;
    CAN_Init(CAN1, &can);

    /*can bus filter init config*/
    filter.CAN_FilterNumber = 0;
    filter.CAN_FilterMode   = CAN_FilterMode_IdMask;
    filter.CAN_FilterScale  = CAN_FilterScale_32bit;
    filter.CAN_FilterIdHigh = 0x0000;		//标志位高16位
    filter.CAN_FilterIdLow  = 0x0000;		//标志位低16位
    filter.CAN_FilterMaskIdHigh = 0x0000;	//屏蔽位高16位
    filter.CAN_FilterMaskIdLow  = 0x0000;	//屏蔽位低16位
    filter.CAN_FilterFIFOAssignment = 0; 	//滤波器0连接FIFO 0
    filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&filter);

    /*can receive nvic config*/
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
    nvic.NVIC_IRQChannelSubPriority        = NvicSP;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
    /*can receive and transmit config*/
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}


/**
  * @brief	:can2总线初始化
  * @note   :--频率为1MHz，相关配置系数请不要修改！
  * @param	:NvicPP, 抢占优先级
			 NvicSP, 响应优先级
  * @return	:void
  *
  * @date   :2016/07/01
  * @design :
  **/
void CAN2_Init(u8 NvicPP, u8 NvicSP)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
    /*can bus init config*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);//PB12:RX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);//PB13:TX

    gpio.GPIO_Pin  = GPIO_Pin_12
				   | GPIO_Pin_13 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);

    /*can receive nvic config*/
    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
    nvic.NVIC_IRQChannelSubPriority        = NvicSP;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    /*can bus init config*/
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = DISABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
	
    //CAN BaudRate 42/(1+9+4)/3=1Mbps
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1  = CAN_BS1_9tq;
    can.CAN_BS2  = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;
    CAN_Init(CAN2, &can);

    /*can bus filter init config*/
    filter.CAN_FilterNumber = 14;
    filter.CAN_FilterMode   = CAN_FilterMode_IdMask;
    filter.CAN_FilterScale  = CAN_FilterScale_32bit;
    filter.CAN_FilterIdHigh = 0x0000;//标志位高16位
    filter.CAN_FilterIdLow  = 0x0000;//标志位低16位
    filter.CAN_FilterMaskIdHigh = 0x0000;//屏蔽位高16位
    filter.CAN_FilterMaskIdLow  = 0x0000;//屏蔽位低16位
    filter.CAN_FilterFIFOAssignment = 0; //滤波器14连接FIFO0
    filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&filter);
	
    /*can receive and transmit config*/
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
}



/**
  * @brief	:CAN总线发送数据报文
  * @note   :--发送长度为 8 Bytes！
  * @param	:CANx , 选用的CAN设备(CAN1 or CAN2)
             ID   , 目标ID
             *Data, 发送的数据数组指针
  * @return	:void
  *
  * @date   :2016/03/02
  * @design :梁宗南((SERILJI))
  **/
void CAN_SendMail(CAN_TypeDef* CANx, u32 ID, u8* Data)
{
	CanTxMsg txMessage;

	txMessage.StdId   = ID;
	txMessage.IDE	  = CAN_Id_Standard;
	txMessage.RTR	  = CAN_RTR_Data;
	txMessage.DLC	  = 8;
	
	txMessage.Data[0] = *(Data + 0);
	txMessage.Data[1] = *(Data + 1);
	txMessage.Data[2] = *(Data + 2);
	txMessage.Data[3] = *(Data + 3);
	txMessage.Data[4] = *(Data + 4);
	txMessage.Data[5] = *(Data + 5);
	txMessage.Data[6] = *(Data + 6);
	txMessage.Data[7] = *(Data + 7);
	
	CAN_Transmit(CANx, &txMessage);
}



/**
  * @brief	:通过can1总线发送地盘电机报文
  * @note   :--
  * @param	:ChassisLF, 地盘左前轮(1号轮，CANID=0x201)
			 ChassisLB, 地盘左后轮(2号轮，CANID=0x202)
			 ChassisRF, 地盘右前轮(3号轮，CANID=0x203)
			 ChassisRB, 地盘右后轮(4号轮，CANID=0x204)
  * @return	:void
  *
  * @date   :2016/07/01
  * @design :
  **/
void CAN1_SendChassisMail(s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB)
{
	TxMessage.StdId   = 0x200;
	TxMessage.IDE	  = CAN_Id_Standard;
	TxMessage.RTR	  = CAN_RTR_Data;
	TxMessage.DLC	  = 0x08;
	
	TxMessage.Data[0] = (ChassisLF&0xFF00)>>8;
	TxMessage.Data[1] =  ChassisLF&0x00FF;
	TxMessage.Data[2] = (ChassisLB&0xFF00)>>8;
	TxMessage.Data[3] =  ChassisLB&0x00ff;
	TxMessage.Data[4] = (ChassisRF&0xFF00)>>8;
	TxMessage.Data[5] =  ChassisRF&0x00FF;
	TxMessage.Data[6] = (ChassisRB&0xFF00)>>8;
	TxMessage.Data[7] =  ChassisRB&0x00FF;
	
	CAN_Transmit(CAN1, &TxMessage);
}

void CAN2_SendChassisMail(u32 ID,s16 ChassisLF, s16 ChassisLB, s16 ChassisRF, s16 ChassisRB)
{
	TxMessage.StdId   = (ID == 0x1FF) ? 0x1FF : 0x200;
	TxMessage.IDE	  = CAN_Id_Standard;
	TxMessage.RTR	  = CAN_RTR_Data;
	TxMessage.DLC	  = 0x08;
	
	TxMessage.Data[0] = (ChassisLF&0xFF00)>>8;
	TxMessage.Data[1] =  ChassisLF&0x00FF;
	TxMessage.Data[2] = (ChassisLB&0xFF00)>>8;
	TxMessage.Data[3] =  ChassisLB&0x00ff;
	TxMessage.Data[4] = (ChassisRF&0xFF00)>>8;
	TxMessage.Data[5] =  ChassisRF&0x00FF;
	TxMessage.Data[6] = (ChassisRB&0xFF00)>>8;
	TxMessage.Data[7] =  ChassisRB&0x00FF;
	
	CAN_Transmit(CAN2, &TxMessage);
}

/**
  * @brief	:通过can1总线发送中心板电机报文
  * @note   :--若不特殊说明，中心板ID必须为0x002！
             --注意发送的数据顺序和官方发送不同！
  * @param	:MBoard1H, 1号中心板第一组数据(对应左前轮，1号轮)
			 MBoard1L, 1号中心板第二组数据(对应右前轮，2号轮)
			 MBoard2H, 2号中心板第一组数据(对应左后轮，3号轮)
			 MBoard2L, 2号中心板第二组数据(对应右后轮，4号轮)
  * @return	:u8, 报文发送状态
  *
  * @date   :2017/07/09
  * @design :
  **/
u8 CAN1_SendMBoardMail(s16 MBoard1H, s16 MBoard1L, s16 MBoard2H, s16 MBoard2L)
{
    u16 i=0; 
    u8 mailBox;//报文发送的邮箱号
    
	TxMessage.StdId   = 0x002;
	TxMessage.IDE	  = CAN_Id_Standard;
	TxMessage.RTR	  = CAN_RTR_Data;
	TxMessage.DLC	  = 0x08;
	
	TxMessage.Data[0] = (MBoard1H&0xFF00)>>8;
	TxMessage.Data[1] =  MBoard1H&0x00FF;
	TxMessage.Data[2] = (MBoard1L&0xFF00)>>8;
	TxMessage.Data[3] =  MBoard1L&0x00ff;
	TxMessage.Data[4] = (MBoard2H&0xFF00)>>8;
	TxMessage.Data[5] =  MBoard2H&0x00FF;
	TxMessage.Data[6] = (MBoard2L&0xFF00)>>8;
	TxMessage.Data[7] =  MBoard2L&0x00FF;
	
	mailBox = CAN_Transmit(CAN1, &TxMessage);
    while( (CAN_TransmitStatus(CAN1, mailBox) == CAN_TxStatus_Failed) && (i < 0xFFF) )//等待发送结束
    {        
        i++;
    }
    
    if(i >= 0xFFF){
        return 1;
    }
    
    return 0;	
}
u8 CAN1_SendManifold(u8 SendDate[])
{
//	u16 i=0; 
//  u8 mailBox;//报文发送的邮箱号
    
	TxMessage.StdId   = 0x301;
	TxMessage.IDE	  = CAN_Id_Standard;
	TxMessage.RTR	  = CAN_RTR_Data;
	TxMessage.DLC	  = 0x08;
	
	TxMessage.Data[0] = SendDate[0];
	TxMessage.Data[1] = SendDate[1];
	TxMessage.Data[2] = SendDate[2];
	TxMessage.Data[3] = SendDate[3];
	TxMessage.Data[4] = SendDate[4];
	TxMessage.Data[5] = SendDate[5];
	TxMessage.Data[6] = SendDate[6];
	TxMessage.Data[7] = SendDate[7];
	
	CAN_Transmit(CAN2, &TxMessage);
	return 0;
}


void Cloud_SendControlMail(s16 CloudYaw, s16 CloudPitch)
{
	TxMessage.StdId   = 0x1FF;
	TxMessage.IDE	  = CAN_Id_Standard;
	TxMessage.RTR	  = CAN_RTR_Data;
	TxMessage.DLC	  = 0x08;
	
	TxMessage.Data[0] = (CloudYaw&0xFF00)>>8;
	TxMessage.Data[1] =  CloudYaw&0x00FF;
	TxMessage.Data[2] = (CloudPitch&0xFF00)>>8;
	TxMessage.Data[3] =  CloudPitch&0x00ff;
	TxMessage.Data[4] =  0;
	TxMessage.Data[5] =  0;
	TxMessage.Data[6] =  0;
	TxMessage.Data[7] =  0;
	
	CAN_Transmit(CAN1, &TxMessage);
}



