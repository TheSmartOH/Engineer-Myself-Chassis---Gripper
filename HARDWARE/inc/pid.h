#ifndef __PID_H
#define __PID_H

#include "system.h"
typedef struct
{
		u8 number;
    //PID 三参数
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出
    fp32 max_iout; //最大积分输出

    fp32 ref;
		fp32 nowfdb;
    fp32 fdb;
		fp32 lastfdb;
	
    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次

} PidTypeDef;


void PID_Init(PidTypeDef *pid, const fp32 PID[3], fp32 max_out, fp32 max_iout);
void Pid_Realize(PidTypeDef *pid);
void Pid_PosiSpeeD(PidTypeDef * pidO,PidTypeDef * pidI,int num);
void Pid_PosiSpeeP(PidTypeDef * pidO,PidTypeDef * pidI,int num);









#endif


