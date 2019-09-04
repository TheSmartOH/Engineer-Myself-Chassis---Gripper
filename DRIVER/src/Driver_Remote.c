/**
  ***************************************************************************************
  * @file    Driver_Remote.c
  * @author  �㿭��
  * @version V0.0.0
  * @date    2017.02.08
  * @brief   ����ң�����ݣ����ṩ��غ���������ģ�����ң�ص��������,�ô���ֻ�������ң�����ݣ�
  *		�����Լ���̨��������ڵ����Լ���̨ģ���н��д���
  *			�ṩ����ң��ͨ��s1��s2��funtion
  *  
  ***************************************************************************************
  * @attention
  *			��ֲ������
  *					1.��ң�����ݽ��ղ��ֽ�ң�ص����ݴ��ݽ�����Ȼ��ֻҪ�������function
  *				����ȡ���ݼ��ɣ�
  *					2.���ֿ���������������ͬ���Ը���ʵ��������������и���
  *					
  *			ʹ�÷�����
  *					DBusDataDec(sbus_rx_buffer);
  *					֮�����к���ֱ�ӵ��ü���
  *
  *		����1��ң�����ݽ������ڵ��жϵ����ȼ��Ƚϸߣ���ʱ����һ��������֣�������
  *		�����ж��ﴦ�����ݣ���������ʹ��ң���е����ݣ�������ʱ��ң�ؽ����жϱ���������ʼ
  *		����ң�����ݣ����֮���ٴη���ԭ�㣬��ʱ�����Ѿ������˸ı䣬��ô�᲻��Կ��Ʋ���
  *		Ӱ�죿����Ŀǰ��ʹ��������������Ӱ����Ժ��Բ��ƣ�
  *
  ***************************************************************************************  
  */
#include "Driver_Remote.h"
#include "stdio.h"
#define RATEMAX  (0XFFFFu)

_E_ShootState			shootState;
_E_FrictionWheelState	wheelState;
_E_ShootHeat			shootHeat;
//_E_ShootConfirm			shootconfirm;

u8 last_CHasssic_sym=0,Trail_status=0,Give_Bullet_status=0,_Bullet_Sheng=0,switch_Mode=0;
u8 SelfSpin_Mark,CHasssic_sym,Get_ullet,CLOUD_State,Get_Ballet_symbol;

static u16 remoteRate = 0;                                                         //֡��ͳ��
u8 Get_Ballet=0,Reset_Ballet=0,Mode_Choice=0;
DBusDataType dBusData;                                                             //ң�ؽ��յ�������

/*
 ****************************************************************************************
 *	��    �֣�DBusDataDec
 *	��    ����u8 recData[]
 *	����ֵ  ��void
 *	��������������ң������
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *
 ****************************************************************************************
*/
void DBusDataDec(u8 recData[])
{
	dBusData.rc.ch0 = (recData[0]|recData[1]<<8)&0x07ff;
	dBusData.rc.ch1 = ((recData[1]>>3)|(recData[2]<<5))&0x07ff;
	dBusData.rc.ch2 = ((recData[2]>>6)|(recData[3]<<2)|(recData[4]<<10))&0x07ff;
	dBusData.rc.ch3 = ((recData[4]>>1)|(recData[5]<<7))&0x07ff;

	dBusData.rc.s1 = ((recData[5]>>4)&0x000C)>>2;//!< Left Switch
	dBusData.rc.s2 = ((recData[5]>>4)&0x0003);   //!< Right Switch

	dBusData.mouse.x = recData[6]|(recData[7]<<8);  //!< Mouse X axis
	dBusData.mouse.y = recData[8]|(recData[9]<<8);  //!< Mouse Y axis
	dBusData.mouse.z = recData[10]|(recData[11]<<8);//!< Mouse Z axis
	dBusData.mouse.press_l = recData[12];//!< Mouse Left Is Press
	dBusData.mouse.press_r = recData[13];//!< Mouse Right Is Press

	dBusData.key.v = recData[14]|(recData[15] << 8);

	
//	remoteRate++;                                                                     //֡��ͳ��
//	if(remoteRate == RATEMAX) remoteRate = RATEMAX;                                   //��ֹ���
	dBusData.rc_ok = dBusData.rc.s1 ? 1 : 0; 
	dBusData.rc_ok = dBusData.rc.s2 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch0 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch1 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch2 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch3 ? dBusData.rc_ok : 0; 	
}


/*
 ****************************************************************************************
 *	��    �֣�DBusDataDec
 *	��    ����u8 recData[]
 *	����ֵ  ��void
 *	��������������ң������
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *			
 ****************************************************************************************
*/

u16 GetRemoteRate(void)
{
	return remoteRate;
}

