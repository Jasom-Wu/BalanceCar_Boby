#include "timer.h"
#include "bsp_usart.h"
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)


void Timer_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����TIM2_IRQ�ж�Ϊ�ж�Դ */
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
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // ���������ж�  
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// ������º��жϱ�־λ
	TIM_Cmd(TIM2, DISABLE);
}
void Timer_Init(void)
{
	Timer_GPIO_Configuration();
	Timer_Config();
}
/*
����������TIM2ͨ��һ��ȡ����ʱ������
����������mode      
					|mode = 0;  us���
					|mode = 1;  ms���
					|mode = 2;  s���

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
