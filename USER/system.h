#ifndef __SYSTEM_H
#define __SYSTEM_H

/* freertos 配置文件 */
#include "FreeRTOSConfig.h"

/*FreeRTOS相关头文件*/
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
/* Project includes */
#include "startTask.h"
/*算数驱动*/
#include "arm_math.h"
#include "Func_Usermath.h"
/*底层硬件驱动*/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
/*扩展模块驱动*/
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
/*Board Support Package驱动*/
#include "Bsp_can.h"
#include "Bsp_tim.h"
#include "Bsp_usart2.h"
void systemInit(void);

#endif /* __SYSTEM_H */

