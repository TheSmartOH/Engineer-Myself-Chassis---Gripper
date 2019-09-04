/**
  ******************************************************************************
  * 
  * 键鼠操作命令提取（兼容英雄车、工程车、步兵车的大部分操作！）
  *
  * 注：
  *     1、当前只为步兵车提供控制操作！
  *     2、本库中的函数至今标志的置\复位，不进行实际的操作！
  *   
  * 
  *	创建时间：2017年04月27日
  * 最后更新：2017年04月27日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#include "Driver_KeyMouse.h"

#include "usart.h"
//static void ServoOpenOrNot(u8 NewState);//判断是否开启弹仓盖
//static void ServoAutoDelayClose(void);//弹仓盖延时关闭
//static void SelfSpinStateGet(u8 KeyState);//底盘自旋状态提取
//static void FireShootGet(u8 NewState);//判断摩擦轮是否开启
//static void FireShootDartle(u8 KeyState);//判断连发
//static void FireShootSpeed(u8 KeyState);//控制摩擦轮转速
//static void LowSpeedStateGet(u8 KeyState);//高低速状态提取
//static void BigOperatorGet(u8 KeyState);//大符模式切换
//static void SmallOperatorGet(u8 KeyState);//小符模式切换
//static void Trail_Get(u8 NewState);
//static void Chassic_Get(u8 NewState);

_KeyMouseFlagStruct KeyMouseFlag;//键鼠所对应的动作标志
u8 KeyValue[16];//键鼠对应按键值的状态，1表示该按键按下 0未按下
_Operator_Get Bigoperator;
_Operator_Get Smalloperator;

//u8 SelfSpin_Mark=0,CHasssic_sym=0;

/**
  * @brief	:获取按键值，并对KeyMouseFlag中对应成员进行赋值
  * @note   :--同时按键状态被记录！
  * @param	:void
  * @return	:void
  *
  * @date   :2017/04/27
  * @design :
  **/
void GetKeyValue(void)
{
	_KeyStruct * key = GetKeyData();//获取键盘按键值
	
	u8 i=0;
	for(i=0;i<16;i++)
	{
		KeyValue[i] = (key->v >> i) & 0x0001;//判断键盘是否按下！
	}
	if(KEY_VALUE_W)
	{
		KeyMouseFlag.movlr = -1;//底盘前进标记
	}        
	if(KEY_VALUE_S)
	{
		KeyMouseFlag.movlr =  1;//底盘后退标记
	}        
	if(KEY_VALUE_A)
	{
		KeyMouseFlag.movfb = -1;//底盘左移标记
	}        
	if(KEY_VALUE_D)
	{
		KeyMouseFlag.movfb =  1;//底盘前进标记
	}
	if(KEY_VALUE_W == 0 && KEY_VALUE_A == 0 && KEY_VALUE_S == 0 && KEY_VALUE_D == 0)
		KeyMouseFlag.movfb = KeyMouseFlag.movlr = 0;

}


/**
  * @brief	:获取按键值
  * @note   :--同时按键状态被记录！
  * @param	:void
  * @return	:void
  *
  * @date   :2017/04/27
  * @design :
  **/
void GetCloudKey(void){
	
	_KeyStruct *key = GetKeyData();//获取键盘按键值
	
	u8 i=0;
	for(i=0;i<16;i++)
	{
		KeyValue[i] = (key->v >> i) & 0x0001;//判断键盘是否按下！
	}

	if(KEY_VALUE_Q)
	{
		KeyMouseFlag.movrt = -1;//云台左旋
		Bigoperator = UNCHAR_PATTERN;
		Smalloperator = UNCHAR_PATTERN;
	}        
	
	if(KEY_VALUE_E)
	{
		KeyMouseFlag.movrt =  1;//云台右旋
		Bigoperator = UNCHAR_PATTERN;
		Smalloperator = UNCHAR_PATTERN;
	} 
	
}


/**
  * @brief	:按键缓冲区初始化，清零操作
  * @note   :--1、键鼠对应的动作标志(KeyMouseFlag)清零；
             --2、按键值清零，等待下一次按键按下；
             --3、仅复位实时动作，如方向控制的按键！
  * @param	:void
  * @return	:void
  *
  * @date   :2017/04/27
  * @design :
  **/
