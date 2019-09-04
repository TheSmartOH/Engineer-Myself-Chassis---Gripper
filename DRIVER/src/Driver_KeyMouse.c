/**
  ******************************************************************************
  * 
  * �������������ȡ������Ӣ�۳������̳����������Ĵ󲿷ֲ�������
  *
  * ע��
  *     1����ǰֻΪ�������ṩ���Ʋ�����
  *     2�������еĺ��������־����\��λ��������ʵ�ʵĲ�����
  *   
  * 
  *	����ʱ�䣺2017��04��27��
  * �����£�2017��04��27��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#include "Driver_KeyMouse.h"

#include "usart.h"
//static void ServoOpenOrNot(u8 NewState);//�ж��Ƿ������ָ�
//static void ServoAutoDelayClose(void);//���ָ���ʱ�ر�
//static void SelfSpinStateGet(u8 KeyState);//��������״̬��ȡ
//static void FireShootGet(u8 NewState);//�ж�Ħ�����Ƿ���
//static void FireShootDartle(u8 KeyState);//�ж�����
//static void FireShootSpeed(u8 KeyState);//����Ħ����ת��
//static void LowSpeedStateGet(u8 KeyState);//�ߵ���״̬��ȡ
//static void BigOperatorGet(u8 KeyState);//���ģʽ�л�
//static void SmallOperatorGet(u8 KeyState);//С��ģʽ�л�
//static void Trail_Get(u8 NewState);
//static void Chassic_Get(u8 NewState);

_KeyMouseFlagStruct KeyMouseFlag;//��������Ӧ�Ķ�����־
u8 KeyValue[16];//�����Ӧ����ֵ��״̬��1��ʾ�ð������� 0δ����
_Operator_Get Bigoperator;
_Operator_Get Smalloperator;

//u8 SelfSpin_Mark=0,CHasssic_sym=0;

/**
  * @brief	:��ȡ����ֵ������KeyMouseFlag�ж�Ӧ��Ա���и�ֵ
  * @note   :--ͬʱ����״̬����¼��
  * @param	:void
  * @return	:void
  *
  * @date   :2017/04/27
  * @design :
  **/
void GetKeyValue(void)
{
	_KeyStruct * key = GetKeyData();//��ȡ���̰���ֵ
	
	u8 i=0;
	for(i=0;i<16;i++)
	{
		KeyValue[i] = (key->v >> i) & 0x0001;//�жϼ����Ƿ��£�
	}
	if(KEY_VALUE_W)
	{
		KeyMouseFlag.movlr = -1;//����ǰ�����
	}        
	if(KEY_VALUE_S)
	{
		KeyMouseFlag.movlr =  1;//���̺��˱��
	}        
	if(KEY_VALUE_A)
	{
		KeyMouseFlag.movfb = -1;//�������Ʊ��
	}        
	if(KEY_VALUE_D)
	{
		KeyMouseFlag.movfb =  1;//����ǰ�����
	}
	if(KEY_VALUE_W == 0 && KEY_VALUE_A == 0 && KEY_VALUE_S == 0 && KEY_VALUE_D == 0)
		KeyMouseFlag.movfb = KeyMouseFlag.movlr = 0;

}


/**
  * @brief	:��ȡ����ֵ
  * @note   :--ͬʱ����״̬����¼��
  * @param	:void
  * @return	:void
  *
  * @date   :2017/04/27
  * @design :
  **/
void GetCloudKey(void){
	
	_KeyStruct *key = GetKeyData();//��ȡ���̰���ֵ
	
	u8 i=0;
	for(i=0;i<16;i++)
	{
		KeyValue[i] = (key->v >> i) & 0x0001;//�жϼ����Ƿ��£�
	}

	if(KEY_VALUE_Q)
	{
		KeyMouseFlag.movrt = -1;//��̨����
		Bigoperator = UNCHAR_PATTERN;
		Smalloperator = UNCHAR_PATTERN;
	}        
	
	if(KEY_VALUE_E)
	{
		KeyMouseFlag.movrt =  1;//��̨����
		Bigoperator = UNCHAR_PATTERN;
		Smalloperator = UNCHAR_PATTERN;
	} 
	
}


