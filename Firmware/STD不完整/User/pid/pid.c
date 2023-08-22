#include "pid.h"
extern float pitch,roll,yaw; 										//欧拉角
extern short gyrox,gyroy,gyroz;									//陀螺仪原始数据
__IO PID_Typedef velocity_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef balance_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef turn_PID = {0,0,0,0,0,0,0,0};
float PID_Process(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder)
{
	static float err_least;
	
	err_least = PIDStructure->target_val - (leftencoder+rightencoder);
	PIDStructure->err *= 0.8;
	PIDStructure->err += 0.2 * err_least;//去除部分高频，实现低通滤波
	PIDStructure->integral += PIDStructure->err;
	if(PIDStructure->integral > 10000)//积分限幅
		PIDStructure->integral = 10000;
	else if(PIDStructure->integral < -10000)
		PIDStructure->integral = -10000;
	PIDStructure->output_val = PIDStructure->Kp * PIDStructure->err + PIDStructure->Ki * PIDStructure->integral + PIDStructure->Kd * (PIDStructure->err - PIDStructure->err_last);
	PIDStructure->err_last = PIDStructure->err;
	if(pitch<-35||pitch>35) 			PIDStructure->integral=0;
	return PIDStructure->output_val;
}
/**************************************************************************
函数功能：直立PD控制
入口参数：角度、机械平衡角度（机械中值）、角速度
返回  值：直立控制PWM
**************************************************************************/
float balance_UP(__IO PID_Typedef *PIDStructure,float Angle,float Gyro)
{  
	 balance_PID.output_val =PIDStructure->Kp * Angle - PIDStructure->Kd*Gyro;  //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance_PID.output_val;
}
