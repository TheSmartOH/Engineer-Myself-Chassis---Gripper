/**
  ******************************************************************************
  * 
  * CAN����������������stm32f4xxϵ�е�Ƭ��
  * ���⺯�����ṩ�ײ�������
  * 
  * ע��2017.06.30�Ա��ļ��������޸ģ�
  *     1������֮ǰ�ĳ�����룬���������豸�������ݡ�ID���ͣ�
  *     2��ɾ��ԭ�еĳ����ļ���������ӱ��ļ����ɣ�
  * 
  *
  *	����ʱ�䣺2016��07��01��
  * �����£�2017��07��09��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#include "Bsp_can.h"//����can���ͽ��ձ��Ķ���


CanRxMsg RxMessage;
CanTxMsg TxMessage;

/**
  * @brief	:can1���߳�ʼ��
  * @note   :--Ƶ��Ϊ1MHz���������ϵ���벻Ҫ�޸ģ�
  * @param	:NvicPP, ��ռ���ȼ�
			 NvicSP, ��Ӧ���ȼ�
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
    filter.CAN_FilterIdHigh = 0x0000;		//��־λ��16λ
    filter.CAN_FilterIdLow  = 0x0000;		//��־λ��16λ
    filter.CAN_FilterMaskIdHigh = 0x0000;	//����λ��16λ
    filter.CAN_FilterMaskIdLow  = 0x0000;	//����λ��16λ
    filter.CAN_FilterFIFOAssignment = 0; 	//�˲���0����FIFO 0
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
  * @brief	:can2���߳�ʼ��
  * @note   :--Ƶ��Ϊ1MHz���������ϵ���벻Ҫ�޸ģ�
  * @param	:NvicPP, ��ռ���ȼ�
			 NvicSP, ��Ӧ���ȼ�
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
    filter.CAN_FilterIdHigh = 0x0000;//��־λ��16λ
    filter.CAN_FilterIdLow  = 0x0000;//��־λ��16λ
    filter.CAN_FilterMaskIdHigh = 0x0000;//����λ��16λ
    filter.CAN_FilterMaskIdLow  = 0x0000;//����λ��16λ
    filter.CAN_FilterFIFOAssignment = 0; //�˲���14����FIFO0
    filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&filter);
	
    /*can receive and transmit config*/
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
}



/**
  * @brief	:CAN���߷������ݱ���
  * @note   :--���ͳ���Ϊ 8 Bytes��
  * @param	:CANx , ѡ�õ�CAN�豸(CAN1 or CAN2)
             ID   , Ŀ��ID
             *Data, ���͵���������ָ��
  * @return	:void
  *
  * @date   :2016/03/02
  * @design :������((SERILJI))
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
  * @brief	:ͨ��can1���߷��͵��̵������
  * @note   :--
  * @param	:ChassisLF, ������ǰ��(1���֣�CANID=0x201)
			 ChassisLB, ���������(2���֣�CANID=0x202)
			 ChassisRF, ������ǰ��(3���֣�CANID=0x203)
			 ChassisRB, �����Һ���(4���֣�CANID=0x204)
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
  * @brief	:ͨ��can1���߷������İ�������
  * @note   :--��������˵�������İ�ID����Ϊ0x002��
             --ע�ⷢ�͵�����˳��͹ٷ����Ͳ�ͬ��
  * @param	:MBoard1H, 1�����İ��һ������(��Ӧ��ǰ�֣�1����)
			 MBoard1L, 1�����İ�ڶ�������(��Ӧ��ǰ�֣�2����)
			 MBoard2H, 2�����İ��һ������(��Ӧ����֣�3����)
			 MBoard2L, 2�����İ�ڶ�������(��Ӧ�Һ��֣�4����)
  * @return	:u8, ���ķ���״̬
  *
  * @date   :2017/07/09
  * @design :
  **/
u8 CAN1_SendMBoardMail(s16 MBoard1H, s16 MBoard1L, s16 MBoard2H, s16 MBoard2L)
{
    u16 i=0; 
    u8 mailBox;//���ķ��͵������
    
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
    while( (CAN_TransmitStatus(CAN1, mailBox) == CAN_TxStatus_Failed) && (i < 0xFFF) )//�ȴ����ͽ���
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
//  u8 mailBox;//���ķ��͵������
    
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



