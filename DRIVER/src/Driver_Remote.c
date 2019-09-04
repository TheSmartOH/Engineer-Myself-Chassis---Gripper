/**
  ***************************************************************************************
  * @file    Driver_Remote.c
  * @author  茹凯琪
  * @version V0.0.0
  * @date    2017.02.08
  * @brief   解析遥控数据，并提供相关函数供其他模块调用遥控的相关数据,该代码只负责解析遥控数据，
  *		底盘以及云台相关数据在底盘以及云台模块中进行处理；
  *			提供解析遥控通道s1，s2的funtion
  *  
  ***************************************************************************************
  * @attention
  *			移植条件：
  *					1.在遥控数据接收部分将遥控的数据传递进来，然后只要调用相关function
  *				来获取数据即可；
  *					2.部分可能由于命名规则不同可以根据实际情况对命名进行更改
  *					
  *			使用范例：
  *					DBusDataDec(sbus_rx_buffer);
  *					之后所有函数直接调用即可
  *
  *		问题1：遥控数据解析所在的中断的优先级比较高，这时就有一个问题出现，当正在
  *		其他中断里处理数据，并且正在使用遥控中的数据，但是这时候遥控接收中断被触发，开始
  *		解析遥控数据，完成之后再次返回原点，这时数据已经发生了改变，那么会不会对控制产生
  *		影响？（就目前的使用情况来看，这个影响可以忽略不计）
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

static u16 remoteRate = 0;                                                         //帧率统计
u8 Get_Ballet=0,Reset_Ballet=0,Mode_Choice=0;
DBusDataType dBusData;                                                             //遥控接收到的数据

/*
 ****************************************************************************************
 *	名    字：DBusDataDec
 *	参    数：u8 recData[]
 *	返回值  ：void
 *	功能描述：解析遥控数据
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
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

	
//	remoteRate++;                                                                     //帧率统计
//	if(remoteRate == RATEMAX) remoteRate = RATEMAX;                                   //防止溢出
	dBusData.rc_ok = dBusData.rc.s1 ? 1 : 0; 
	dBusData.rc_ok = dBusData.rc.s2 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch0 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch1 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch2 ? dBusData.rc_ok : 0; 
	dBusData.rc_ok = dBusData.rc.ch3 ? dBusData.rc_ok : 0; 	
}


/*
 ****************************************************************************************
 *	名    字：DBusDataDec
 *	参    数：u8 recData[]
 *	返回值  ：void
 *	功能描述：解析遥控数据
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
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
 *	名    字：GetRemoteData
 *	参    数：void
 *	返回值  ：RemoteType*
 *	功能描述：获得遥控器的数据（去掉键鼠的相关数据）
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *			
 ****************************************************************************************
*/


_RemoteStruct* GetRemoteData(void)
{
	return &(dBusData.rc);
}


/*
 ****************************************************************************************
 *	名    字：GetKeyData ,GetMouseData
 *	参    数：void
 *	返回值  ：KeyType*,MouseType*
 *	功能描述：获得键鼠的数据（去掉遥控器的相关数据）
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
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
 *	名    字：GetS2Mode
 *	参    数：void
 *	返回值  ：u8
 *	功能描述：获得遥控器s2通道的状态,
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *			对于通道s2需要有4种选择：关机，自动射击，遥控，键鼠；但是只有3个位置，如果使用类似
 *		于通道s1控制射击的过程，会导致拨杆的位置不定，所以这里对拨杆s2进行了另一种设置
 *
 *			2017.02.09解决了上述问题，但是有一个新的问题，当在遥控和键鼠之间切换的时候会有一段
 *		时间会返回自动射击的值
 *			2017.2.13解决了20170209的问题
 *
 *			返回值类型								功能	
 *			SHUTDOWN						返回该值应设置关机
 *			AUTO_CONTROL					返回该值应设置自动射击
 *			KEYMOUSE_CONTROL				返回该值应设置键鼠模式
 *			REMOTE_CONTROL					返回该值应设置遥控模式
 *
 *			使用范例：
 *			switch(GetS2Mode()){
 *				case SHUTDOWN:printf("shutdown\n");break;
 *				case AUTO_CONTROL:printf("autoControl\n");break;
 *				case KEYMOUSE_CONTROL:printf("keymouse\n");break;
 *				case REMOTE_CONTROL:printf("remote\n");break;
 *			}
 *			
 ****************************************************************************************
*/
static u8 s2ValueCurrent = 0;                                                   //现在的s2值
static u8 s2ValueLast = 0;                                                      //上次的s2值
//static u8 s2Changed = 0;
//static u8 firstInAuto = 1;
static u8 s2Mode;
u8 GetS2Mode(void)
{
	s2ValueLast = s2ValueCurrent;//保存历史
	s2ValueCurrent = dBusData.rc.s2;//更新
//	printf("getmode %d    %d    %d    %d\r\n", dBusData.rc.s2,dBusData.rc.ch0,dBusData.rc.s1,dBusData.rc.ch3);	
	//拨杆处在2或3时分别为关机或者自动射击
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
 *	名    字：GetS1Mode
 *	参    数：void
 *	返回值  ：FrictionWheelState_e
 *	功能描述：拨杆s1控制射击状态
 *	建立时间：2017.02.09 茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *		仅改变全局变量wheelState以及shootState	20170213
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