void KeyStatusReset(void)
{
	u8 i=0;
	for(i=0;i<16;i++){
			KeyValue[i] = 0;//恢复按键状态为未按下
	}

	//注：仅对键盘有关的标志清零复位，对鼠标的无效！！！
	KeyMouseFlag.movfb = 0;
	KeyMouseFlag.movlr = 0; 
	KeyMouseFlag.movrt = 0;
}



/**
  * @brief	:判断取弹
  * @note   :--按下按键开启弹仓盖，延时后关闭，若持续按下，则以最后一次按下推后SERVO_DELAY_CLOSE_TIME秒！
  * @param	:NewState , 新的按键状态
  * @return	:void
  *
  * @date   :2017/07/08
  * @design :
  **/

//#define SERVO_DELAY_CLOSE_TIME          3   //弹仓盖延时关闭延时时间

////static u32 Servo_Open_Time=0;
//static void ServoOpenOrNot(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    
//    if( (NewState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//		Get_Ballet=1;

//    }
//    Last_Key_State = NewState;
//}



///**
//  * @brief	:弹仓盖延时关闭
//  * @note   :--为避免操作手补完弹后忘记关闭弹仓盖，弹仓盖在打开后SERVO_DELAY_CLOSE_TIME秒后自动关闭！
//  * @param	:void
//  * @return	:void
//  *
//  * @date   :2017/07/08
//  * @design :
//  **/
//static void ServoAutoDelayClose(void)
//{        
//    u32 now_time=0;
//    
//    now_time = Get_Time_Micros();
//    if(KeyMouseFlag.servo == 1)//弹仓盖处于开启状态
//    {
//        if((now_time - Servo_Open_Time) >= (SERVO_DELAY_CLOSE_TIME * 1000000))
//        {
//            KeyMouseFlag.servo = 0;//弹仓盖关闭
//        }
//    }
//}



///**
//  * @brief	:底盘自旋状态提取
//  * @note   :--奇次开，偶次关！
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2017/07/09
//  * @design :
//  **/
//static void SelfSpinStateGet(u8 KeyState)
//{
//    static u8 Last_Key_State=0;
//    
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//         SelfSpin_Mark = ~SelfSpin_Mark;
//      }

//    Last_Key_State = KeyState;
//}



///**
//  * @brief	:高低速状态提取
//  * @note   :--奇次低速，偶次高速！
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2017/07/17
//  * @design :
//  **/
//static void LowSpeedStateGet(u8 KeyState)
//{
//    static u8 Last_Key_State=0;
//    
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//        KeyMouseFlag.lowspeed = (KeyMouseFlag.lowspeed == 1) ? 0 : 1;
//    }
//    Last_Key_State = KeyState;
//}



///**
//  * @brief	:选择取弹模式
//  * @note   :
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2017/07/09
//  * @design :
//  **/
//static void FireShootGet(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//          Mode_Choice = ~Mode_Choice; 		
//    }
//	Last_Key_State = NewState;
//}

//static void Trail_Get(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//        Trail_status--;
//    }
//	Last_Key_State = NewState;
//}
//static void Chassic_Get(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//          CHasssic_sym = ~CHasssic_sym; 		
//    }
//	Last_Key_State = NewState;
//}

///**
//  * @brief	:改变发射状态
//  * @note   :默认状态为单发，再按一次为连发
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/
//static void FireShootDartle(u8 KeyState){
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//				Reset_Ballet=1;
//	}
//	Last_Key_State = KeyState;
//}

///**
//  * @brief	:控制摩擦轮速度
//  * @note   :默认状态为正常射速
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/

//static void FireShootSpeed(u8 KeyState){
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//        Give_Bullet_status--;
//	}
//	Last_Key_State = KeyState;
//}



///**
//  * @brief	:按键进入大符模式
//  * @note   :按其他按键时退出打符模式
//  * @param	:KeyState, 按键状态
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/
//static void BigOperatorGet(u8 KeyState){
//	
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//		Bigoperator = ( Bigoperator == CHAR_PATTERN ) ? UNCHAR_PATTERN : CHAR_PATTERN;
//		Smalloperator = UNCHAR_PATTERN;
//	}
//	Last_Key_State = KeyState;
//		
//}

//static void SmallOperatorGet(u8 KeyState){
//	
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//提取按键下降沿
//		Smalloperator = ( Smalloperator == CHAR_PATTERN ) ? UNCHAR_PATTERN : CHAR_PATTERN;
//		Bigoperator = UNCHAR_PATTERN;
//	}
//	Last_Key_State = KeyState;
//		
//}

