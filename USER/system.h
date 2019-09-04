#ifndef __SYSTEM_H
#define __SYSTEM_H

/* freertos �����ļ� */
#include "FreeRTOSConfig.h"

/*FreeRTOS���ͷ�ļ�*/
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
/* Project includes */
#include "startTask.h"
/*��������*/
#include "arm_math.h"
#include "Func_Usermath.h"
/*�ײ�Ӳ������*/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
/*��չģ������*/
#include "Driver_Remote.h"
#include "Driver_KeyMouse.h"
#include "Chassis.h"
#include "pid.h"
#include "cloud.h"
/*Mpu6050*/
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "mpui2c.h"
/*Board Support Package����*/
#include "Bsp_can.h"
#include "Bsp_tim.h"
#include "Bsp_usart2.h"
void systemInit(void);

#endif /* __SYSTEM_H */

