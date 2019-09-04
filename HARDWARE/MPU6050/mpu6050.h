/**
  ******************************************************************************
  * 
  * ���������ԭ�����̼����ӷ������������̣���ԭ�������в�����Ӧ�Ը���
  * Mpu6050 ���ᴫ������������������������stm32f4xxϵ�е�Ƭ��
  * �����޸ģ����޸�MPU6050�ļ����¶�Ӧ�ļ�
  * 
  * 
  *	����ʱ�䣺2016��08��27��
  * �����£�2016��10��05��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __MPU6050_H__
#define __MPU6050_H__



/*Mpu6050 Needed Library Define*/
#include "Bsp_usart4.h"//�����ļ�ʱʹ�ã�����ʹ��ʱ��ɾ����������ܻᱨ��

#include "mpui2c.h"   												  	  
#include "delay.h"

#include "inv_mpu.h"//ʹ���ڲ��˶�����
#include "inv_mpu_dmp_motion_driver.h" 



/*Mpu6050 Main Register Define*/
#define MPU_SAMPLE_RATE_REG		0X19	//����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG				0X1A	//���üĴ���

#define MPU_GYRO_CFG_REG		0X1B	//���������üĴ���
											//250	:	LSB=131
											//500	:	LSB=65.5
											//1000	:	LSB=32.8
											//2000	:	LSB=16.4
											
#define MPU_ACCEL_CFG_REG		0X1C	//���ٶȼ����üĴ���
											//2g	:	LSB=16384
											//4g	:	LSB=8192
											//8g	:	LSB=4096
											//16g	:	LSB=2048
											
#define MPU_MOTION_DET_REG		0X1F	//�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG			0X23	//FIFOʹ�ܼĴ���

#define MPU_INTBP_CFG_REG		0X37	//�ж�/��·���üĴ���
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���

#define MPU_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_TEMP_OUTH_REG		0X41	//�¶�ֵ��8λ�Ĵ���
#define MPU_TEMP_OUTL_REG		0X42	//�¶�ֵ��8λ�Ĵ���
#define MPU_GYRO_XOUTH_REG		0X43	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG		0X44	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG		0X45	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG		0X46	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG		0X47	//������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG		0X48	//������ֵ,Z���8λ�Ĵ���

#define MPU_USER_CTRL_REG		0X6A	//�û����ƼĴ���
#define MPU_PWR_MGMT1_REG		0X6B	//��Դ����Ĵ���1
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2 
#define MPU_DEVICE_ID_REG		0X75	//����ID�Ĵ���
 
 
 
/*Mpu6050 Address Define*/
//���AD0��(9��)��GND,I2C��ַΪ0X68(���������λ).
//���AD0��(9��)��VCC,I2C��ַΪ0X69(���������λ).
#define MPU_ADDR				0X68


//<!-- mpu6050��������������
#define MPUGYROXOFFSET		    0.0f //������x�᷽��ƫ����
#define MPUGYROYOFFSET          0.0f //������y�᷽��ƫ����
#define MPUGYROZOFFSET          0.0f //������z�᷽��ƫ����



/*Mpu6050 Param Class Define*/
///*mpu6050ʵ������*/
typedef struct
{  
    struct
    {
        float OX; //���ٶ�ʵ������
        float OY;
        float OZ;
		
        float GX; //���ٶ�ʵ������
        float GY;
        float GZ;
		
        float T;  //�¶�ʵ������
    }Real;
      
    struct
    {
        float Pitch;
        float Roll;
        float Yaw;
    }Euler;//����ʵ�ʶԵ�ŷ����
}_mpu6050;

///*mpu6050���ٶ�ƫ����*/
typedef struct
{
    float X;
    float Y;
    float Z;
}_omegaOffset;

///*mpu6050ŷ����ƫ����*/
typedef struct
{
    float Pitch;
    float Roll;
    float Yaw;
}_eulerOffset;



/*Mpu6050 Param Class Define*/
//��̬����
extern _mpu6050 MpuDataCloud;//mpu6050�ɼ����ݣ�����������ٶȡ�������ٶȡ�����Ƕ�����(dmp�����)
//ƫ����������
extern _omegaOffset OmegaOffset;//���ٶ�ƫ����
extern _eulerOffset EulerOffset;//ŷ����ƫ����



/*Mpu6050 Function Define*/
u8 MPU_Init(void);//��ʼ��mpu6050

u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf);//i2c����д����
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf); //i2c����������
u8 MPU_Write_Byte(u8 reg, u8 data);//i2cдһ���ֽ�
u8 MPU_Read_Byte(u8 reg);//i2c��һ���ֽ�

u8 MPU_Set_Gyro_Fsr(u8 fsr); //���������������̷�Χ 
u8 MPU_Set_Accel_Fsr(u8 fsr);//���ü��ٶȼ������̷�Χ  
u8 MPU_Set_LPF(u16 lpf);	 //�������ֵ�ͨ�˲��� 
u8 MPU_Set_Rate(u16 rate);	 //����mpu6050������

u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz);	//��ȡ�����ǵ�ԭʼ����
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az);//��ȡ���ٶȼ�ԭʼ����
short MPU_Get_Temperature(void);						//��ȡ�ڲ��¶ȴ�����ֵ

u8 MPU6050_GetAngle(float *AX, float *AY, float *AZ);//��ȡmpu6050��ǰ�Ƕ�
u8 MPU6050_GetAccel(float *GX, float *GY, float *GZ);//��ȡmpu6050��ǰ���ٶ�
u8 MPU6050_GetGyro(float *OX, float *OY, float *OZ); //��ȡmpu6050��ǰ���ٶ�
u8 MPU6050_GetData(_mpu6050 *mpu);//��ȡmpu6050��ǰ��������

u8 MPU6050_Update(void);//����mpu6050ŷ���Ǽ����ٶ�
void Updata_MPU6050_YawContinuation(void);
float Get_MPU_Z_Angle(u8 Key);
extern float MPU6050_YawContinuation;

#endif