void ClearRemoteRate(void)
{
	remoteRate = 0;
}
/*
 ****************************************************************************************
 *	��    �֣�GetRemoteData
 *	��    ����void
 *	����ֵ  ��RemoteType*
 *	�������������ң���������ݣ�ȥ�������������ݣ�
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *			
 ****************************************************************************************
*/


_RemoteStruct* GetRemoteData(void)
{
	return &(dBusData.rc);
}


/*
 ****************************************************************************************
 *	��    �֣�GetKeyData ,GetMouseData
 *	��    ����void
 *	����ֵ  ��KeyType*,MouseType*
 *	������������ü�������ݣ�ȥ��ң������������ݣ�
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *			
 ****************************************************************************************
*/

_KeyStruct* GetKeyData(void)
{
	return &(dBusData.key);
}

_MouseStruct* GetMouseData(void)
{
	return &(dBusData.mouse);
}

u8 GetDbusData(void)
{
	return dBusData.rc_ok;
}
	


/*
 ****************************************************************************************
 *	��    �֣�GetS2Mode
 *	��    ����void
 *	����ֵ  ��u8
 *	�������������ң����s2ͨ����״̬,
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *			����ͨ��s2��Ҫ��4��ѡ�񣺹ػ����Զ������ң�أ����󣻵���ֻ��3��λ�ã����ʹ������
 *		��ͨ��s1��������Ĺ��̣��ᵼ�²��˵�λ�ò�������������Բ���s2��������һ������
 *
 *			2017.02.09������������⣬������һ���µ����⣬����ң�غͼ���֮���л���ʱ�����һ��
 *		ʱ��᷵���Զ������ֵ
 *			2017.2.13�����20170209������
 *
 *			����ֵ����								����	
 *			SHUTDOWN						���ظ�ֵӦ���ùػ�
 *			AUTO_CONTROL					���ظ�ֵӦ�����Զ����
 *			KEYMOUSE_CONTROL				���ظ�ֵӦ���ü���ģʽ
 *			REMOTE_CONTROL					���ظ�ֵӦ����ң��ģʽ
 *
 *			ʹ�÷�����
 *			switch(GetS2Mode()){
 *				case SHUTDOWN:printf("shutdown\n");break;
 *				case AUTO_CONTROL:printf("autoControl\n");break;
 *				case KEYMOUSE_CONTROL:printf("keymouse\n");break;
 *				case REMOTE_CONTROL:printf("remote\n");break;
 *			}
 *			
 ****************************************************************************************
*/
static u8 s2ValueCurrent = 0;                                                   //���ڵ�s2ֵ
static u8 s2ValueLast = 0;                                                      //�ϴε�s2ֵ
//static u8 s2Changed = 0;
//static u8 firstInAuto = 1;
static u8 s2Mode;
u8 GetS2Mode(void)
{
	s2ValueLast = s2ValueCurrent;//������ʷ
	s2ValueCurrent = dBusData.rc.s2;//����
//	printf("getmode %d    %d    %d    %d\r\n", dBusData.rc.s2,dBusData.rc.ch0,dBusData.rc.s1,dBusData.rc.ch3);	
	//���˴���2��3ʱ�ֱ�Ϊ�ػ������Զ����
switch(s2ValueLast << 2 | s2ValueCurrent)
	{
		case REMOTE_SWITCH_CHANGE_1TO3:
			Trail_status++;
			break;
		case REMOTE_SWITCH_CHANGE_2TO3:
		CHasssic_sym = ~CHasssic_sym; 		
            break;			
		case REMOTE_SWITCH_CHANGE_3TO1:
			
			break;
		case REMOTE_SWITCH_CHANGE_3TO2:
			
			break;
	}	
	return s2Mode;
}

/*
 ****************************************************************************************
 *	��    �֣�GetS1Mode
 *	��    ����void
 *	����ֵ  ��FrictionWheelState_e
 *	��������������s1�������״̬
 *	����ʱ�䣺2017.02.09 �㿭��
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�ʱ�䣬����
 *		���ı�ȫ�ֱ���wheelState�Լ�shootState	20170213
 *
 *
 *
 ****************************************************************************************
*/
static u8 s1ValueCurrent = 0,s1ValueLast = 0;
//static u8 s1Changed = 0;
//static u8 isWheelTurning = 0;
void GetS1Mode(void)
{
	s1ValueLast = s1ValueCurrent;
	s1ValueCurrent = dBusData.rc.s1;
	switch(s1ValueLast << 2 | s1ValueCurrent)
	{
		case REMOTE_SWITCH_CHANGE_1TO3:
			Get_Ballet=1;
			break;
		case REMOTE_SWITCH_CHANGE_2TO3:
			Give_Bullet_status++;
      break;
		case REMOTE_SWITCH_CHANGE_3TO1:
		    break;
		case REMOTE_SWITCH_CHANGE_3TO2:
			break;
	}
}









