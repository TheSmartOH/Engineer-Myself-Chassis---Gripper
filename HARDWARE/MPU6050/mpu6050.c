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



#include "mpu6050.h"
#include "Func_Usermath.h"


///*测试代码，copy至main函数内即可*/
//	
//	USART1_Init(500000, 0, 0);//串口初始化为500000
//	while(MPU_Init());//初始化Mpu6050	
//	while(mpu_dmp_init());//内部运动引擎
//	
// 	while(1)
//	{
//		niming_debug_demo(); 
//	} 	



_mpu6050     MpuDataCloud;	//mpu6050采集数据，包括三轴加速度、三轴角速度、三轴角度数据(dmp处理后)
_omegaOffset OmegaOffset;	//角速度偏移量
_eulerOffset EulerOffset;	//欧拉角偏移量

 float MPU6050_YawContinuation = 0.0f;                                 //MPU Yaw轴连续值
float MPU6050_YawAngle = 0.0f;                                 		 //MPU Yaw轴非连续值
/**
  * @brief	:初始化mpu6050
  * @note   :--
  * @param	:void
  * @return	:0,成功 其他,失败
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Init(void)
{ 
	MPU_I2C_Init();//初始化i2c总线
	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x80);//复位mpu6050
  delay_xms(100);
	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x00);//唤醒mpu6050 
	
	MPU_Set_Gyro_Fsr(3);//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);//加速度传感器,±2g
	MPU_Set_Rate(50);//设置采样率50Hz
	
	MPU_Write_Byte(MPU_INT_EN_REG, 0x00);//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0x00);//i2c主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0xFF);//开启fifo
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0x80);//int引脚低电平有效
	
	if(MPU_Read_Byte(MPU_DEVICE_ID_REG) == MPU_ADDR)//读divice地址
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x01);//设置CLKSEL以PLL-X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0x00);//加速度与陀螺仪都工作
		MPU_Set_Rate(50);//设置采样率为50Hz
		
		return 0;
 	}
	else 
	{
		return 1;
	}
}



/**
  * @brief	:设置mpu6050陀螺仪传感器满量程范围
  * @note   :--
  * @param	:fsr, 0, ±250dps
				  1, ±500dps
			      2, ±1000dps
				  3, ±2000dps
  * @return	:0,成功 其他,失败
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3); 
}



/**
  * @brief	:设置mpu6050加速度计满量程范围
  * @note   :--
  * @param	:fsr, 0, ±2g
				  1, ±4g
				  2, ±8g
				  3, ±16g
  * @return	:0,成功 其他,失败
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3);
}



/**
  * @brief	:设置mpu6050数字低通滤波器
  * @note   :--
  * @param	:lpf, 数字低通滤波频率(Hz)
  * @return	:0,成功 其他,失败
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	
	if(lpf >= 188)
		data = 1;
	else if(lpf >= 98)
		data = 2;
	else if(lpf >= 42)
		data = 3;
	else if(lpf >= 20)
		data = 4;
	else if(lpf >= 10)
		data = 5;
	else 
		data = 6; 
	
	return MPU_Write_Byte(MPU_CFG_REG, data); 
}



/**
  * @brief	:设置mpu6050采样率(假定Fs=1KHz)
  * @note   :--
  * @param	:rate, 4~1000(Hz)
  * @return	:0,成功 其他,失败
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	
	if(rate>1000)
		rate = 1000;
	if(rate<4)
		rate = 4;
	
	data = 1000/rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);//设置数字低通滤波器
	
 	return MPU_Set_LPF(rate/2);//自动设置LPF为采样率的一半
}



/**
  * @brief	:得到温度值
  * @note   :--
  * @param	:void
  * @return	:温度值(扩大了100倍)
  *
  * @date   :2016/08/27
  * @design :
  **/
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	
	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf); 
	
    raw  = ((u16)buf[0]<<8)|buf[1];  
    temp = 36.53 + ((double)raw)/340;  
	
    return temp*100;
}



