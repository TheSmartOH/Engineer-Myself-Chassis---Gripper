/**
  ******************************************************************************
  * 
  * �߾��ȹ��Ե���ģ�� JY-901 ʹ��˵����(������������оƬ��stm32f4xxϵ��)
  *	
  * ��Ʒ������
  *     ģ�鼯�ɸ߾��ȵ������ǡ����ٶȼơ��شų������������ø����ܵ�΢��������
  *     �Ƚ��Ķ���ѧ�����뿨������̬�˲��㷨���ܹ���������ʵʱ�˶���̬��
  *
  * ��Ʒ���ԣ�
  *     1.֧��usart��i2c�������ֽӿڣ�usart����Ϊ2400bps~921600bps
  *                                   i2c֧��ȫ��400k���ʣ�
  *     2.���200Hz����������ʣ��������ݿ�ͨ����λ��ѡ��������ʿɵ���
  *     3.����4·��չ�ӿڣ������ó�ģ�����롢�������롢���������pwm�����
  *     4.�߱�GPS�����������ɽ��ܷ���NMEA-0183��׼�Ĵ���GPS���ݣ�
  *
  * ���ܲ�����  
  *     1.��ѹ��3V~6V��
  *     2.������<40mA��
  *     3.����ά�ȣ�3����ٶȡ�3����ٶȡ�3ά�ų���
  *     4.��̬�����ȶ��ȣ�0.01�㣻
  *     5.����������ʣ�0.1Hz~200Hz��
  *     6.���ݽӿڣ�usart(TTL��ƽ��������֧��2400��4800��9600��19200��38400��
  *     57600��115200��230400��460800��921600)��i2c֧�ָ���i2c����400K��
  *     7����չ�ڹ��ܣ� ģ������(0~Vcc)��
  *                     �������롢
  *                     ���������
  *                     pwm���(����1us~65535us���ֱ���1us)��
  *
  *  ������ϣ�
  *     �Ա��������ǿ�
  *     ��ַ��http://Robotcontrol.taobao.com
  *
  *  �ر�ע�⣺
  *     ���ļ���������JY-901����usartͨ��ʹ�õ���������˵����
  *     �������ѱ�ɾ��������Ҫʹ��i2c���ƣ�����׼��mpu6050�Ŀ����ļ���
  *
  *  
  *	����ʱ�䣺2017��04��15��
  * �����£�2017��04��15��
  * �� �� �ˣ�������(LJI)
  * E_Mail	��liangzongnan0214@163.com
  * MyBlog  ��liangzongnan.com
  * QQ		��494089986
  * 
  ******************************************************************************
***/



#include "mpu6050_jy901.h"


_JY901TimeStruct		jy901Time;  //ʱ�����
_JY901AccStruct         jy901Acc;   //���ٶȲ���
_JY901GyroStruct 		jy901Gyro;  //���ٶȲ���
_JY901AngleStruct       jy901Angle; //�ǶȲ���
_JY901MagStruct 		jy901Mag;   //�شų�����

float YawStartAngle=0.0f;           //yaw���ʼ�Ƕ�

/**
  * @brief	:�����жϵ��ú���������ÿ�յ�һ�����ݣ�����һ���������
  * @note   :--�����ڴ����жϷ��������ܣ����磺
               void USART1_IRQHandler(void)
               {            
                   if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
                   {        
                       USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                       USART_ClearFlag(USART1, USART_FLAG_RXNE);
                        
                       CopeSerialData((unsigned char)USART1->DR);//��������
                   }
               }
  * @date   :2017/04/15
  * @design :������(LJI)
  **/
u8 YawAngleFirstGet=0;
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt=0;	
	
	ucRxBuffer[ucRxCnt++] = ucData;
	if(ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt = 0;
		return;
	}
    
	if(ucRxCnt<11)//���ݲ���11�����򷵻�
    {
        return;
    }else{
		switch(ucRxBuffer[1])
		{
            /*memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����*/
			case 0x50: memcpy(&jy901Time,    &ucRxBuffer[2], 8); break;
			case 0x51: memcpy(&jy901Acc,     &ucRxBuffer[2], 8); break;
			case 0x52: memcpy(&jy901Gyro,    &ucRxBuffer[2], 8); break;
			case 0x53: memcpy(&jy901Angle,   &ucRxBuffer[2], 8), YawAngleFirstGet = 1; break;
			case 0x54: memcpy(&jy901Mag,     &ucRxBuffer[2], 8); break;
		}
		ucRxCnt = 0;
	}
}










