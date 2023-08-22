#ifndef __PID_H_
#define __PID_H_
#include "stm32f1xx_hal.h"
typedef struct//8个
{
	__IO float target_val;   //目标值
	__IO float err;          //偏差值
	__IO float err_last;     //上一个偏差值
	__IO float integral;     //积分值
	__IO float Kp,Ki,Kd;     //比例、积分、微分系数
	__IO float output_val;   //输出值
}PID_Typedef;
extern __IO PID_Typedef velocity_PID;
extern __IO PID_Typedef balance_PID;
extern __IO PID_Typedef turn_PID;
extern float Mechanical_balance;
extern short Gyroy_error;
float velocity_Control(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder);
float balance_Control(__IO PID_Typedef *PIDStructure,float Angle,float Gyro);
float turn_Control(__IO PID_Typedef *PIDStructure,int gyro_Z);
#endif
