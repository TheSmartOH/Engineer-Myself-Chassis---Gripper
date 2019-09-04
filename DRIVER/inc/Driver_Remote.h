#ifndef _REMOTE_H
#define _REMOTE_H

#include "stm32f4xx.h"


#define REMOTE_SWITCH_VALUE_UP       	0x01u	//��=1,0000 0001
#define REMOTE_SWITCH_VALUE_DOWN		0x02u	//��=2,0000 0010
#define REMOTE_SWITCH_VALUE_CENTRAL		0x03u	//��=3,0000 0011

#define REMOTE_SWITCH_CHANGE_1TO3       (uint8_t)((REMOTE_SWITCH_VALUE_UP<<2)|REMOTE_SWITCH_VALUE_CENTRAL)   //00 0111  
#define REMOTE_SWITCH_CHANGE_2TO3       (uint8_t)((REMOTE_SWITCH_VALUE_DOWN<<2)|REMOTE_SWITCH_VALUE_CENTRAL) //00 1011 
#define REMOTE_SWITCH_CHANGE_3TO1       (uint8_t)((REMOTE_SWITCH_VALUE_CENTRAL<<2)|REMOTE_SWITCH_VALUE_UP)   //00 1101
#define REMOTE_SWITCH_CHANGE_3TO2       (uint8_t)((REMOTE_SWITCH_VALUE_CENTRAL<<2)|REMOTE_SWITCH_VALUE_DOWN) //00 1110


#define AUTO_CONTROL				(REMOTE_SWITCH_VALUE_CENTRAL) //�Զ����
#define KEYMOUSE_CONTROL			(0XFFu)						//�������
#define REMOTE_CONTROL				(0XEFu)						//ң�ؿ���

#define SHUTDOWN					(REMOTE_SWITCH_VALUE_DOWN)	//�ػ�
#define WHEEL_TURNING   	(0x01u)
#define WHEEL_NO_TURNING 	(0x00u)

//#define    KEY_VALUE_W        key_value[0]    //ǰ��
//#define    KEY_VALUE_S        key_value[1]    //����
//#define    KEY_VALUE_A        key_value[2]    //���� 
//#define    KEY_VALUE_D        key_value[3]    //����

//#define    KEY_VALUE_SHIFT    key_value[4]    //�ߵ����л�
//#define    KEY_VALUE_CTRL     key_value[5]    //ǿ��ͣ��

//#define    KEY_VALUE_Q        key_value[6]    //������ת_�� 
//#define    KEY_VALUE_E        key_value[7]    //������ת_�� 
//#define    KEY_VALUE_R        key_value[8]    //ǿ��ֹͣ������� 

//#define    KEY_VALUE_F        key_value[9]    //
//#define    KEY_VALUE_G        key_value[10]   //

//#define    KEY_VALUE_Z        key_value[11]   //
//#define    KEY_VALUE_X        key_value[12]   //
//#define    KEY_VALUE_C        key_value[13]   // 
//#define    KEY_VALUE_V        key_value[14]   //
//#define    KEY_VALUE_B        key_value[15]   //

/*ң����ͨ��*/
typedef	struct
{
	uint16_t ch0;//!< Right Stick Left_Right
	uint16_t ch1;//!< Right Stick Up_Dowm
	uint16_t ch2;//!< Left Stick Left_Right
	uint16_t ch3;//!< Left Stick Up_Dowm
	uint8_t  s1; //!< Left Switch
	uint8_t  s2; //!< Right Switch
}_RemoteStruct;

/*���ջ�ͨ��ԭʼ����__���*/
typedef struct
{
	int16_t x;      //!< Mouse X axis
	int16_t y;      //!< Mouse Y axis
	int16_t z;      //!< Mouse Z axis	
	uint8_t press_l;//!< Mouse Left Is Press
	uint8_t press_r;//!< Mouse Right Is Press
}_MouseStruct;

/*���ջ�ͨ��ԭʼ����__����*/
typedef struct
{		
	uint16_t v;//!< Key Value
}_KeyStruct;

/*���ջ�ͨ��ԭʼ����__ȫͨ��*/

typedef struct
{
		u8           rc_ok;
    _RemoteStruct    rc;	
	_MouseStruct     mouse;	
	_KeyStruct       key;
}DBusDataType;

///*���״̬ö��*/
typedef enum
{
	NOSHOOTING = 0,
	SHOOTING   = 1,
}_E_ShootState;

//typedef enum
//{
//	SHOOTCONFIRM   = 1,
//	UNSHOOTCONFIRM = 0,
//}_E_ShootConfirm;

typedef enum
{
	NOSUPPERHEAT =  1,
	SUPPERHEAT   = 0,
}_E_ShootHeat;

///*Ħ����״̬ö��*/
typedef enum
{
	FRICTION_WHEEL_OFF = 0,
	FRICTION_WHEEL_ON  = 1,
}_E_FrictionWheelState;


extern _E_ShootState         	shootState;	//���״̬  _Ĭ�������
extern _E_FrictionWheelState	wheelState;	//Ħ����״̬_Ĭ��ֹͣ
extern _E_ShootHeat				shootHeat;	//����״̬
//extern _E_ShootConfirm			shootconfirm;//����ȷ��

extern DBusDataType dBusData;//ң�ؽ��յ�������
extern u8 Get_Ballet;
extern u8 Reset_Ballet;
extern u8 Mode_Choice,Rset_Ballet,Auto_Ballet,Auto_Ballet_symbol;

extern u8 SelfSpin_Mark,CHasssic_sym,Get_ullet,CLOUD_State,Get_Ballet_symbol;

void DBusDataDec(u8 recData[]);
_RemoteStruct* GetRemoteData(void);
_KeyStruct* GetKeyData(void);
_MouseStruct* GetMouseData(void);
u8 GetS2Mode(void);
void GetS1Mode(void);
u8 GetDbusData(void);














#endif






