/**
  * @brief	:得到陀螺仪值(原始值)
  * @note   :--
  * @param	:*gx,*gy,*gz, 陀螺仪x,y,z轴的原始读数(带符号)
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6],res;  
	
	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if(res==0)
	{
		*gx = ((u16)buf[0]<<8)|buf[1];  
		*gy = ((u16)buf[2]<<8)|buf[3];  
		*gz = ((u16)buf[4]<<8)|buf[5];
	} 	
	
    return res;
}



/**
  * @brief	:得到加速度值(原始值)
  * @note   :--
  * @param	:*ax,*ay,*az, 加速度计x,y,z轴的原始读数(带符号)
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    u8 buf[6],res;  
	
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if(res==0)
	{
		*ax = ((u16)buf[0]<<8)|buf[1];  
		*ay = ((u16)buf[2]<<8)|buf[3];  
		*az = ((u16)buf[4]<<8)|buf[5];
	} 	
	
    return res;
}



/**
  * @brief	:i2c连续写
  * @note   :--
  * @param	:addr, 器件地址 
			 reg , 寄存器地址
			 len , 写入长度
			 buf , 数据区
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i; 
	
    MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_I2C_Wait_Ack())	
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//发送寄存器地址
    MPU_I2C_Wait_Ack();	
	
	for(i=0;i<len;i++)
	{
		MPU_I2C_Send_Byte(buf[i]);//发送数据
		if(MPU_I2C_Wait_Ack())
		{
			MPU_I2C_Stop();	 
			return 1;		 
		}		
	}    
    MPU_I2C_Stop();	 
	return 0;	
} 



/**
  * @brief	:i2c连续读
  * @note   :--
  * @param	:addr, 器件地址
			 reg , 要读取的寄存器地址
			 len , 要读取的长度
			 buf , 读取到的数据存储区
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{ 
 	MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//发送寄存器地址
    MPU_I2C_Wait_Ack();
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
    MPU_I2C_Wait_Ack();
	while(len)
	{
		if(len==1)
			*buf = MPU_I2C_Read_Byte(0);//读数据,发送nACK 
		else 
			*buf = MPU_I2C_Read_Byte(1);//读数据,发送ACK  
		
		len--;
		buf++; 
	}    
    MPU_I2C_Stop();	//产生一个停止条件 
	return 0;	
}



/**
  * @brief	:i2c写一个字节
  * @note   :-
  * @param	:reg , 寄存器地址
			 data, 数据
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Write_Byte(u8 reg, u8 data) 				 
{ 
  MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//发送寄存器地址
    MPU_I2C_Wait_Ack();	
	
	MPU_I2C_Send_Byte(data);//发送数据
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();	 
		return 1;		 
	}		 
    MPU_I2C_Stop();	 
	return 0;
}



/**
  * @brief	:i2c读一个字节 
  * @note   :--
  * @param	:reg, 寄存器地址
  * @return	:u8, 接收到的数据字节
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	MPU_I2C_Wait_Ack();
	
    MPU_I2C_Send_Byte(reg);//写寄存器地址
    MPU_I2C_Wait_Ack();	
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
    MPU_I2C_Wait_Ack();
	
	res = MPU_I2C_Read_Byte(0);//读取数据,发送nACK 
    MPU_I2C_Stop();	
	
	return res;		
}



/**
  * @brief	:获取mpu6050当前角度 
  * @note   :--
  * @param	:*AX,*AY,*AZ, 当前三轴的角度 
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :梁宗南(LJI)
  **/
u8 MPU6050_GetAngle(float *AX, float *AY, float *AZ)
{	
	if(mpu_dmp_get_data(AX, AY, AZ)==0)
	{
		return 0;
	}		
	
	return 1;
}



