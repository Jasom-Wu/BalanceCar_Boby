#include "delay.h"
#include "tim.h"
void delay_us(uint32_t us)
{
	HAL_TIM_Base_Start(&INNER_TIM);
	__HAL_TIM_SetCounter(&INNER_TIM,0);// 先把计数清零
	while( us > __HAL_TIM_GetCounter(&INNER_TIM) );
	HAL_TIM_Base_Stop(&INNER_TIM);
}
void delay_ms(uint32_t ms)
{
	__HAL_TIM_SetCounter(&INNER_TIM,0);// 先把计数清零
	HAL_TIM_Base_Start(&INNER_TIM);
	//__HAL_TIM_ENABLE(&INNER_TIM);
	for(uint32_t x=0;x<ms;x++)
	{
		while( 1000 > __HAL_TIM_GetCounter(&INNER_TIM) );
		__HAL_TIM_SetCounter(&INNER_TIM,0);
	}	
	HAL_TIM_Base_Stop(&INNER_TIM);
	//__HAL_TIM_DISABLE(&INNER_TIM);
}
void delay_s(uint32_t s)
{
	HAL_TIM_Base_Start(&INNER_TIM);
	__HAL_TIM_SetCounter(&INNER_TIM,0);// 先把计数清零
	for(uint32_t x=0;x<s;x++)
	for(uint32_t i=0;i<20;i++)
	{
		while( 50000 > __HAL_TIM_GetCounter(&INNER_TIM) );
		__HAL_TIM_SetCounter(&INNER_TIM,0);
	}
	HAL_TIM_Base_Stop(&INNER_TIM);
}
