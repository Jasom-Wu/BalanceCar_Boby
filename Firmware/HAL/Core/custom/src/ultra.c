#include "ultra.h"
#include "delay.h"
#include "usart.h"
#include "tim.h"
__IO _Bool ULTRA_FLAG = 0;
static float GetCaptureIntervalTime(uint8_t mode);
static uint16_t TIM_Counter =0u;
float UltraGetDistance(void)
{
	float time=0,distance=0;
	ULTRA_TRIG = 1;
	delay_us(10);
	ULTRA_TRIG = 0;
	HAL_TIM_Base_Stop(&ULTRA_TIM);
	time = GetCaptureIntervalTime(1);
	distance = time * 170;
	distance /= 10;
	return distance;
}
/*
函数描述：TIM2通道一获取按键时间间隔。
变量描述：mode      
					|mode = 0;  us输出
					|mode = 1;  ms输出
					|mode = 2;  s输出

*/
static float GetCaptureIntervalTime(uint8_t mode)
{
	float time;
	switch(mode)
	{
		case 0:
		{
			time = (float)TIM_Counter;
			break;
		}
		case 1:
		{
			time = (float)TIM_Counter/1000;
			break;
		}
		case 2:
		{
			time = (float)TIM_Counter/1000000;
			break;
		}
		default: time = 0;
	}return time;
}
void Ultra_EXIT_Service(void)
{
	if(ULTRA_ECHO == 1)
		{										
				HAL_TIM_Base_Start(&ULTRA_TIM);      //开启时钟
				__HAL_TIM_SET_COUNTER(&ULTRA_TIM,0);
				while(ULTRA_ECHO != 0)               //等待低电平
				{
					if(__HAL_TIM_GetCounter(&ULTRA_TIM)>=10000)
					{
						break;
					}	
				}
				//HAL_TIM_Base_Stop(&ULTRA_TIM);		   //定时器2失能
				TIM_Counter = (uint16_t)__HAL_TIM_GetCounter(&ULTRA_TIM);
		}
}

