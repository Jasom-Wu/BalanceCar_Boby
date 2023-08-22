#ifndef __PID_H_
#define __PID_H_
#include "stm32f10x.h"
typedef struct//8个
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float integral;     //积分值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float output_val;   //输出值
}PID_Typedef;
extern __IO PID_Typedef velocity_PID;
extern __IO PID_Typedef balance_PID;
extern __IO PID_Typedef turn_PID;
float PID_Process(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder);
float balance_UP(__IO PID_Typedef *PIDStructure,float Angle,float Gyro);
#endif
