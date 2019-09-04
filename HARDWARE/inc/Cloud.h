#ifndef __cloud_H
#define __cloud_H



#define CLOUD_GYRO_X_OFFSET            -1.0730f                     //俯仰陀螺仪x轴方向偏移量(云台)
#define CLOUD_GYRO_Y_OFFSET	    	    0.5325f                     //俯仰陀螺仪y轴方向偏移量(云台)
#define CLOUD_GYRO_Z_OFFSET            -1.8381f                     //俯仰陀螺仪z轴方向偏移量(云台)


#define CLOUD_GYRO_X_ZERO_CORRECT       0.6f                        //俯仰陀螺仪x轴方向死区修正(云台)
#define CLOUD_GYRO_Y_ZERO_CORRECT       0.6f                        //俯仰陀螺仪y轴方向死区修正(云台)
#define CLOUD_GYRO_Z_ZERO_CORRECT       0.8f                        //俯仰陀螺仪z轴方向死区修正(云台)


void Cloud_Control(void);

void MpuGetData(void);






#endif /*__cloud_H*/

