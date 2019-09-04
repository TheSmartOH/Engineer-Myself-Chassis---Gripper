/**
  ******************************************************************************
  * 
  * 借鉴自正点原子例程及电子发烧友网友例程，对原版代码进行部分适应性更改
  * Mpu6050 六轴传感器的驱动，本部分适用于stm32f4xx系列单片机
  * 如需修改，请修改MPU6050文件夹下对应文件
  * 
  * 
  *	创建时间：2016年08月27日
  * 最后更新：2016年10月05日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#ifndef __MPU6050_H__
#define __MPU6050_H__



/*Mpu6050 Needed Library Define*/
#include "Bsp_usart4.h"//测试文件时使用，正常使用时可删掉，否则可能会报错！

#include "mpui2c.h"   												  	  
#include "delay.h"

#include "inv_mpu.h"//使用内部运动引擎
#include "inv_mpu_dmp_motion_driver.h" 



/*Mpu6050 Main Register Define*/
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器

#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
											//250	:	LSB=131
											//500	:	LSB=65.5
											//1000	:	LSB=32.8
											//2000	:	LSB=16.4
											
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
											//2g	:	LSB=16384
											//4g	:	LSB=8192
											//8g	:	LSB=4096
											//16g	:	LSB=2048
											
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器

#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器
#define MPU_TEMP_OUTH_REG		0X41	//温度值高8位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器
#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
 
 
/*Mpu6050 Address Define*/
//如果AD0脚(9脚)接GND,I2C地址为0X68(不包含最低位).
//如果AD0脚(9脚)接VCC,I2C地址为0X69(不包含最低位).
#define MPU_ADDR				0X68


//<!-- mpu6050陀螺仪修正量：
#define MPUGYROXOFFSET		    0.0f //陀螺仪x轴方向偏移量
#define MPUGYROYOFFSET          0.0f //陀螺仪y轴方向偏移量
#define MPUGYROZOFFSET          0.0f //陀螺仪z轴方向偏移量



/*Mpu6050 Param Class Define*/
///*mpu6050实际数据*/
typedef struct
{  
    struct
    {
        float OX; //角速度实际数据
        float OY;
        float OZ;
		
        float GX; //加速度实际数据
        float GY;
        float GZ;
		
        float T;  //温度实际数据
    }Real;
      
    struct
    {
        float Pitch;
        float Roll;
        float Yaw;
    }Euler;//结算实际对的欧拉角
}_mpu6050;

///*mpu6050角速度偏移量*/
typedef struct
{
    float X;
    float Y;
    float Z;
}_omegaOffset;

///*mpu6050欧拉角偏移量*/
typedef struct
{
    float Pitch;
    float Roll;
    float Yaw;
}_eulerOffset;



/*Mpu6050 Param Class Define*/
//姿态数据
extern _mpu6050 MpuDataCloud;//mpu6050采集数据，包括三轴加速度、三轴角速度、三轴角度数据(dmp处理后)
//偏差数据数据
extern _omegaOffset OmegaOffset;//角速度偏移量
extern _eulerOffset EulerOffset;//欧拉角偏移量



/*Mpu6050 Function Define*/
u8 MPU_Init(void);//初始化mpu6050

u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf);//i2c连续写数据
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf); //i2c连续读数据
u8 MPU_Write_Byte(u8 reg, u8 data);//i2c写一个字节
u8 MPU_Read_Byte(u8 reg);//i2c读一个字节

u8 MPU_Set_Gyro_Fsr(u8 fsr); //设置陀螺仪满量程范围 
u8 MPU_Set_Accel_Fsr(u8 fsr);//设置加速度计满量程范围  
u8 MPU_Set_LPF(u16 lpf);	 //设置数字低通滤波器 
u8 MPU_Set_Rate(u16 rate);	 //设置mpu6050采样率

u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz);	//读取陀螺仪的原始数据
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az);//读取加速度计原始数据
short MPU_Get_Temperature(void);						//读取内部温度传感器值

u8 MPU6050_GetAngle(float *AX, float *AY, float *AZ);//获取mpu6050当前角度
u8 MPU6050_GetAccel(float *GX, float *GY, float *GZ);//获取mpu6050当前加速度
u8 MPU6050_GetGyro(float *OX, float *OY, float *OZ); //获取mpu6050当前角速度
u8 MPU6050_GetData(_mpu6050 *mpu);//获取mpu6050当前三轴数据

u8 MPU6050_Update(void);//更新mpu6050欧拉角及角速度
void Updata_MPU6050_YawContinuation(void);
float Get_MPU_Z_Angle(u8 Key);
extern float MPU6050_YawContinuation;

#endif


