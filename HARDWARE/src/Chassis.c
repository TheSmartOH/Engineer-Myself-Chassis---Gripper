#include "system.h"
#include "Chassis.h"
PidTypeDef ChassisPID[6];

PidTypeDef GripperPositionPID[2];
PidTypeDef GripperSpeedPID[2];
s8 count_turn[2] = {0, 0};
static s16 Speed_Target[6]={0, 0, 0, 0,0,0};                              //�������ݵ����İ棬stm32f407v ����ֻ�������ݼ��㣬������ջ����㣡����

_RampStruct LFRamp = {0, 0, 0, 0};//������������б��
_RampStruct LBRamp = {0, 0, 0, 0};//������������б��
_RampStruct RFRamp = {0, 0, 0, 0};//������������б��
_RampStruct RBRamp = {0, 0, 0, 0};//������������б��
_RampStruct LCRamp = {0, 0, 0, 0};
_RampStruct RCRamp = {0, 0, 0, 0};

/*
 ******************************************************************************************************************************
 *	��    �֣�SetTyreSpeedRef(), SetTyreSpeedFdb(), GetTyreSpeedOutput()
 *	��    ����
 *	����ֵ  ��
 *	�����������ò��ֺ���Ϊ�ٶȻ�pid��������
 *	����ʱ�䣺2017.03.11
 *	��    ע��ÿ���޸�ʱ���ע�޸����ɣ�Ҫд����ʲô��Ϊʲô����Ҫд��ô������ʱ�䣬����
 *			����tyreNumber������ѡ����ĸ����ӽ���pid���㣬��ѡ�����У�
 *					TYRE_LF		0
 *					TYRE_LB 	1
 *					TYRE_RF		2
 *					TYRE_RB		3
 *
 ******************************************************************************************************************************
*/
void SetTyreSpeedRef(int tyreNumber,int ref)
{
	if(tyreNumber >= 0 && tyreNumber < 6)
		ChassisPID[tyreNumber].ref = ref;
}



void SetTyrePosionFdb(int tyreNumber,int fdb)
{
	if(tyreNumber >= 0 && tyreNumber < 6)
		ChassisPID[tyreNumber].fdb = fdb;
}

void SetTyreSpeedFdb(int tyreNumber,int fdb)
{
	if(tyreNumber >= 0 && tyreNumber < 6)
		ChassisPID[tyreNumber].fdb = fdb;
}

// && (GripperPositionPID[tyreNumber].fdb != fdb))
void SetGripperPositionFdb(int tyreNumber,int fdb)
{
	if(tyreNumber >= 0 && tyreNumber < 6)
		GripperPositionPID[tyreNumber].fdb = fdb;
}

void SetGripperSpeedFdb(int tyreNumber,int fdb)
{
	if(tyreNumber >= 0 && tyreNumber < 6)
		GripperSpeedPID[tyreNumber].fdb = fdb;
}


u16 GlobalFlag = 0x0000;//ȫ�ּ�������־(����Ϊ1)�������ⲿ�豸
u16 CanMonitor = 0x0000;//�����������can��·����

#define KeyMouse      1
#define RemoteControl 3

fp32 ShanWai[4];
#define KEY_CHASSIS_SPEED       	(750*10)    //��������µ����ƶ��ٶ�                        //ԭ����750*10��
#define KEY_CHASSIS_LOW_MINUS   	(4*750)		//�ߵ���ģʽ�ٶȲ�
#define MOUSE_REMOTE           		(60)        //����Ӧ������ת������


void ChassisControl(void)
{
	s16 rotate_data = 0;
	static int A = 0;
	const static fp32 PID[3] = {9.0f,0.8f,2.0f};
	_RemoteStruct * rcdata = GetRemoteData();                             //�õ�ң�ص�����
	_MouseStruct *mouse = GetMouseData();//��ȡ����ƶ�ֵ
	rotate_data = (float)(mouse->x)*150+KeyMouseFlag.movrt*2000;
	GetKeyValue();
	if(A == 0)
	{
		for(int i = 0; i < 6; i++)
			PID_Init(&ChassisPID[i], PID, 8000, 12000);
		A++;
	}
		if(rcdata->s2 == RemoteControl)
	{
		LFRamp.Target = ( (rcdata->ch2-1024)*0.55 + (rcdata->ch3-1024)*0.55) * 10 + (rcdata->ch0-1024)*5.5;
		LBRamp.Target = (-(rcdata->ch2-1024)*0.55 + (rcdata->ch3-1024)*0.55) * 10 + (rcdata->ch0-1024)*5.5;
		RFRamp.Target = ( (rcdata->ch2-1024)*0.55 - (rcdata->ch3-1024)*0.55) * 10 + (rcdata->ch0-1024)*5.5;
		RBRamp.Target = (-(rcdata->ch2-1024)*0.55 - (rcdata->ch3-1024)*0.55) * 10 + (rcdata->ch0-1024)*5.5;
		LCRamp.Target = (rcdata->ch3-1024)*10;
		RCRamp.Target = -(rcdata->ch3-1024)*10;
	}
	else if(rcdata->s2 == KeyMouse)
	{
    LFRamp.Target = (-KeyMouseFlag.movfb - KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS) + rotate_data;
		LBRamp.Target = ( KeyMouseFlag.movfb - KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS) + rotate_data;
		RFRamp.Target = (-KeyMouseFlag.movfb + KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS) + rotate_data; 
		RBRamp.Target = ( KeyMouseFlag.movfb + KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS) + rotate_data;
		LCRamp.Target = -( KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS)*0.636;
		RCRamp.Target = ( KeyMouseFlag.movlr) * (KEY_CHASSIS_SPEED - KEY_CHASSIS_LOW_MINUS)*0.636;
//		printf("%d %d %d %d %d %d\t\r\n",RFRamp.Target,LBRamp.Target,RFRamp.Target,RBRamp.Target,LCRamp.Target,RCRamp.Target);
	}
	ShanWai[0] = ChassisPID[0].fdb;
	ShanWai[1] = ChassisPID[1].fdb;
	ShanWai[2] = ChassisPID[2].fdb;
	ShanWai[3] = ChassisPID[3].fdb;
	shanwai_displaywave2(ShanWai,4);
	
	Speed_Target[0] = (s16)(LFRamp.Target);
	Speed_Target[1] = (s16)(LBRamp.Target);
	Speed_Target[2] = (s16)(RFRamp.Target);
	Speed_Target[3] = (s16)(RBRamp.Target);
	Speed_Target[4] = (s16)(LCRamp.Target);
	Speed_Target[5] = (s16)(RCRamp.Target);
	
	for(int i = 0;i < 6 ;i++)
	{
		SetTyreSpeedRef(i,Speed_Target[i]);
		Pid_Realize(&ChassisPID[i]);
	}
	if(rcdata->s2 != 2)
	{
//		CAN1_SendChassisMail(0, ChassisPID[1].out, 0, 0);
		CAN1_SendChassisMail(ChassisPID[0].out, ChassisPID[1].out, ChassisPID[2].out, ChassisPID[3].out);
		Cloud_SendControlMail(ChassisPID[4].out, ChassisPID[5].out);
	}
	else
		CAN1_SendChassisMail(0, 0, 0, 0);
//	CAN1_SendChassisMail(0,ChassisPID[1].out,0,0);


}


