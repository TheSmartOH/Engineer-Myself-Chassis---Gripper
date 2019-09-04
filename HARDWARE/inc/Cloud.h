#ifndef __cloud_H
#define __cloud_H



#define CLOUD_GYRO_X_OFFSET            -1.0730f                     //����������x�᷽��ƫ����(��̨)
#define CLOUD_GYRO_Y_OFFSET	    	    0.5325f                     //����������y�᷽��ƫ����(��̨)
#define CLOUD_GYRO_Z_OFFSET            -1.8381f                     //����������z�᷽��ƫ����(��̨)


#define CLOUD_GYRO_X_ZERO_CORRECT       0.6f                        //����������x�᷽����������(��̨)
#define CLOUD_GYRO_Y_ZERO_CORRECT       0.6f                        //����������y�᷽����������(��̨)
#define CLOUD_GYRO_Z_ZERO_CORRECT       0.8f                        //����������z�᷽����������(��̨)


void Cloud_Control(void);

void MpuGetData(void);






#endif /*__cloud_H*/

