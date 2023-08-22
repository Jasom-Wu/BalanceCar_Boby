#ifndef __PWM_H_
#define __PWM_H_
#include "stm32f10x.h"
#include "sys.h"
#include "pid.h"
//============TIM1 两路PWM===========
#define      TIM1_APBxClock_CMD     RCC_APB2PeriphClockCmd
#define      TIM1_RCC_CLK           RCC_APB2Periph_TIM1
#define      TIM1_Period            (uint16_t)10000
#define      TIM1_Prescaler         0

// TIM1 输出比较通道1
#define            TIM1_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            TIM1_CH1_PORT          GPIOA
#define            TIM1_CH1_PIN           GPIO_Pin_8

// TIM1 输出比较通道2
#define            TIM1_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            TIM1_CH2_PORT          GPIOA
#define            TIM1_CH2_PIN           GPIO_Pin_9

// TIM1 输出比较通道3
#define            TIM1_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            TIM1_CH3_PORT          GPIOA
#define            TIM1_CH3_PIN           GPIO_Pin_10

// TIM1 输出比较通道4
#define            TIM1_CH4_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            TIM1_CH4_PORT          GPIOA
#define            TIM1_CH4_PIN           GPIO_Pin_11

//==============TB6612驱动引脚定义============
#define      MOTOR_APBxClock_CMD     RCC_APB2PeriphClockCmd
#define      MOTOR_GPIO_RCC_CLK      RCC_APB2Periph_GPIOB
#define      MOTOR_GPIO_PORT     		 GPIOB
#define      MOTOR_GPIO_PIN1         GPIO_Pin_12
#define      MOTOR_GPIO_PIN2         GPIO_Pin_13
#define      MOTOR_GPIO_PIN3         GPIO_Pin_14
#define      MOTOR_GPIO_PIN4         GPIO_Pin_15

#define AIN1   PBout(12)
#define AIN2   PBout(13)
#define BIN1   PBout(14)
#define BIN2   PBout(15)


#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11



void Motor_Init(void);
void Set_Pwm(int moto1,int moto2);
#endif
