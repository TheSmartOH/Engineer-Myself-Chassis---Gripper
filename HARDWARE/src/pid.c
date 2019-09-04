#include "pid.h"

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


void PID_Init(PidTypeDef *pid, const fp32 PID[3], fp32 max_out, fp32 max_iout)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
		pid->number = 8;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}


void Pid_Realize(PidTypeDef *pid)
{
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
	pid->error[0] = pid->ref - pid->fdb;
	if(ABS(pid->error[0]) < 1000) //当偏差在一定范围内时进行积分项的计算
	{
		pid->Iout += pid->error[0] * pid->Ki;
	}
	if(pid->Iout > pid->max_iout)pid->Iout = pid->max_iout;
	else if(pid->Iout < 0-pid->max_iout)pid->Iout = 0-pid->max_iout;
	//计算pid三项
	pid->out = pid->Kp * pid->error[0] + pid->Iout
	+ pid->Kd * (pid->error[0] - pid->error[1]);
	if(pid->out > pid->max_out)pid->out = pid->max_out;
	else if(pid->out < (0-pid->max_out))pid->out = 0-pid->max_out;
	
}

void Pid_PosiSpeeD(PidTypeDef * pidO,PidTypeDef * pidI,int num)
{
	pidO->error[2] = pidO->error[1];
	pidO->error[1] = pidO->error[0];
	pidO->error[0] = pidO->ref - (pidO->fdb + 8192 * num);

	pidO->Pout = pidO->Kp * (pidO->error[0] - pidO->error[1]);
	pidO->Iout = pidO->Ki * pidO->error[0];
	pidO->Dbuf[2] = pidO->Dbuf[1];
	pidO->Dbuf[1] = pidO->Dbuf[0];
	pidO->Dbuf[0] = (pidO->error[0] - 2.0f * pidO->error[1] + pidO->error[2]);
	pidO->Dout = pidO->Kd * pidO->Dbuf[0];
	pidO->out += pidO->Pout + pidO->Iout + pidO->Dout;
	LimitMax(pidO->out, pidO->max_out);

	pidI->ref = pidO->out;
	pidI->error[2] = pidI->error[1];
	pidI->error[1] = pidI->error[0];
	pidI->error[0] = pidI->ref - pidI->fdb;

	pidI->Pout = pidI->Kp * (pidI->error[0] - pidI->error[1]);
	pidI->Iout = pidI->Ki * pidI->error[0];
	pidI->Dbuf[2] = pidI->Dbuf[1];
	pidI->Dbuf[1] = pidI->Dbuf[0];
	pidI->Dbuf[0] = (pidI->error[0] - 2.0f * pidI->error[1] + pidI->error[2]);
	pidI->Dout = pidI->Kd * pidI->Dbuf[0];
	pidI->out += pidI->Pout + pidI->Iout + pidI->Dout;
	LimitMax(pidI->out, pidI->max_out);
	
}


void Pid_PosiSpeeP(PidTypeDef * pidO,PidTypeDef * pidI,int num)
{
	pidO->error[2] = pidO->error[1];
	pidO->error[1] = pidO->error[0];
	pidO->error[0] = pidO->ref - (pidO->fdb + 8192 * num);

	pidO->Pout = pidO->Kp * pidO->error[0];
	pidO->Iout += pidO->Ki * pidO->error[0];
	pidO->Dbuf[2] = pidO->Dbuf[1];
	pidO->Dbuf[1] = pidO->Dbuf[0];
	pidO->Dbuf[0] = (pidO->error[0] - pidO->error[1]);
	pidO->Dout = pidO->Kd * pidO->Dbuf[0];
	pidO->out = pidO->Pout + pidO->Iout + pidO->Dout;
	LimitMax(pidO->Iout, pidO->max_iout);

	pidI->ref = pidO->out;
	pidI->error[2] = pidI->error[1];
	pidI->error[1] = pidI->error[0];
	pidI->error[0] = pidI->ref - pidI->fdb;

	pidI->Pout = pidI->Kp * pidI->error[0];
	pidI->Iout += pidI->Ki * pidI->error[0];
	pidI->Dbuf[2] = pidI->Dbuf[1];
	pidI->Dbuf[1] = pidI->Dbuf[0];
	pidI->Dbuf[0] = (pidI->error[0] - pidI->error[1]);
	pidI->Dout = pidI->Kd * pidI->Dbuf[0];
	pidI->out = pidI->Pout + pidI->Iout + pidI->Dout;
	LimitMax(pidI->out, pidI->max_out);
	
}