CanRxMsg GripperRxMessage;
void Gripper_Driver(void)
{
	_RemoteStruct * rcdata = GetRemoteData();                             //�õ�ң�ص�����
/*****************                ��������                *****************/	
	if(rcdata->s2 == 3 && rcdata->s1 == 1)
		GripperPositionPID[0].ref = 8192 * 10.5;
	else if(rcdata->s2 == 3 && rcdata->s1 == 3)
		GripperPositionPID[0].ref = 8192 * 6;
	else if(rcdata->s2 == 3 && rcdata->s1 == 2)
		GripperPositionPID[0].ref = 4119;
//PID�������
	const fp32 Gripper_Position[3] = {2.4, 0.0, 0.0};
	const fp32 Gripper_Speed[3] = {3.2, 0.5, 0.0};
/*****************                PID��ʼ��                *****************/
//Header:PID��ʼ�����ÿ��ѭ����ִ�У�������������Bug������
	
	PID_Init(&GripperPositionPID[0],Gripper_Position,24000,15000);
	PID_Init(&GripperSpeedPID[0],Gripper_Speed,24000,15000);
	GripperPositionPID[0].lastfdb = GripperPositionPID[0].fdb;
	if(GripperRxMessage.StdId == Shifting_Bullet_Front)
	{
		SetGripperPositionFdb(0,(s16)(GripperRxMessage.Data[0]<<8 | GripperRxMessage.Data[1]));

		Get_Countturn(0);

		Pid_PosiSpeeD(&GripperPositionPID[0],&GripperSpeedPID[0],count_turn[0]);
//		printf("%.0f \r\n",GripperPositionPID[0].out);
//  	printf("%d %.0f %.0f\t\r\n",count_turn[0],GripperPositionPID[0].fdb,GripperPositionPID[0].lastfdb);
		ShanWai[0] = GripperPositionPID[0].fdb + count_turn[0] * 8192;
		ShanWai[1] = GripperPositionPID[0].ref;
		ShanWai[2] = 0;
//		ShanWai[0] = GripperPositionPID[0].fdb + count_turn[0] * 8192;
//		ShanWai[1] = GripperPositionPID[0].ref;
//		ShanWai[2] = count_turn[0];
		shanwai_displaywave2(ShanWai,2);
		if(rcdata->s2 == 3 && rcdata->s1 != 2)
			CAN2_SendChassisMail(0x200, 0, 0, 0, 0);

		else
		{
			CAN2_SendChassisMail(0x200, 0, 0, 0, 0);
			count_turn[0] = 0;
		}
	}

/**
  * @brief	:
  * @note   :
  * @date   :2019//
  * @design :XYX
  **/
//		CAN1_SendChassisMail(0, GripperPositionPID[0].out, 0, 0);

}


void Get_Countturn(int n)
{
	 if( (GripperPositionPID[n].error[0]>8000) || (GripperPositionPID[n].error[0]<-8000) )
	{
		if( (GripperPositionPID[n].error[0]>0) && ((GripperPositionPID[n].fdb - GripperPositionPID[n].lastfdb )<-2000) )
      count_turn[n]++;
		else if( (GripperPositionPID[n].error[0]<0) && ((GripperPositionPID[n].fdb - GripperPositionPID[n].lastfdb )>2000) )
			count_turn[n]--;
	}
	else
	{
		if( (GripperPositionPID[n].fdb - GripperPositionPID[n].lastfdb )<-4096 )
			count_turn[n]++;
    else if( (GripperPositionPID[n].fdb - GripperPositionPID[n].lastfdb )>4096 )
			count_turn[n]--;
	}
}














