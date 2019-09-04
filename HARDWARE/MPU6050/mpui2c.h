/**
  ******************************************************************************
  * 
  * Mpu6050 i2c������������������stm32f4xxϵ�е�Ƭ��
  * �����޸ģ����޸�mpui2c.h�±���޸Ĳ���
  * 
  * 
  *	����ʱ�䣺2016��08��27��
  * �����£�2016��10��05��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#ifndef __MPUI2C_H__
#define __MPUI2C_H__



/*Mpu_i2c Needed Library Define*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
   	   		   


/********************************* i2c ���û����Ĳ��� **********************************/

#define		MPU_I2C_PORT			GPIOB
#define		MPU_I2C_PIN_SCL			GPIO_Pin_9
#define		MPU_I2C_PIN_SDA			GPIO_Pin_8
#define		MPU_I2C_CLOCK_CMD		RCC_AHB1Periph_GPIOB

/**************************************************************************************/

#define		MPU_I2C_SCL_0()			GPIO_ResetBits(MPU_I2C_PORT, MPU_I2C_PIN_SCL)
#define		MPU_I2C_SCL_1()			GPIO_SetBits(MPU_I2C_PORT, MPU_I2C_PIN_SCL)
#define		MPU_I2C_SDA_0()			GPIO_ResetBits(MPU_I2C_PORT, MPU_I2C_PIN_SDA)
#define		MPU_I2C_SDA_1()			GPIO_SetBits(MPU_I2C_PORT, MPU_I2C_PIN_SDA)

#define		MPU_SDA_READ()			GPIO_ReadInputDataBit(MPU_I2C_PORT, MPU_I2C_PIN_SDA)

#define		MPU_SDA_IN()			MPU_I2C_SDA_InMode()
#define		MPU_SDA_OUT()			MPU_I2C_SDA_OutMode()



/*Mpu_i2c Function Define*/
void MPU_I2C_Init(void);	   //��ʼ��i2c
void MPU_I2C_SDA_InMode(void); //��ʼ��i2c_sdaΪ����
void MPU_I2C_SDA_OutMode(void);//��ʼ��i2c_sdaΪ���

void MPU_I2C_SDA_InMode(void);
void MPU_I2C_SDA_OutMode(void);

void MPU_I2C_Start(void);//����i2c��ʼ�ź�
void MPU_I2C_Stop(void); //����i2cֹͣ�ź�

u8 MPU_I2C_Wait_Ack(void);//�ȴ�Ӧ��

void MPU_I2C_Ack(void); //����ack Ӧ��
void MPU_I2C_NAck(void);//����nackӦ��

void MPU_I2C_Send_Byte(u8 txd);//i2c����һ���ֽ�
u8 MPU_I2C_Read_Byte(u8 ack);  //i2c����һ���ֽ�



#endif


