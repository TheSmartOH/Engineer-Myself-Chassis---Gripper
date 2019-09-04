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
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#include "mpui2c.h"
//#include "Bsp_Usart1.h"//�����ļ�ʱʹ�ã�����ʹ��ʱ��ɾ����������ܻᱨ��



#define		MPU_I2C_Delay(x)	delay_us(x)	//���ģ��i2c��ʱ����ʱ
#define		MPU_ERR_TIME_MAX	200			//���ȴ��ӻ�Ӧ��ʱ��

/**
  * @brief	:��ʼ��i2c
		     ��ʼ��i2c_sdaΪ����
			 ��ʼ��i2c_sdaΪ���
  * @note   :--
  * @param	:void
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_Init(void)
{					     
	GPIO_InitTypeDef  gpio;

	RCC_AHB1PeriphClockCmd(MPU_I2C_CLOCK_CMD, ENABLE);//ʹ��GPIOCʱ��

	gpio.GPIO_Pin   = MPU_I2C_PIN_SCL
					| MPU_I2C_PIN_SDA;
	gpio.GPIO_Mode  = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(MPU_I2C_PORT, &gpio);
	
	MPU_I2C_SCL_1();
	MPU_I2C_SDA_1();
}

void MPU_I2C_SDA_InMode(void)
{
	GPIO_InitTypeDef	gpio;
		
	gpio.GPIO_Pin  = MPU_I2C_PIN_SDA;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	 
	GPIO_Init(MPU_I2C_PORT, &gpio);
}

void MPU_I2C_SDA_OutMode(void)
{
	GPIO_InitTypeDef	gpio;
		
	gpio.GPIO_Pin   = MPU_I2C_PIN_SDA;
	gpio.GPIO_Mode  = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(MPU_I2C_PORT, &gpio);
	
	MPU_I2C_SDA_1();
}



/**
  * @brief	:����i2c��ʼ�ź�
  * @note   :--
  * @param	:void
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_Start(void)
{
	MPU_SDA_OUT();	//sda����Ϊ���	
	MPU_I2C_SDA_1();	  	  
	MPU_I2C_SCL_1();//sclΪ�ߵ�ƽʱ
	MPU_I2C_Delay(4);
 	MPU_I2C_SDA_0();//sda�½���Ϊ��ʼ�ź�
	MPU_I2C_Delay(4);
	MPU_I2C_SCL_0();//׼�����ͻ�������� 
}	  



/**
  * @brief	:����i2cֹͣ�ź�
  * @note   :--
  * @param	:void
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_Stop(void)
{
	MPU_SDA_OUT();	//sda����Ϊ���	
	MPU_I2C_SCL_0();//scl�͵�ƽʱ����sda�ı�
	MPU_I2C_SDA_0();
 	MPU_I2C_Delay(4);
	MPU_I2C_SCL_1();//sclΪ�ߵ�ƽʱ 
	MPU_I2C_SDA_1();//sda������Ϊֹͣ�ź�
	MPU_I2C_Delay(4);							   	
}



/**
  * @brief	:�ȴ�i2cӦ���ź�
  * @note   :--
  * @param	:void
  * @return	:1, ����Ӧ��ʧ��
			 0, ����Ӧ��ɹ�
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_I2C_Wait_Ack(void)
{
	u8 Err_Time=0;
	
	MPU_SDA_OUT();	//sda����Ϊ��� 	
	MPU_I2C_SDA_1();
	MPU_I2C_Delay(1);	   
	MPU_I2C_SCL_1();
	MPU_I2C_Delay(1);	
	
	MPU_SDA_IN();	//sda����Ϊ���� 		
	while(MPU_SDA_READ())//�ȴ�Ӧ���źŵ���
	{
		Err_Time++;
		if(Err_Time>MPU_ERR_TIME_MAX)
		{
			MPU_I2C_Stop();
			return 1;
		}
	}
	
	MPU_I2C_SCL_0(); 	   
	return 0;  
} 



/**
  * @brief	:����ackӦ��
  * @note   :--
  * @param	:void
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_Ack(void)
{
	MPU_I2C_SCL_0();
	
	MPU_SDA_OUT();	//sda����Ϊ���
	MPU_I2C_SDA_0();//sda�͵�ƽ
	MPU_I2C_Delay(2);
	MPU_I2C_SCL_1();//scl�½���Ϊack�ź�
	MPU_I2C_Delay(2);
	MPU_I2C_SCL_0();
}



/**
  * @brief	:����nackӦ��
  * @note   :--
  * @param	:void
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_NAck(void)
{
	MPU_I2C_SCL_0();
	
	MPU_SDA_OUT();	//sda����Ϊ���
	MPU_I2C_SDA_1();//sda�ߵ�ƽ
	MPU_I2C_Delay(2);
	MPU_I2C_SCL_1();//scl�½���Ϊnack�ź�
	MPU_I2C_Delay(2);
	MPU_I2C_SCL_0();
}	



/**
  * @brief	:i2c����һ���ֽ�
  * @note   :--
  * @param	:txd, �����͵������ֽ�
  * @return	:void
  *
  * @date   :2016/08/27
  * @design :
  **/
void MPU_I2C_Send_Byte(u8 txd)
{                        
    u8 i;   
	
	MPU_SDA_OUT();	//sda����Ϊ��� 	    
    MPU_I2C_SCL_0();
	
    for(i=0;i<8;i++)//��λ���ȷ���
    {              
		if(txd&0x80)
		{
			MPU_I2C_SDA_1();
		}
		else
		{
			MPU_I2C_SDA_0();
		} 		
        txd <<= 1; 
		
		MPU_I2C_Delay(2);//����������ʱ���룡 
		MPU_I2C_SCL_1(); //scl�½��ط�������
		MPU_I2C_Delay(2); 
		MPU_I2C_SCL_0();	
		MPU_I2C_Delay(2);
    }	 
} 	    



/**
  * @brief	:i2c��ȡһ���ֽ�
  * @note   :--
  * @param	:ack, (1,ack 0,nack)
  * @return	:u8,���յ�����
  *
  *
  * @date   :2016/08/27
  * @design :
  **/
u8 MPU_I2C_Read_Byte(u8 ack)
{
	u8 i,rec_dat=0;
	
	MPU_SDA_IN();//sda����Ϊ����
    for(i=0;i<8;i++)
	{
        MPU_I2C_SCL_0(); 
        MPU_I2C_Delay(2);
		MPU_I2C_SCL_1();
		
        rec_dat <<= 1;	
        if(MPU_SDA_READ())
			rec_dat++;   
		
		MPU_I2C_Delay(1); 
    }	
	
    if (!ack)
        MPU_I2C_NAck();//����nack
    else
        MPU_I2C_Ack(); //����ack  
	
    return rec_dat;
}




