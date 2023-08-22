#ifndef __PID_H_
#define __PID_H_
#include "stm32f1xx_hal.h"
typedef struct//8��
{
	__IO float target_val;   //Ŀ��ֵ
	__IO float err;          //ƫ��ֵ
	__IO float err_last;     //��һ��ƫ��ֵ
	__IO float integral;     //����ֵ
	__IO float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	__IO float output_val;   //���ֵ
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
