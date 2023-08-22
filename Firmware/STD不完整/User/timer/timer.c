#include "timer.h"
#include "bsp_usart.h"
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)


void Timer_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置TIM2_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
static void Timer_GPIO_Configuration(void)
{
	
}
static void Timer_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//============TIM2===========
	TIM2_APBxClock_CMD(TIM2_RCC_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM2_Period ;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Prescaler;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_ARRPreloadConfig(TIM2,ENABLE);
   
	TIM_SetCounter(TIM2,0);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 开启更新中断  
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除更新和中断标志位
	TIM_Cmd(TIM2, DISABLE);
}
void Timer_Init(void)
{
	Timer_GPIO_Configuration();
	Timer_Config();
}
/*
函数描述：TIM2通道一获取按键时间间隔。
变量描述：mode      
					|mode = 0;  us输出
					|mode = 1;  ms输出
					|mode = 2;  s输出

*/
float GetCaptureIntervalTime(uint8_t mode)
{
	float time;
	switch(mode)
	{
		case 0:
		{
			time = (float)TIM2->CNT;
			break;
		}
		case 1:
		{
			time = (float)TIM2->CNT/1000;
			break;
		}
		case 2:
		{
			time = (float)TIM2->CNT/1000000;
			break;
		}
		default: time = 0;
	}return time;
}
