/**
  ******************************************************************************
  * 
  * �������������ȡ������Ӣ�۳������̳����������Ĵ󲿷ֲ�������
  * ע����ǰֻΪ�������ṩ���Ʋ�����
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



#ifndef __DRIVER_KEYMOUSE_H__
#define __DRIVER_KEYMOUSE_H__



/*Driver_KeyMouse Needed Library Define*/
#include "Bsp_tim.h"
#include "Driver_Remote.h"
#include "delay.h"
#include "sys.h"




/*Driver_KeyMouse Param Class Define*/
///*�������*/
typedef struct
{
	s8  movfb;          //�����ƶ�          1����    0���޶���    -1����
	s8  movlr;          //ǰ���ƶ�         -1��ǰ    0���޶���     1����
	s8  movrt;			//������ת		   -1����	 0���޶���	   1����

	s8  servo;          //���ָǶ�����־    1����    0����
	s8  selfspin;       //��������״̬      1������  0��������
	s8	lowspeed;		//�ߵ���״̬		1������  0������
	
	s8	fireshoot;		//Ħ���ֿ���		1����	 0����
	s8  shootdartle;	//������			1������	 0������
	s8  shootspeed;		//����				1����	 0����
}_KeyMouseFlagStruct;

typedef enum
{
	CHAR_PATTERN   = 1,
	UNCHAR_PATTERN = 0,
}_Operator_Get;
extern _KeyMouseFlagStruct KeyMouseFlag;//��������Ӧ�Ķ�����־ 
extern u8 KeyValue[16];//�����Ӧ����ֵ��״̬
extern _Operator_Get Bigoperator;
extern _Operator_Get Smalloperator;



/*Driver_KeyMouse Command Define*/
//------------------------------------------------------------------------------|
//����ֵ��  Q       |   W       |   E       |   R       |           |           |
//��Ӧλ��  bit[6]  |   bit[0]  |   bit[7]  |   bit[8]  |           |           |
//------------------------------------------------------------------------------|
//����ֵ��  A       |   S       |   D       |   F       |   G       |           |
//��Ӧλ��  bit[2]  |   bit[1]  |   bit[3]  |   bit[9]  |   bit[10] |           |
//------------------------------------------------------------------------------|
//����ֵ��  Shift   |   Z       |   X       |   C       |   V       |   B       |        
//��Ӧλ��  bit[4]  |   bit[11] |   bit[12] |   bit[13] |   bit[14] |   bit[15] |       
//------------------------------------------------------------------------------|
//����ֵ��  Ctrl    |           |           |           |           |           |
//��Ӧλ��  bit[5]  |           |           |           |           |           |
//------------------------------------------------------------------------------|


#define KEY_VALUE_W              KeyValue[0]         //�����Ӧ����ֵ��״̬��1��ʾ�ð������� 0δ����
#define KEY_VALUE_S              KeyValue[1]
#define KEY_VALUE_A              KeyValue[2]
#define KEY_VALUE_D              KeyValue[3]

#define KEY_VALUE_SHIFT          KeyValue[4]
#define KEY_VALUE_CTRL           KeyValue[5]

#define KEY_VALUE_Q              KeyValue[6]
#define KEY_VALUE_E              KeyValue[7]
#define KEY_VALUE_R              KeyValue[8] 

#define KEY_VALUE_F              KeyValue[9]
#define KEY_VALUE_G              KeyValue[10]

#define KEY_VALUE_Z              KeyValue[11]
#define KEY_VALUE_X              KeyValue[12]
#define KEY_VALUE_C              KeyValue[13]
#define KEY_VALUE_V              KeyValue[14]
#define KEY_VALUE_B              KeyValue[15]



/*Driver_KeyMouse Function Define*/
void GetKeyValue(void);//��ȡ����ֵ������KeyMouseFlag�ж�Ӧ��Ա���и�ֵ
void KeyStatusReset(void);//������������ʼ�����������
void GetCloudKey(void);
extern u8 SelfSpin_Mark,CHasssic_sym;

#endif









