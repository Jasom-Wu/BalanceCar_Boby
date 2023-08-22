#include "ultra.h"
#include "timer.h"
#include "delay.h"
#include "bsp_usart.h"
static void Ultra_EXIT_Config(void);
static void Ultra_GPIO_Config(void);
void UltraConfig(void)
{
	Ultra_GPIO_Config();
	Ultra_EXIT_Config();
}
static void Ultra_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 初始化GPIO
	RCC_APB2PeriphClockCmd(ULTRA_TRIG_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = ULTRA_TRIG_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ULTRA_TRIG_GPIO_PORT, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(ULTRA_ECHO_GPIO_CLK,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = ULTRA_ECHO_GPIO_PIN;
	GPIO_Init(ULTRA_ECHO_GPIO_PORT, &GPIO_InitStructure);
}
void Ultra_EXTI_NVIC_Config(void)//加个static使得该函数只能在此文件中调用
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
static void Ultra_EXIT_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 初始化EXTI
	EXTI_InitTypeDef  EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

}
float UltraGetDistance(void)
{
	float time=0,distance=0;
	ULTRA_TRIG = 1;
	delay_us(15);
	ULTRA_TRIG = 0;
	
	time = GetCaptureIntervalTime(1);
	distance = time * 170;
	distance /= 10;
	return distance;
}
void Ultra_EXIT_Service(void)
{
	if(ULTRA_ECHO == 1)
		{	
				TIM_SetCounter(TIM2,0);																		
				TIM_Cmd(TIM2, ENABLE);               //开启时钟
				while(ULTRA_ECHO != 0)               //等待低电平
				{
					if(TIM2->CNT>60000)
					{
						Printf(USART1,"ERROR\n");
						break;
					}	
				}
				TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));				
				//TIM_Cmd(TIM2, DISABLE);			         //定时器3失能
		}
}

