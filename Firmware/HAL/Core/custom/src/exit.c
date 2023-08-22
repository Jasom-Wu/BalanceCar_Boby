#include "exit.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "usart.h"
#include "ultra.h"
#include "pid.h"
#include "encoder.h"
#include "pwm.h"
#include "stdlib.h"
#include "blue_teeth.h"
#include "delay.h"
float pitch,roll,yaw; 		//欧拉角
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
int left_encode=0,right_encode=0,right_pwm=0,left_pwm=0;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(ULTRA_FLAG == 1)
		Ultra_EXIT_Service();
	if(HAL_GPIO_ReadPin(MPU6050_INT_GPIO_Port, MPU6050_INT_Pin)==GPIO_PIN_RESET && INT_FLAG==1)
	{

		static float Pitch_last = 0;
		static _Bool PIDONCEFLAG = 1;
		static uint8_t ultra_delay = 0;
			if(PIDONCEFLAG)
			{
				delay_s(6);// 6秒钟等待pitch解算完成
				__HAL_TIM_SetCounter(&htim3,0);//这里一点要清定时器，否则编码器读出来的值会变得很大导致错误
				__HAL_TIM_SetCounter(&htim4,0);
				while(abs(pitch-Pitch_last)>0.5||pitch==0)
				{
					Pitch_last = pitch;
					mpu_dmp_get_data(&pitch,&roll,&yaw);
				}
				PIDONCEFLAG = 0;
			}
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			
		left_encode = Read_Encoder(3);
		right_encode = Read_Encoder(4);
		
			if(abs(pitch)<40)
			{
				//前进速度控制
				if(User_Data.VelocityVal * velocity_PID.target_val < 0)
					velocity_PID.integral = 0;
				if(abs(User_Data.VelocityVal)<15&&abs(User_Data.VelocityVal)>2)
					velocity_PID.target_val += User_Data.VelocityVal>0?1:-1;
				else if(abs(User_Data.VelocityVal)>=30)
					velocity_PID.target_val += User_Data.VelocityVal>0?2:-2;
				else
					velocity_PID.target_val=0;
				velocity_PID.target_val = velocity_PID.target_val>25?25:velocity_PID.target_val<-25?-25:velocity_PID.target_val;//速度限幅
				
				// 转向控制
				if(abs(User_Data.TurnVal)<15&&abs(User_Data.TurnVal)>2)
					turn_PID.target_val += User_Data.TurnVal>0?10:-10;
				else if(abs(User_Data.TurnVal)>=30)
					turn_PID.target_val += User_Data.TurnVal>0?20:-20;
				else
					turn_PID.target_val=0;
				turn_PID.target_val = turn_PID.target_val>100?100:turn_PID.target_val<-100?-100:turn_PID.target_val;//速度限幅
			}
			
		velocity_Control(&velocity_PID,(float)left_encode,(float)right_encode);
		balance_PID.target_val = velocity_PID.output_val;
		balance_Control(&balance_PID,pitch,gyroy);
		turn_Control(&turn_PID,gyroz);
		left_pwm = right_pwm =	balance_PID.output_val;
		left_pwm += turn_PID.output_val;
		right_pwm -= turn_PID.output_val;
		/*if((ultra_delay+=1)>50)
		{
			float distance = UltraGetDistance();
			ultra_delay = 0;
			if(distance <15)
				velocity_PID.target_val = -25;
			else
				velocity_PID.target_val = 0;
		}*/
		if(pitch<-40||pitch>40)
			left_pwm = right_pwm = 0;
		Set_Pwm(left_pwm,right_pwm);
	}
	
}

