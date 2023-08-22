#include "pwm.h"
#include "stdlib.h"
short Death_Voltage = 210;//电机死区补偿
//PWM输出初始化
//使用TIM1第一、第四通道。

void Motor_Init(void)
{
	AIN1=0,AIN2=0;
	BIN1=0,BIN2=0;
}

void Set_Pwm(int moto1,int moto2)
{
	if(abs(moto1)<100)
		moto1 = 0;
	if(abs(moto2)<100)
		moto2 = 0;
	moto2 = -moto2;
	if(moto1<0)
	{AIN2=1;AIN1=0;moto1=-moto1;}
	else if(moto1>0)
	{AIN2=0;AIN1=1;}
	else //if(abs(moto1<100))
	{AIN2=0;AIN1=0;moto1=0;}
	
	if(moto2<0)
	{BIN2=1;BIN1=0;moto2=-moto2;}
	else if(moto2>0)
	{BIN2=0;BIN1=1;}
	else //if(abs(moto2<100))
	{BIN2=0;BIN1=0;moto2=0;}
	
	if(moto1 > PWM_Preload-1000)
		moto1 = PWM_Preload-1000;
	if(moto2 > PWM_Preload-1000)
		moto2 = PWM_Preload-1000;
	
	PWMA = moto1+Death_Voltage;
	PWMB = moto2+Death_Voltage;
}