/**
  * @brief	:������������ʼ�����������
  * @note   :--1�������Ӧ�Ķ�����־(KeyMouseFlag)���㣻
             --2������ֵ���㣬�ȴ���һ�ΰ������£�
             --3������λʵʱ�������緽����Ƶİ�����
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
			KeyValue[i] = 0;//�ָ�����״̬Ϊδ����
	}

	//ע�����Լ����йصı�־���㸴λ����������Ч������
	KeyMouseFlag.movfb = 0;
	KeyMouseFlag.movlr = 0; 
	KeyMouseFlag.movrt = 0;
}



/**
  * @brief	:�ж�ȡ��
  * @note   :--���°����������ָǣ���ʱ��رգ����������£��������һ�ΰ����ƺ�SERVO_DELAY_CLOSE_TIME�룡
  * @param	:NewState , �µİ���״̬
  * @return	:void
  *
  * @date   :2017/07/08
  * @design :
  **/

//#define SERVO_DELAY_CLOSE_TIME          3   //���ָ���ʱ�ر���ʱʱ��

////static u32 Servo_Open_Time=0;
//static void ServoOpenOrNot(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    
//    if( (NewState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//		Get_Ballet=1;

//    }
//    Last_Key_State = NewState;
//}



///**
//  * @brief	:���ָ���ʱ�ر�
//  * @note   :--Ϊ��������ֲ��굯�����ǹرյ��ָǣ����ָ��ڴ򿪺�SERVO_DELAY_CLOSE_TIME����Զ��رգ�
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
//    if(KeyMouseFlag.servo == 1)//���ָǴ��ڿ���״̬
//    {
//        if((now_time - Servo_Open_Time) >= (SERVO_DELAY_CLOSE_TIME * 1000000))
//        {
//            KeyMouseFlag.servo = 0;//���ָǹر�
//        }
//    }
//}



///**
//  * @brief	:��������״̬��ȡ
//  * @note   :--��ο���ż�ιأ�
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2017/07/09
//  * @design :
//  **/
//static void SelfSpinStateGet(u8 KeyState)
//{
//    static u8 Last_Key_State=0;
//    
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//         SelfSpin_Mark = ~SelfSpin_Mark;
//      }

//    Last_Key_State = KeyState;
//}



///**
//  * @brief	:�ߵ���״̬��ȡ
//  * @note   :--��ε��٣�ż�θ��٣�
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2017/07/17
//  * @design :
//  **/
//static void LowSpeedStateGet(u8 KeyState)
//{
//    static u8 Last_Key_State=0;
//    
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//        KeyMouseFlag.lowspeed = (KeyMouseFlag.lowspeed == 1) ? 0 : 1;
//    }
//    Last_Key_State = KeyState;
//}



///**
//  * @brief	:ѡ��ȡ��ģʽ
//  * @note   :
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2017/07/09
//  * @design :
//  **/
//static void FireShootGet(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//          Mode_Choice = ~Mode_Choice; 		
//    }
//	Last_Key_State = NewState;
//}

//static void Trail_Get(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//        Trail_status--;
//    }
//	Last_Key_State = NewState;
//}
//static void Chassic_Get(u8 NewState)
//{    
//    static u8 Last_Key_State=0;
//    if( (NewState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//          CHasssic_sym = ~CHasssic_sym; 		
//    }
//	Last_Key_State = NewState;
//}

///**
//  * @brief	:�ı䷢��״̬
//  * @note   :Ĭ��״̬Ϊ�������ٰ�һ��Ϊ����
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/
//static void FireShootDartle(u8 KeyState){
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//				Reset_Ballet=1;
//	}
//	Last_Key_State = KeyState;
//}

///**
//  * @brief	:����Ħ�����ٶ�
//  * @note   :Ĭ��״̬Ϊ��������
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/

//static void FireShootSpeed(u8 KeyState){
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//        Give_Bullet_status--;
//	}
//	Last_Key_State = KeyState;
//}



///**
//  * @brief	:����������ģʽ
//  * @note   :����������ʱ�˳����ģʽ
//  * @param	:KeyState, ����״̬
//  * @return	:void
//  *
//  * @date   :2018/04/23
//  * @design :
//  **/
//static void BigOperatorGet(u8 KeyState){
//	
//    static u8 Last_Key_State=0;
//	
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
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
//    if( (KeyState == 1) && (Last_Key_State == 0) ){//��ȡ�����½���
//		Smalloperator = ( Smalloperator == CHAR_PATTERN ) ? UNCHAR_PATTERN : CHAR_PATTERN;
//		Bigoperator = UNCHAR_PATTERN;
//	}
//	Last_Key_State = KeyState;
//		
//}

