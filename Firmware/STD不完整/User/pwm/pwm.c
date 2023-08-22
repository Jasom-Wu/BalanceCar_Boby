#include "pwm.h"
//PWM输出初始化
//使用TIM1第一、第四通道。
static void TIM1_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(TIM1_CH1_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin = TIM1_CH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出(要复用！！)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIM1_CH1_PORT, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(TIM1_CH4_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin = TIM1_CH4_PIN;
	GPIO_Init(TIM1_CH4_PORT, &GPIO_InitStructure);
}
static void TIM1_PWM_Init(void)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM1_APBxClock_CMD(TIM1_RCC_CLK, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = TIM1_Period; 
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_Prescaler; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_SetCounter(TIM1,0);
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}

static void TB6612_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  MOTOR_APBxClock_CMD(MOTOR_GPIO_RCC_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_PIN1|MOTOR_GPIO_PIN2|MOTOR_GPIO_PIN3|MOTOR_GPIO_PIN4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);
	AIN1=0,AIN2=0;
	BIN1=0,BIN2=0;
}

void Motor_Init(void)
{
	TIM1_GPIO_Configuration();
	TIM1_PWM_Init();
	TB6612_Init();
}

void Set_Pwm(int moto1,int moto2)
{
	moto1 = -moto1;
	if(moto1<0)
	{AIN2=1;AIN1=0;moto1=-moto1;}
	else if(moto1>0)
	{AIN2=0;AIN1=1;}
	else 
	{AIN2=0;AIN1=0;}
	
	if(moto2<0)
	{BIN2=1;BIN1=0;moto2=-moto2;}
	else if(moto2>0)
	{BIN2=0;BIN1=1;}
	else 
	{BIN2=0;BIN1=0;}
	
	if(moto1 > TIM1_Period-2500)
		moto1 = TIM1_Period-2500;
	if(moto2 > TIM1_Period-2500)
		moto2 = TIM1_Period-2500;
	PWMA = moto1;
	PWMB = moto2;
}