/**
  * @brief	:获取mpu6050当前加速度 
  * @note   :--
  * @param	:*GX,*GY,*GZ, 当前三轴的加速度 
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU6050_GetAccel(float *GX, float *GY, float *GZ)
{
	short ax,ay,az;
	
	if(MPU_Get_Accelerometer(&ax, &ay, &az)==0)
	{
		*GX = (float)ax/16384.0f;
		*GY = (float)ay/16384.0f;
		*GZ = (float)az/16384.0f;
		
		return 0;
	}		
	
	return 1;
}



/**
  * @brief	:获取mpu6050当前角速度 
  * @note   :--
  * @param	:*OX,*OY,*OZ, 当前三轴的角速度 
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU6050_GetGyro(float *OX, float *OY, float *OZ)
{
	short gx,gy,gz;
	
	if(MPU_Get_Gyroscope(&gx, &gy, &gz)==0)
	{
		*OX = (float)gx/16.4f;
		*OY = (float)gy/16.4f;
		*OZ = (float)gz/16.4f;
		
		return 0;
	}		
	
	return 1;
}



/**
  * @brief	:获取mpu6050当前三轴数据 
  * @note   :--
  * @param	:*mpu, 当前三轴的数据 
  * @return	:0x00, 获取到数据
			 0x01, 未获取到角度数据
			 0x02, 未获取到加速度数据
			 0x04, 未获取到角速度数据
			 0x07, 未获取到数据
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU6050_GetData(_mpu6050 *mpu)
{
	u8 err_flag[3]={0,0,0};
	u8 err_res=0;
	
	err_flag[0] = MPU6050_GetAngle(&(mpu->Euler.Pitch), &(mpu->Euler.Roll), &(mpu->Euler.Yaw));	//获取角度数据
	err_flag[1] = MPU6050_GetAccel(&(mpu->Real.GX), &(mpu->Real.GY), &(mpu->Real.GZ));			//获取加速度数据
	err_flag[2] = MPU6050_GetGyro(&(mpu->Real.OX), &(mpu->Real.OY), &(mpu->Real.OZ));			//获取角速度数据
	
	err_res = (err_flag[2]<<2)|(err_flag[1]<<1)|(err_flag[0]<<0);
	return err_res;
}



/**
  * @brief	:更新mpu6050欧拉角及角速度 
  * @note   :--
  * @param	:void 
  * @return	:0,成功 其他,失败
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU6050_Update(void)
{
    u8 result;
    
    float Pitch, Roll, Yaw;
    float OX, OY, OZ;
    float GX, GY, GZ;
 
	result = MPU6050_GetAngle(&Pitch, &Roll, &Yaw);
    if(result)return 1;
    result = MPU6050_GetGyro(&OX, &OY, &OZ);
    if(result)return 2;
    result = MPU6050_GetAccel(&GX, &GY, &GZ);
    if(result)return 3;
    
    MpuDataCloud.Euler.Pitch = Pitch - EulerOffset.Pitch;
    MpuDataCloud.Euler.Roll  = Roll  - EulerOffset.Roll;
    MpuDataCloud.Euler.Yaw   = Yaw   - EulerOffset.Yaw;
    
    MpuDataCloud.Real.OX = -OX + MPUGYROXOFFSET;
    MpuDataCloud.Real.OY = -OY + MPUGYROYOFFSET;
    MpuDataCloud.Real.OZ = -OZ + MPUGYROZOFFSET;
    
    //角速度零值修正！    
    MpuDataCloud.Real.GX = GX;
    MpuDataCloud.Real.GY = GY;
    MpuDataCloud.Real.GZ = GZ;
     
    return 0;
}

	float pitch, roll, yaw;

void Updata_MPU6050_YawContinuation(void)
{
	float angleErr = 0.0f;

	MPU6050_GetAngle(&pitch, &roll, &yaw);
//	printf("yaw=%f\r\n",yaw);
	/* 过180处理 */
	if(ABS(yaw - MPU6050_YawAngle) >= 180.0f){
		if(yaw < 0){
			angleErr = yaw + 360.0f - MPU6050_YawAngle;
		}else{
			angleErr = yaw - 360.0f - MPU6050_YawAngle;
		}
	}else{
		angleErr = yaw - MPU6050_YawAngle;
	}
	MPU6050_YawContinuation += angleErr;
	MPU6050_YawAngle = yaw;
//		printf("mm=%f\r\n",MPU6050_YawContinuation);

}

float Get_MPU_Z_Angle(u8 Key)
{
    if(Key){ return MPU6050_YawContinuation;}
	else {return MPU6050_YawAngle;}
	
}










