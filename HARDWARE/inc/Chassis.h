#ifndef __Chassis_H
#define __Chassis_H

#include "system.h"

#define TYRE_LF		0
#define TYRE_LB 	1
#define TYRE_RF		2
#define TYRE_RB		3
#define TYRE_LC		4
#define TYRE_RC		5

/*���ID*/
#define CHASSIS_LF_MOTOR_CAN_ID         0x201
#define CHASSIS_LB_MOTOR_CAN_ID         0x202
#define CHASSIS_RF_MOTOR_CAN_ID         0x203
#define CHASSIS_RB_MOTOR_CAN_ID         0x204

#define WHEEL_LEFT_ID			0x205
#define WHEEL_RIGHT_ID		    0x206

#define Pitch_Motor              0x205   //ע��ÿ��can��·�������ص��������
#define YAW_Motor                0x206                                       //Poker��id����̶�Ϊ0x201��-- 2017.04.04
#define Getting_Bullet_Motor       0x201                                       //Poker��id����̶�Ϊ0x201��-- 2017.04.04
#define Shifting_Bullet_Front      0x202
#define Shifting_Bullet_Right      0x203

extern u16 GlobalFlag;		                    //ȫ�ּ�������־(��1����)�������ⲿ�豸
                                                //bit[0]:can1ͨѶ������־λ
                                                //bit[1]:can2ͨѶ������־λ
                                                //bit[2]:�Ӿ�������ɱ�־λ
                                                //bit[3]:���ջ��������ձ��λ

extern u16 CanMonitor;                          //���������(��1����)��can��·����
                                                //bit[0]:0x201��3510 - LF
                                                //bit[1]:0x202��3510 - LB
                                                //bit[2]:0x203��3510 - RF
                                                //bit[3]:0x204��3510 - RB
                                                //bit[4]:0x205��6623 - Yaw
                                                //bit[5]:0x206��6623 - Pitch
                                                //bit[6]:0x20x��2006 - Poker
                                                //bit[7]:0x401���ٷ�imu����������																						
	
extern _RemoteStruct * R;	
typedef struct
{
    s16 Target;     //����ֵ
    s16 Current;    //��ǰֵ
    s16 Output;     //б�����
    s16 Dirt;       //����������б��ϵ��
}_RampStruct;
void ChassisControl(void);
void SetTyreSpeedRef(int tyreNumber,int ref);
void SetTyreSpeedFdb(int tyreNumber,int fdb);
void Gripper_Driver(void);
void Get_Countturn(int n);
void SetGripperPositionFdb(int tyreNumber,int fdb);
void SetGripperSpeedFdb(int tyreNumber,int fdb);










#endif