//ע�⣺
//���³�����RM2017����ʹ�ã����������jy901���еĳ���
//����Ҫ���ģ������и���usart�����ã���
//�ر�ģ�ģ��ʹ����Ҫʹ����λ��������ز�������Ĭ�ϣ���96��n��8��1��ʽ������
/**
  * @brief	:jy901 mpu6050ģ��ͨ�ų�ʼ��
  * @note   :--ʹ��ǰ��Ҫʹ����λ����ģ����г�ʼ����
  * @param	:Baud  ,������
			 NvicPP,��ռ���ȼ�
			 NvicSP,��Ӧ���ȼ�
  * @return	:void
  *
  * @date   :2017/05/03
  * @design :������(LJI)
  **/
void JY901_Init(u32 Baud, u8 NvicPP, u8 NvicSP)
{
	GPIO_InitTypeDef	gpio;
	USART_InitTypeDef	usart;
	NVIC_InitTypeDef	nvic;	

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    gpio.GPIO_Pin   = GPIO_Pin_10;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &gpio);
    
    gpio.GPIO_Pin   = GPIO_Pin_11; 
    gpio.GPIO_OType = GPIO_OType_OD;//���ù���ֻ��������  
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOC, &gpio); 
 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);//PC10:TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);//PC11:RX	            
    
	usart.USART_BaudRate   = Baud;//����������
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits   = USART_StopBits_1;
	usart.USART_Parity     = USART_Parity_No; 
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3, &usart); 

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���ղ����ж�
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);        
    
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
    nvic.NVIC_IRQChannelSubPriority        = NvicSP;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);//rxʹ���жϣ�

	USART_Cmd(USART3, ENABLE);
}



/**
  * @brief	:jy901 mpu6050ģ��Ƕ���ʾ
  * @note   :--ʹ��0.96��oledģ����ʾ��ʹ��malloc�ڴ����
             --��Ҫ����ʹ������Ҫ�ͷ��ڴ棡����
  * @param	:void
  * @return	:void
  *
  * @date   :2017/05/03
  * @design :������(LJI)
  **/
void JY901_AngleDisplay(void)
{
    char *str1 = (char *)malloc(15);
    char *str2 = (char *)malloc(15);
    char *str3 = (char *)malloc(15);
    
    sprintf(str1, "AngX:%7.3f   ", GET_JY901_X_ANGLE());
    sprintf(str2, "AngY:%7.3f   ", GET_JY901_Y_ANGLE());
    sprintf(str3, "AngZ:%7.3f   ", GET_JY901_Z_ANGLE());
    
    OLED_DisplaySTR(0, 0, "jy901 Angle:\0", 8, 0);
    OLED_DisplaySTR(0, 1, (u8 *)str1, 8, 0);
    OLED_DisplaySTR(0, 2, (u8 *)str2, 8, 0);
    OLED_DisplaySTR(0, 3, (u8 *)str3, 8, 0);
    OLED_Update();
    
    free(str1);
    free(str2);
    free(str3);        
}



/**
  * @brief	:ת����������z��Ƕ�
  * @note   :--����λ����Ȧ����λ��������ת���Ƕȣ�����
  * @param	:void
  * @return	:float, ת����������Ƕ�
  *
  * @date   :2017/06/18
  * @design :������(LJI)
  **/
_IMUAngleSumStruct IMUYawAngle={0.0f, 0};
float JY901_GetContinuousAngle(void)
{
    static float real_angle=0.0f,last_angle = 0.0f;
    
    /* yaw��ǶȾ���������������� */
	last_angle = real_angle;
	real_angle = GET_JY901_Z_ANGLE();

    if(last_angle > 90.0f && real_angle < -90.0f)//Ȧ����Ȧ�жϣ�
    {
        IMUYawAngle.turnsCnt++;
    }else if(last_angle < -90.0f && real_angle > 90.0f)
    {
        IMUYawAngle.turnsCnt--;    
    }
    
	IMUYawAngle.totalAngle = real_angle + (float)(IMUYawAngle.turnsCnt * 360);//yaw��Ƕ�
    return IMUYawAngle.totalAngle;
}





/**
  * @brief	:
  * @note   :--����3�ж�
  * @param	:
  * @return	:
  *
  * @date   :2017/05/03
  * @design :������(LJI)
  **/
void USART3_IRQHandler(void)
{            
	if(USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET)
    {        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
            
        CopeSerialData((unsigned char)USART3->DR);//��������
    }
}










