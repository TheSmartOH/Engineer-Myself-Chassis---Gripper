/**
  ******************************************************************************
  * 
  * 高精度惯性导航模块 JY-901 使用说明书(本驱动库适用芯片：stm32f4xx系列)
  *	
  * 产品概述：
  *     模块集成高精度的陀螺仪、加速度计、地磁场传感器，采用高性能的微处理器和
  *     先进的动力学解算与卡尔曼动态滤波算法，能够快速求解出实时运动姿态；
  *
  * 产品特性：
  *     1.支持usart和i2c两种数字接口：usart速率为2400bps~921600bps
  *                                   i2c支持全速400k速率；
  *     2.最高200Hz数据输出速率，输入内容可通过上位机选择，输出速率可调；
  *     3.保留4路扩展接口，可配置成模拟输入、数字输入、数字输出、pwm输出；
  *     4.具备GPS连接能力，可接受符合NMEA-0183标准的串口GPS数据；
  *
  * 性能参数：  
  *     1.电压：3V~6V；
  *     2.电流：<40mA；
  *     3.测量维度：3轴加速度、3轴角速度、3维磁场；
  *     4.姿态测量稳定度：0.01°；
  *     5.数据输出速率：0.1Hz~200Hz；
  *     6.数据接口：usart(TTL电平，波特率支持2400、4800、9600、19200、38400、
  *     57600、115200、230400、460800、921600)，i2c支持高速i2c速率400K；
  *     7、扩展口功能： 模拟输入(0~Vcc)、
  *                     数字输入、
  *                     数字输出、
  *                     pwm输出(周期1us~65535us，分辨率1us)；
  *
  *  相关资料：
  *     淘宝：君悦智控
  *     网址：http://Robotcontrol.taobao.com
  *
  *  特别注意：
  *     本文件仅保留与JY-901进行usart通信使用的数据类型说明；
  *     其他的已被删减，若需要使用i2c控制，自行准备mpu6050的控制文件！
  *
  *  
  *	创建时间：2017年04月15日
  * 最后更新：2017年04月15日
  * 创 建 人：梁宗南(LJI)
  * E_Mail	：liangzongnan0214@163.com
  * MyBlog  ：liangzongnan.com
  * QQ		：494089986
  * 
  ******************************************************************************
***/



#include "mpu6050_jy901.h"


_JY901TimeStruct		jy901Time;  //时间参数
_JY901AccStruct         jy901Acc;   //加速度参数
_JY901GyroStruct 		jy901Gyro;  //角速度参数
_JY901AngleStruct       jy901Angle; //角度参数
_JY901MagStruct 		jy901Mag;   //地磁场参数

float YawStartAngle=0.0f;           //yaw轴初始角度

/**
  * @brief	:串口中断调用函数，串口每收到一个数据，调用一次这个函数
  * @note   :--放置于串口中断服务函数汇总，例如：
               void USART1_IRQHandler(void)
               {            
                   if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
                   {        
                       USART_ClearITPendingBit(USART1, USART_IT_RXNE);
                       USART_ClearFlag(USART1, USART_FLAG_RXNE);
                        
                       CopeSerialData((unsigned char)USART1->DR);//处理数据
                   }
               }
  * @date   :2017/04/15
  * @design :梁宗南(LJI)
  **/
u8 YawAngleFirstGet=0;
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt=0;	
	
	ucRxBuffer[ucRxCnt++] = ucData;
	if(ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt = 0;
		return;
	}
    
	if(ucRxCnt<11)//数据不满11个，则返回
    {
        return;
    }else{
		switch(ucRxBuffer[1])
		{
            /*memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。*/
			case 0x50: memcpy(&jy901Time,    &ucRxBuffer[2], 8); break;
			case 0x51: memcpy(&jy901Acc,     &ucRxBuffer[2], 8); break;
			case 0x52: memcpy(&jy901Gyro,    &ucRxBuffer[2], 8); break;
			case 0x53: memcpy(&jy901Angle,   &ucRxBuffer[2], 8), YawAngleFirstGet = 1; break;
			case 0x54: memcpy(&jy901Mag,     &ucRxBuffer[2], 8); break;
		}
		ucRxCnt = 0;
	}
}










//注意：
//以下程序是RM2017比赛使用，并非整理的jy901库中的程序！
//如需要更改，请自行更改usart的配置！！
//特别的，模块使用需要使用上位机设置相关参数，如默认，则96、n、8、1格式！！！
/**
  * @brief	:jy901 mpu6050模块通信初始化
  * @note   :--使用前需要使用上位机对模块进行初始化！
  * @param	:Baud  ,波特率
			 NvicPP,抢占优先级
			 NvicSP,响应优先级
  * @return	:void
  *
  * @date   :2017/05/03
  * @design :梁宗南(LJI)
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
    gpio.GPIO_OType = GPIO_OType_OD;//复用功能只针对输出！  
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
    GPIO_Init(GPIOC, &gpio); 
 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);//PC10:TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);//PC11:RX	            
    
	usart.USART_BaudRate   = Baud;//波特率设置
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits   = USART_StopBits_1;
	usart.USART_Parity     = USART_Parity_No; 
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3, &usart); 

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//接收采用中断
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);        
    
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = NvicPP;
    nvic.NVIC_IRQChannelSubPriority        = NvicSP;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);//rx使用中断！

	USART_Cmd(USART3, ENABLE);
}



/**
  * @brief	:jy901 mpu6050模块角度显示
  * @note   :--使用0.96寸oled模块显示，使用malloc内存管理！
             --不要忘记使用完了要释放内存！！！
  * @param	:void
  * @return	:void
  *
  * @date   :2017/05/03
  * @design :梁宗南(LJI)
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
  * @brief	:转化成连续的z轴角度
  * @note   :--程序复位，则圈数复位，即重新转化角度！！！
  * @param	:void
  * @return	:float, 转化后的连续角度
  *
  * @date   :2017/06/18
  * @design :梁宗南(LJI)
  **/
_IMUAngleSumStruct IMUYawAngle={0.0f, 0};
float JY901_GetContinuousAngle(void)
{
    static float real_angle=0.0f,last_angle = 0.0f;
    
    /* yaw轴角度经过处理后变成连续的 */
	last_angle = real_angle;
	real_angle = GET_JY901_Z_ANGLE();

    if(last_angle > 90.0f && real_angle < -90.0f)//圈数过圈判断！
    {
        IMUYawAngle.turnsCnt++;
    }else if(last_angle < -90.0f && real_angle > 90.0f)
    {
        IMUYawAngle.turnsCnt--;    
    }
    
	IMUYawAngle.totalAngle = real_angle + (float)(IMUYawAngle.turnsCnt * 360);//yaw轴角度
    return IMUYawAngle.totalAngle;
}





/**
  * @brief	:
  * @note   :--串口3中断
  * @param	:
  * @return	:
  *
  * @date   :2017/05/03
  * @design :梁宗南(LJI)
  **/
void USART3_IRQHandler(void)
{            
	if(USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET)
    {        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
            
        CopeSerialData((unsigned char)USART3->DR);//处理数据
    }
}










