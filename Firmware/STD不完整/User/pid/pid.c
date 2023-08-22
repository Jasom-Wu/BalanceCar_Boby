#include "pid.h"
extern float pitch,roll,yaw; 										//ŷ����
extern short gyrox,gyroy,gyroz;									//������ԭʼ����
__IO PID_Typedef velocity_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef balance_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef turn_PID = {0,0,0,0,0,0,0,0};
float PID_Process(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder)
{
	static float err_least;
	
	err_least = PIDStructure->target_val - (leftencoder+rightencoder);
	PIDStructure->err *= 0.8;
	PIDStructure->err += 0.2 * err_least;//ȥ�����ָ�Ƶ��ʵ�ֵ�ͨ�˲�
	PIDStructure->integral += PIDStructure->err;
	if(PIDStructure->integral > 10000)//�����޷�
		PIDStructure->integral = 10000;
	else if(PIDStructure->integral < -10000)
		PIDStructure->integral = -10000;
	PIDStructure->output_val = PIDStructure->Kp * PIDStructure->err + PIDStructure->Ki * PIDStructure->integral + PIDStructure->Kd * (PIDStructure->err - PIDStructure->err_last);
	PIDStructure->err_last = PIDStructure->err;
	if(pitch<-35||pitch>35) 			PIDStructure->integral=0;
	return PIDStructure->output_val;
}
/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ���еƽ��Ƕȣ���е��ֵ�������ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
float balance_UP(__IO PID_Typedef *PIDStructure,float Angle,float Gyro)
{  
	 balance_PID.output_val =PIDStructure->Kp * Angle - PIDStructure->Kd*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance_PID.output_val;
}
