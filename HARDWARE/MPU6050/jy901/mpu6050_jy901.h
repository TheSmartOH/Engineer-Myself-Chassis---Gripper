/**
  ******************************************************************************
  * 
  * 高精度惯性导航模块 JY-901 使用说明书(本驱动库适用芯片：stm32f4xx系列)
  *	
  * 产品概述：
  *     模块集成高精度的陀螺仪、加速度计、地磁场传感器，采用高性能的微处理器和
  *     先进的动力学解算与卡尔曼动态滤波算法，能够快速求解出实时运动姿态；
  *
  * 产品特性：
  *     1.支持usart和i2c两种数字接口：usart速率为2400bps~921600bps
  *                                   i2c支持全速400k速率；
  *     2.最高200Hz数据输出速率，输入内容可通过上位机选择，输出速率可调；
  *     3.保留4路扩展接口，可配置成模拟输入、数字输入、数字输出、pwm输出；
  *     4.具备GPS连接能力，可接受符合NMEA-0183标准的串口GPS数据；
  *
  * 性能参数：  
  *     1.电压：3V~6V；
  *     2.电流：<40mA；
  *     3.测量维度：3轴加速度、3轴角速度、3维磁场；
  *     4.姿态测量稳定度：0.01°；
  *     5.数据输出速率：0.1Hz~200Hz；
  *     6.数据接口：usart(TTL电平，波特率支持2400、4800、9600、19200、38400、
  *     57600、115200、230400、460800、921600)，i2c支持高速i2c速率400K；
  *     7、扩展口功能： 模拟输入(0~Vcc)、
  *                     数字输入、
  *                     数字输出、
  *                     pwm输出(周期1us~65535us，分辨率1us)；
  *
  *  相关资料：
  *     淘宝：君悦智控
  *     网址：http://Robotcontrol.taobao.com
  *
  *  特别注意：
  *     本文件仅保留与JY-901进行usart通信使用的数据类型说明；
  *     其他的已被删减，若需要使用i2c控制，自行准备mpu6050的控制文件！
  *
  *  
  *	创建时间：2017年04月15日
  * 最后更新：2017年04月15日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#ifndef __MPU6050_JY901__
#define __MPU6050_JY901__



/*MPU6050_JY901 Param Class Define*/
//*<!--时间参数，由GPS提供，会比北京时间慢8小时 
typedef struct
{
	unsigned char   ucYear;
	unsigned char   ucMonth;
	unsigned char   ucDay;
	unsigned char   ucHour;
	unsigned char   ucMinute;
	unsigned char   ucSecond;
	unsigned short  usMiliSecond;
}_JY901TimeStruct;


//*<!--加速度参数，由MPU提供 
typedef struct
{
	short acc[3];
	short T;
}_JY901AccStruct;


//*<!--角速度参数，由MPU提供
typedef struct
{
	short gyro[3];
	short T;
}_JY901GyroStruct;


//*<!--角度参数，由JY-901模块内部解析 
typedef struct 
{
	short angle[3];
	short T;
}_JY901AngleStruct;


//*!--地磁场参数，由JY-901模块内部解析
typedef struct
{
	short mag[3];
	short T;
}_JY901MagStruct;

extern _JY901TimeStruct		jy901Time;  //时间参数
extern _JY901AccStruct      jy901Acc;   //加速度参数
extern _JY901GyroStruct     jy901Gyro;  //角速度参数
extern _JY901AngleStruct    jy901Angle; //角度参数
extern _JY901MagStruct 		jy901Mag;   //地磁场参数



/*MPU6050_JY901 Data Format Define*/
// 加速度 acc  转换公式 ：jy901Acc.acc[i]/32768*16；
// 角速度 gyro 转换公式 ：jy901Gyro.gyro[i]/32768*2000；
// 角  度 ang  转换公式 ：jy901Angle.angle[i]/32768*180；
// 地磁场 mag  转换公式 ：jy901Mag.mag[i]；



/*MPU6050_JY901 Function Define*/
void CopeSerialData(unsigned char ucData);//串口中断调用函数，串口每收到一个数据，调用一次这个函数





//注意：
//以下程序是RM2017比赛使用，并非整理的jy901库中的程序！
//如需要更改，请自行更改usart的配置！！
//特别的，模块使用需要使用上位机设置相关参数，如默认，则96、n、8、1格式！！！


/*Self_JY901 Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Task_oled.h"



/*Self_JY901 Command Define*/
#define GET_JY901_X_ANGLE()         (float)jy901Angle.angle[0]/32768*180    //获取x轴角度，范围-180~180，float型
#define GET_JY901_Y_ANGLE()         (float)jy901Angle.angle[1]/32768*180    //获取y轴角度，范围-180~180，float型
#define GET_JY901_Z_ANGLE()         (float)jy901Angle.angle[2]/32768*180    //获取z轴角度，范围-180~180，float型



/*Self_JY901 Param Class Define*/
typedef struct 
{
    float   totalAngle; //IMU开机后，角度累计
    s16     turnsCnt;   //IMU当前圈数，开机清空    
}_IMUAngleSumStruct;

extern _IMUAngleSumStruct IMUYawAngle;
extern u8 YawAngleFirstGet;//标记读取到角度值，置1表示已读取到
extern float YawStartAngle;//yaw轴初始角度



/*Self_JY901 Function Define*/
void JY901_Init(u32 Baud, u8 NvicPP, u8 NvicSP);//jy901 mpu6050模块通信初始化
void JY901_AngleDisplay(void);//jy901 mpu6050模块角度显示
float JY901_GetContinuousAngle(void);//转化成连续的z轴角度



#endif









