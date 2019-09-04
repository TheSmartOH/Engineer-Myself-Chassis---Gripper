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



#include "mpu6050.h"
#include "Func_Usermath.h"


///*���Դ��룬copy��main�����ڼ���*/
//	
//	USART1_Init(500000, 0, 0);//���ڳ�ʼ��Ϊ500000
//	while(MPU_Init());//��ʼ��Mpu6050	
//	while(mpu_dmp_init());//�ڲ��˶�����
//	
// 	while(1)
//	{
//		niming_debug_demo(); 
//	} 	



_mpu6050     MpuDataCloud;	//mpu6050�ɼ����ݣ�����������ٶȡ�������ٶȡ�����Ƕ�����(dmp�����)
_omegaOffset OmegaOffset;	//���ٶ�ƫ����
_eulerOffset EulerOffset;	//ŷ����ƫ����

 float MPU6050_YawContinuation = 0.0f;                                 //MPU Yaw������ֵ
float MPU6050_YawAngle = 0.0f;                                 		 //MPU Yaw�������ֵ
/**
  * @brief	:��ʼ��mpu6050
  * @note   :--
  * @param	:void
  * @return	:0,�ɹ� ����,ʧ��
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Init(void)
{ 
	MPU_I2C_Init();//��ʼ��i2c����
	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x80);//��λmpu6050
  delay_xms(100);
	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x00);//����mpu6050 
	
	MPU_Set_Gyro_Fsr(3);//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);//���ٶȴ�����,��2g
	MPU_Set_Rate(50);//���ò�����50Hz
	
	MPU_Write_Byte(MPU_INT_EN_REG, 0x00);//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0x00);//i2c��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0xFF);//����fifo
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0x80);//int���ŵ͵�ƽ��Ч
	
	if(MPU_Read_Byte(MPU_DEVICE_ID_REG) == MPU_ADDR)//��divice��ַ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0x01);//����CLKSEL��PLL-X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0x00);//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);//���ò�����Ϊ50Hz
		
		return 0;
 	}
	else 
	{
		return 1;
	}
}



/**
  * @brief	:����mpu6050�����Ǵ����������̷�Χ
  * @note   :--
  * @param	:fsr, 0, ��250dps
				  1, ��500dps
			      2, ��1000dps
				  3, ��2000dps
  * @return	:0,�ɹ� ����,ʧ��
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr<<3); 
}



/**
  * @brief	:����mpu6050���ٶȼ������̷�Χ
  * @note   :--
  * @param	:fsr, 0, ��2g
				  1, ��4g
				  2, ��8g
				  3, ��16g
  * @return	:0,�ɹ� ����,ʧ��
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr<<3);
}



/**
  * @brief	:����mpu6050���ֵ�ͨ�˲���
  * @note   :--
  * @param	:lpf, ���ֵ�ͨ�˲�Ƶ��(Hz)
  * @return	:0,�ɹ� ����,ʧ��
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
  * @brief	:����mpu6050������(�ٶ�Fs=1KHz)
  * @note   :--
  * @param	:rate, 4~1000(Hz)
  * @return	:0,�ɹ� ����,ʧ��
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
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);//�������ֵ�ͨ�˲���
	
 	return MPU_Set_LPF(rate/2);//�Զ�����LPFΪ�����ʵ�һ��
}



/**
  * @brief	:�õ��¶�ֵ
  * @note   :--
  * @param	:void
  * @return	:�¶�ֵ(������100��)
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
  * @brief	:�õ�������ֵ(ԭʼֵ)
  * @note   :--
  * @param	:*gx,*gy,*gz, ������x,y,z���ԭʼ����(������)
  * @return	:0,�ɹ� ����,ʧ��
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
  * @brief	:�õ����ٶ�ֵ(ԭʼֵ)
  * @note   :--
  * @param	:*ax,*ay,*az, ���ٶȼ�x,y,z���ԭʼ����(������)
  * @return	:0,�ɹ� ����,ʧ��
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
  * @brief	:i2c����д
  * @note   :--
  * @param	:addr, ������ַ 
			 reg , �Ĵ�����ַ
			 len , д�볤��
			 buf , ������
  * @return	:0,�ɹ� ����,ʧ��
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 i; 
	
    MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_I2C_Wait_Ack())	
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//���ͼĴ�����ַ
    MPU_I2C_Wait_Ack();	
	
	for(i=0;i<len;i++)
	{
		MPU_I2C_Send_Byte(buf[i]);//��������
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
  * @brief	:i2c������
  * @note   :--
  * @param	:addr, ������ַ
			 reg , Ҫ��ȡ�ļĴ�����ַ
			 len , Ҫ��ȡ�ĳ���
			 buf , ��ȡ�������ݴ洢��
  * @return	:0,�ɹ� ����,ʧ��
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{ 
 	MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//���ͼĴ�����ַ
    MPU_I2C_Wait_Ack();
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((addr<<1)|1);//����������ַ+������	
    MPU_I2C_Wait_Ack();
	while(len)
	{
		if(len==1)
			*buf = MPU_I2C_Read_Byte(0);//������,����nACK 
		else 
			*buf = MPU_I2C_Read_Byte(1);//������,����ACK  
		
		len--;
		buf++; 
	}    
    MPU_I2C_Stop();	//����һ��ֹͣ���� 
	return 0;	
}



/**
  * @brief	:i2cдһ���ֽ�
  * @note   :-
  * @param	:reg , �Ĵ�����ַ
			 data, ����
  * @return	:0,�ɹ� ����,ʧ��
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Write_Byte(u8 reg, u8 data) 				 
{ 
  MPU_I2C_Start(); 
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();		 
		return 1;		
	}
	
    MPU_I2C_Send_Byte(reg);	//���ͼĴ�����ַ
    MPU_I2C_Wait_Ack();	
	
	MPU_I2C_Send_Byte(data);//��������
	if(MPU_I2C_Wait_Ack())
	{
		MPU_I2C_Stop();	 
		return 1;		 
	}		 
    MPU_I2C_Stop();	 
	return 0;
}



/**
  * @brief	:i2c��һ���ֽ� 
  * @note   :--
  * @param	:reg, �Ĵ�����ַ
  * @return	:u8, ���յ��������ֽ�
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	MPU_I2C_Wait_Ack();
	
    MPU_I2C_Send_Byte(reg);//д�Ĵ�����ַ
    MPU_I2C_Wait_Ack();	
	
    MPU_I2C_Start();
	MPU_I2C_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
    MPU_I2C_Wait_Ack();
	
	res = MPU_I2C_Read_Byte(0);//��ȡ����,����nACK 
    MPU_I2C_Stop();	
	
	return res;		
}



/**
  * @brief	:��ȡmpu6050��ǰ�Ƕ� 
  * @note   :--
  * @param	:*AX,*AY,*AZ, ��ǰ����ĽǶ� 
  * @return	:0,�ɹ� ����,ʧ��
  * 
  * @date   :2016/08/27
  * @design :������(LJI)
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
  * @brief	:��ȡmpu6050��ǰ���ٶ� 
  * @note   :--
  * @param	:*GX,*GY,*GZ, ��ǰ����ļ��ٶ� 
  * @return	:0,�ɹ� ����,ʧ��
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
  * @brief	:��ȡmpu6050��ǰ���ٶ� 
  * @note   :--
  * @param	:*OX,*OY,*OZ, ��ǰ����Ľ��ٶ� 
  * @return	:0,�ɹ� ����,ʧ��
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
  * @brief	:��ȡmpu6050��ǰ�������� 
  * @note   :--
  * @param	:*mpu, ��ǰ��������� 
  * @return	:0x00, ��ȡ������
			 0x01, δ��ȡ���Ƕ�����
			 0x02, δ��ȡ�����ٶ�����
			 0x04, δ��ȡ�����ٶ�����
			 0x07, δ��ȡ������
  * 
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU6050_GetData(_mpu6050 *mpu)
{
	u8 err_flag[3]={0,0,0};
	u8 err_res=0;
	
	err_flag[0] = MPU6050_GetAngle(&(mpu->Euler.Pitch), &(mpu->Euler.Roll), &(mpu->Euler.Yaw));	//��ȡ�Ƕ�����
	err_flag[1] = MPU6050_GetAccel(&(mpu->Real.GX), &(mpu->Real.GY), &(mpu->Real.GZ));			//��ȡ���ٶ�����
	err_flag[2] = MPU6050_GetGyro(&(mpu->Real.OX), &(mpu->Real.OY), &(mpu->Real.OZ));			//��ȡ���ٶ�����
	
	err_res = (err_flag[2]<<2)|(err_flag[1]<<1)|(err_flag[0]<<0);
	return err_res;
}



/**
  * @brief	:����mpu6050ŷ���Ǽ����ٶ� 
  * @note   :--
  * @param	:void 
  * @return	:0,�ɹ� ����,ʧ��
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
    
    //���ٶ���ֵ������    
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
	/* ��180���� */
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










