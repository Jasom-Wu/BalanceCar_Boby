#include "nvic_config.h"
#include "mpu6050.h"
#include "bsp_usart.h"
#include "timer.h"
#include "ultra.h"
void NVIC_Config(void)
{
	USART_NVIC_Configuration();//2.0 2.0
	Timer_NVIC_Configuration();//1.0
	Ultra_EXTI_NVIC_Config();//1.1
	MPU6050_EXTI_NVIC_Config();//2.1
}
