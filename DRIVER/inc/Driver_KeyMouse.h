/**
  ******************************************************************************
  * 
  * 键鼠操作命令提取（兼容英雄车、工程车、步兵车的大部分操作！）
  * 注：当前只为步兵车提供控制操作！
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



#ifndef __DRIVER_KEYMOUSE_H__
#define __DRIVER_KEYMOUSE_H__



/*Driver_KeyMouse Needed Library Define*/
#include "Bsp_tim.h"
#include "Driver_Remote.h"
#include "delay.h"
#include "sys.h"




/*Driver_KeyMouse Param Class Define*/
///*键鼠操作*/
typedef struct
{
	s8  movfb;          //左右移动          1：右    0：无动作    -1：左
	s8  movlr;          //前后移动         -1：前    0：无动作     1：后
	s8  movrt;			//左右旋转		   -1：右	 0：无动作	   1：左

	s8  servo;          //弹仓盖动作标志    1：开    0：关
	s8  selfspin;       //底盘自旋状态      1：自旋  0：不自旋
	s8	lowspeed;		//高低速状态		1：低速  0：高速
	
	s8	fireshoot;		//摩擦轮开启		1：开	 0：关
	s8  shootdartle;	//连发数			1；连发	 0：单发
	s8  shootspeed;		//射速				1：低	 0：高
}_KeyMouseFlagStruct;

typedef enum
{
	CHAR_PATTERN   = 1,
	UNCHAR_PATTERN = 0,
}_Operator_Get;
extern _KeyMouseFlagStruct KeyMouseFlag;//键鼠所对应的动作标志 
extern u8 KeyValue[16];//键鼠对应按键值的状态
extern _Operator_Get Bigoperator;
extern _Operator_Get Smalloperator;



/*Driver_KeyMouse Command Define*/
//------------------------------------------------------------------------------|
//按键值：  Q       |   W       |   E       |   R       |           |           |
//对应位：  bit[6]  |   bit[0]  |   bit[7]  |   bit[8]  |           |           |
//------------------------------------------------------------------------------|
//按键值：  A       |   S       |   D       |   F       |   G       |           |
//对应位：  bit[2]  |   bit[1]  |   bit[3]  |   bit[9]  |   bit[10] |           |
//------------------------------------------------------------------------------|
//按键值：  Shift   |   Z       |   X       |   C       |   V       |   B       |        
//对应位：  bit[4]  |   bit[11] |   bit[12] |   bit[13] |   bit[14] |   bit[15] |       
//------------------------------------------------------------------------------|
//按键值：  Ctrl    |           |           |           |           |           |
//对应位：  bit[5]  |           |           |           |           |           |
//------------------------------------------------------------------------------|


#define KEY_VALUE_W              KeyValue[0]         //键鼠对应按键值的状态，1表示该按键按下 0未按下
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
void GetKeyValue(void);//获取按键值，并对KeyMouseFlag中对应成员进行赋值
void KeyStatusReset(void);//按键缓冲区初始化，清零操作
void GetCloudKey(void);
extern u8 SelfSpin_Mark,CHasssic_sym;

#endif









