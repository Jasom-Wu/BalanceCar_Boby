#include "pid.h"
extern float pitch,roll,yaw; 										//欧拉角
extern short gyrox,gyroy,gyroz;									//陀螺仪原始数据
float Mechanical_balance = 0;
__IO PID_Typedef velocity_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef balance_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef turn_PID = {0,0,0,0,0,0,0,0};

float velocity_Control(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder)
{
	float err_least;
	err_least = PIDStructure->target_val - (leftencoder+rightencoder);
	PIDStructure->err *= 0.85;
	PIDStructure->err += 0.15 * err_least;//去除部分高频，实现低通滤波
	PIDStructure->integral += PIDStructure->err;
	PIDStructure->integral = PIDStructure->integral>10000?10000:PIDStructure->integral<-10000?-10000:PIDStructure->integral;//积分限幅
	PIDStructure->output_val = PIDStructure->Kp * PIDStructure->err + PIDStructure->Ki * PIDStructure->integral + PIDStructure->Kd * (PIDStructure->err - PIDStructure->err_last);
	PIDStructure->err_last = PIDStructure->err;
	if(pitch<-40||pitch>40) 			
		PIDStructure->integral=0;
	return PIDStructure->output_val;
}
/**************************************************************************
函数功能：直立PD控制
入口参数：角度、机械平衡角度（机械中值）、角速度
返回  值：直立控制PWM
**************************************************************************/
float balance_Control(__IO PID_Typedef *PIDStructure,float Angle,float Gyro)
{  
	float real_Angle;
	real_Angle=Angle-Mechanical_balance-PIDStructure->target_val;
	PIDStructure->output_val=PIDStructure->Kp * real_Angle - PIDStructure->Kd*Gyro;  //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return PIDStructure->output_val;
}
/**************************************************************************
函数功能：转向PD控制
入口参数：电机编码器的值、Z轴角速度
返回  值：转向控制PWM
**************************************************************************/
/*int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=44,Kp=20,Kd=0;     
	  //=============遥控左右旋转部分=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left+encoder_right);      
			Turn_Convert=55/Encoder_temp;
			if(Turn_Convert<0.6)Turn_Convert=0.6;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}			
		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;
		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 
		else Turn_Target=0;
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向	速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}*/
/*********************
转向环：系数*Z轴角速度+系数*遥控数据
*********************/
float turn_Control(__IO PID_Typedef *PIDStructure,int gyro_Z)
{
	//这不是一个严格的PD控制器，Kd针对的是转向的约束，但Kp针对的是遥控的转向。
	PIDStructure->output_val = PIDStructure->Kp *  PIDStructure->target_val + PIDStructure->Kd*gyro_Z ;
	return PIDStructure->output_val;
}
