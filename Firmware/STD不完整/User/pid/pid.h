#ifndef __PID_H_
#define __PID_H_
#include "stm32f10x.h"
typedef struct//8��
{
	float target_val;   //Ŀ��ֵ
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float integral;     //����ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float output_val;   //���ֵ
}PID_Typedef;
extern __IO PID_Typedef velocity_PID;
extern __IO PID_Typedef balance_PID;
extern __IO PID_Typedef turn_PID;
float PID_Process(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder);
float balance_UP(__IO PID_Typedef *PIDStructure,float Angle,float Gyro);
#endif
