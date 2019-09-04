#include "system.h"
#include "cloud.h"
PidTypeDef Cloud_Pid[2];

s32 PitchMotor = 0;
s32 YawMotor = 0;

void Cloud_Control(void)
{
	_RemoteStruct * rcdata = GetRemoteData();                             //�õ�ң�ص�����
	_MouseStruct *mouse = GetMouseData();//��ȡ����ƶ�ֵ
	
	
	
	CAN2_SendChassisMail(0x200,rcdata->s1, 0, 0, 0);
	
	
}





void MpuGetData(void)
{
	float pitch, roll, yaw;
	float ox, oy, oz;
	float gx, gy, gz;
	u8 result = 0;
			
	result += MPU6050_GetGyro(&ox, &oy, &oz);
//    if(result)return 1;
	result += MPU6050_GetAccel(&gx, &gy, &gz);
//    if(result)return 1;
    
/****** ��ȡ�Ƕȱ�����ӣ�����Ҫ�Ƕ�Ҳ�����ȡ��Ŀǰ�Ǹ�bug����Ҫ�����-- 2017.04.02 ******/
	result += MPU6050_GetAngle(&pitch, &roll, &yaw);//�����ȡ�Ƕȣ���dmp����Ƕ�����ʾ��ʱ��
//    if(result) return 1;
/******************************************************************************************/    
	MpuDataCloud.Euler.Pitch = pitch;
	MpuDataCloud.Euler.Roll  = roll;
	MpuDataCloud.Euler.Yaw   = yaw;

	//�к�mpu�ĸ�����죬��ֹʱ�� 0 У����-- 2017.04.02
	MpuDataCloud.Real.OX = -ox + CLOUD_GYRO_X_OFFSET;
	MpuDataCloud.Real.OY = -oy + CLOUD_GYRO_Y_OFFSET;
	MpuDataCloud.Real.OZ = -oz + CLOUD_GYRO_Z_OFFSET;

//���ٶ���ֵ������Ŀ���ǽ���������mpu���Ƹ��������翨�����˲��ã������ֱִ��˲�Ч�����ѣ�-- 2017.04.02
	MpuDataCloud.Real.OX = ABS(MpuDataCloud.Real.OX)<CLOUD_GYRO_X_ZERO_CORRECT ? 0.0f : MpuDataCloud.Real.OX;
	MpuDataCloud.Real.OY = ABS(MpuDataCloud.Real.OY)<CLOUD_GYRO_Y_ZERO_CORRECT ? 0.0f : MpuDataCloud.Real.OY;
	MpuDataCloud.Real.OZ = ABS(MpuDataCloud.Real.OZ)<CLOUD_GYRO_Z_ZERO_CORRECT ? 0.0f : MpuDataCloud.Real.OZ;
	
	MpuDataCloud.Real.GX = gx;
	MpuDataCloud.Real.GY = gy; 
	MpuDataCloud.Real.GZ = gz;
//	Speed_Pid[1].fdb = MpuDataCloud.Real.OZ;
//	fp32 Wave[3] ={0,0,0};
//	Wave[0] = MpuDataCloud.Real.OY;
//	Wave[1] = MpuDataCloud.Real.GY;
//	Wave[2] = MpuDataCloud.Euler.Roll;
//	shanwai_displaywave2(Wave,3);
}
















