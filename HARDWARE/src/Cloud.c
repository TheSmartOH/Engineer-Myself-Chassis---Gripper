#include "system.h"
#include "cloud.h"
PidTypeDef Cloud_Pid[2];

s32 PitchMotor = 0;
s32 YawMotor = 0;

void Cloud_Control(void)
{
	_RemoteStruct * rcdata = GetRemoteData();                             //得到遥控的数据
	_MouseStruct *mouse = GetMouseData();//获取鼠标移动值
	
	
	
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
    
/****** 读取角度必须添加，不需要角度也必须读取，目前是个bug，需要解决！-- 2017.04.02 ******/
	result += MPU6050_GetAngle(&pitch, &roll, &yaw);//必须读取角度，由dmp计算角度来表示延时！
//    if(result) return 1;
/******************************************************************************************/    
	MpuDataCloud.Euler.Pitch = pitch;
	MpuDataCloud.Euler.Roll  = roll;
	MpuDataCloud.Euler.Yaw   = yaw;

	//中和mpu的个体差异，静止时对 0 校正！-- 2017.04.02
	MpuDataCloud.Real.OX = -ox + CLOUD_GYRO_X_OFFSET;
	MpuDataCloud.Real.OY = -oy + CLOUD_GYRO_Y_OFFSET;
	MpuDataCloud.Real.OZ = -oz + CLOUD_GYRO_Z_OFFSET;

//角速度零值修正，目的是建立死区，mpu波纹浮动，不如卡尔曼滤波好，但这种粗暴滤波效果极佳！-- 2017.04.02
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
















