#ifndef __PWM_H_
#define __PWM_H_
#include "stm32f1xx_hal.h"
#include "sys.h"
#include "tim.h"
//============TIM1 ��·PWM===========
#define      PWM_TIM 				htim1
#define 			PWM_Preload  		__HAL_TIM_GET_AUTORELOAD(&PWM_TIM)

//==============TB6612�������Ŷ���============

#define AIN1   PBout(12)
#define AIN2   PBout(13)
#define BIN1   PBout(14)
#define BIN2   PBout(15)


#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11



void Motor_Init(void);
void Set_Pwm(int moto1,int moto2);
#endif
