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
����������TIM2ͨ��һ��ȡ����ʱ������
����������mode      
					|mode = 0;  us���
					|mode = 1;  ms���
					|mode = 2;  s���

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
				HAL_TIM_Base_Start(&ULTRA_TIM);      //����ʱ��
				__HAL_TIM_SET_COUNTER(&ULTRA_TIM,0);
				while(ULTRA_ECHO != 0)               //�ȴ��͵�ƽ
				{
					if(__HAL_TIM_GetCounter(&ULTRA_TIM)>=10000)
					{
						break;
					}	
				}
				//HAL_TIM_Base_Stop(&ULTRA_TIM);		   //��ʱ��2ʧ��
				TIM_Counter = (uint16_t)__HAL_TIM_GetCounter(&ULTRA_TIM);
		}
}

