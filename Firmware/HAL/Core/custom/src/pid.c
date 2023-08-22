#include "pid.h"
extern float pitch,roll,yaw; 										//ŷ����
extern short gyrox,gyroy,gyroz;									//������ԭʼ����
float Mechanical_balance = 0;
__IO PID_Typedef velocity_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef balance_PID = {0,0,0,0,0,0,0,0};
__IO PID_Typedef turn_PID = {0,0,0,0,0,0,0,0};

float velocity_Control(__IO PID_Typedef *PIDStructure,float leftencoder,float rightencoder)
{
	float err_least;
	err_least = PIDStructure->target_val - (leftencoder+rightencoder);
	PIDStructure->err *= 0.85;
	PIDStructure->err += 0.15 * err_least;//ȥ�����ָ�Ƶ��ʵ�ֵ�ͨ�˲�
	PIDStructure->integral += PIDStructure->err;
	PIDStructure->integral = PIDStructure->integral>10000?10000:PIDStructure->integral<-10000?-10000:PIDStructure->integral;//�����޷�
	PIDStructure->output_val = PIDStructure->Kp * PIDStructure->err + PIDStructure->Ki * PIDStructure->integral + PIDStructure->Kd * (PIDStructure->err - PIDStructure->err_last);
	PIDStructure->err_last = PIDStructure->err;
	if(pitch<-40||pitch>40) 			
		PIDStructure->integral=0;
	return PIDStructure->output_val;
}
/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ���еƽ��Ƕȣ���е��ֵ�������ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
float balance_Control(__IO PID_Typedef *PIDStructure,float Angle,float Gyro)
{  
	float real_Angle;
	real_Angle=Angle-Mechanical_balance-PIDStructure->target_val;
	PIDStructure->output_val=PIDStructure->Kp * real_Angle - PIDStructure->Kd*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return PIDStructure->output_val;
}
/**************************************************************************
�������ܣ�ת��PD����
��ڲ����������������ֵ��Z����ٶ�
����  ֵ��ת�����PWM
**************************************************************************/
/*int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=44,Kp=20,Kd=0;     
	  //=============ң��������ת����=======================//
  	if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
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
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת��	�ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}*/
/*********************
ת�򻷣�ϵ��*Z����ٶ�+ϵ��*ң������
*********************/
float turn_Control(__IO PID_Typedef *PIDStructure,int gyro_Z)
{
	//�ⲻ��һ���ϸ��PD��������Kd��Ե���ת���Լ������Kp��Ե���ң�ص�ת��
	PIDStructure->output_val = PIDStructure->Kp *  PIDStructure->target_val + PIDStructure->Kd*gyro_Z ;
	return PIDStructure->output_val;
}
