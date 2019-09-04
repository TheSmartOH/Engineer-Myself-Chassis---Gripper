/**
  ******************************************************************************
  * 
  * �߾��ȹ��Ե���ģ�� JY-901 ʹ��˵����(������������оƬ��stm32f4xxϵ��)
  *	
  * ��Ʒ������
  *     ģ�鼯�ɸ߾��ȵ������ǡ����ٶȼơ��شų������������ø����ܵ�΢��������
  *     �Ƚ��Ķ���ѧ�����뿨������̬�˲��㷨���ܹ���������ʵʱ�˶���̬��
  *
  * ��Ʒ���ԣ�
  *     1.֧��usart��i2c�������ֽӿڣ�usart����Ϊ2400bps~921600bps
  *                                   i2c֧��ȫ��400k���ʣ�
  *     2.���200Hz����������ʣ��������ݿ�ͨ����λ��ѡ��������ʿɵ���
  *     3.����4·��չ�ӿڣ������ó�ģ�����롢�������롢���������pwm�����
  *     4.�߱�GPS�����������ɽ��ܷ���NMEA-0183��׼�Ĵ���GPS���ݣ�
  *
  * ���ܲ�����  
  *     1.��ѹ��3V~6V��
  *     2.������<40mA��
  *     3.����ά�ȣ�3����ٶȡ�3����ٶȡ�3ά�ų���
  *     4.��̬�����ȶ��ȣ�0.01�㣻
  *     5.����������ʣ�0.1Hz~200Hz��
  *     6.���ݽӿڣ�usart(TTL��ƽ��������֧��2400��4800��9600��19200��38400��
  *     57600��115200��230400��460800��921600)��i2c֧�ָ���i2c����400K��
  *     7����չ�ڹ��ܣ� ģ������(0~Vcc)��
  *                     �������롢
  *                     ���������
  *                     pwm���(����1us~65535us���ֱ���1us)��
  *
  *  ������ϣ�
  *     �Ա��������ǿ�
  *     ��ַ��http://Robotcontrol.taobao.com
  *
  *  �ر�ע�⣺
  *     ���ļ���������JY-901����usartͨ��ʹ�õ���������˵����
  *     �������ѱ�ɾ��������Ҫʹ��i2c���ƣ�����׼��mpu6050�Ŀ����ļ���
  *
  *  
  *	����ʱ�䣺2017��04��15��
  * �����£�2017��04��15��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __MPU6050_JY901__
#define __MPU6050_JY901__



/*MPU6050_JY901 Param Class Define*/
//*<!--ʱ���������GPS�ṩ����ȱ���ʱ����8Сʱ 
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


//*<!--���ٶȲ�������MPU�ṩ 
typedef struct
{
	short acc[3];
	short T;
}_JY901AccStruct;


//*<!--���ٶȲ�������MPU�ṩ
typedef struct
{
	short gyro[3];
	short T;
}_JY901GyroStruct;


//*<!--�ǶȲ�������JY-901ģ���ڲ����� 
typedef struct 
{
	short angle[3];
	short T;
}_JY901AngleStruct;


//*!--�شų���������JY-901ģ���ڲ�����
typedef struct
{
	short mag[3];
	short T;
}_JY901MagStruct;

extern _JY901TimeStruct		jy901Time;  //ʱ�����
extern _JY901AccStruct      jy901Acc;   //���ٶȲ���
extern _JY901GyroStruct     jy901Gyro;  //���ٶȲ���
extern _JY901AngleStruct    jy901Angle; //�ǶȲ���
extern _JY901MagStruct 		jy901Mag;   //�شų�����



/*MPU6050_JY901 Data Format Define*/
// ���ٶ� acc  ת����ʽ ��jy901Acc.acc[i]/32768*16��
// ���ٶ� gyro ת����ʽ ��jy901Gyro.gyro[i]/32768*2000��
// ��  �� ang  ת����ʽ ��jy901Angle.angle[i]/32768*180��
// �شų� mag  ת����ʽ ��jy901Mag.mag[i]��



/*MPU6050_JY901 Function Define*/
void CopeSerialData(unsigned char ucData);//�����жϵ��ú���������ÿ�յ�һ�����ݣ�����һ���������





//ע�⣺
//���³�����RM2017����ʹ�ã����������jy901���еĳ���
//����Ҫ���ģ������и���usart�����ã���
//�ر�ģ�ģ��ʹ����Ҫʹ����λ��������ز�������Ĭ�ϣ���96��n��8��1��ʽ������


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
#define GET_JY901_X_ANGLE()         (float)jy901Angle.angle[0]/32768*180    //��ȡx��Ƕȣ���Χ-180~180��float��
#define GET_JY901_Y_ANGLE()         (float)jy901Angle.angle[1]/32768*180    //��ȡy��Ƕȣ���Χ-180~180��float��
#define GET_JY901_Z_ANGLE()         (float)jy901Angle.angle[2]/32768*180    //��ȡz��Ƕȣ���Χ-180~180��float��



/*Self_JY901 Param Class Define*/
typedef struct 
{
    float   totalAngle; //IMU�����󣬽Ƕ��ۼ�
    s16     turnsCnt;   //IMU��ǰȦ�����������    
}_IMUAngleSumStruct;

extern _IMUAngleSumStruct IMUYawAngle;
extern u8 YawAngleFirstGet;//��Ƕ�ȡ���Ƕ�ֵ����1��ʾ�Ѷ�ȡ��
extern float YawStartAngle;//yaw���ʼ�Ƕ�



/*Self_JY901 Function Define*/
void JY901_Init(u32 Baud, u8 NvicPP, u8 NvicSP);//jy901 mpu6050ģ��ͨ�ų�ʼ��
void JY901_AngleDisplay(void);//jy901 mpu6050ģ��Ƕ���ʾ
float JY901_GetContinuousAngle(void);//ת����������z��Ƕ�



#endif









