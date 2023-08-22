#ifndef __ULTRA_H_
#define __ULTRA_H_	
#include "stm32f1xx_hal.h"
#include "sys.h"
#define		 ULTRA_TIM 							 htim2
#define     ULTRA_TRIG_GPIO_PORT     GPIOA
#define     ULTRA_TRIG_GPIO_PIN      GPIO_PIN_12

#define            ULTRA_ECHO_GPIO_PORT     GPIOA
#define            ULTRA_ECHO_GPIO_PIN      GPIO_PIN_15

#define 	 	ULTRA_TRIG      PAout(12)
#define			ULTRA_ECHO			 PAin(15)

extern __IO _Bool ULTRA_FLAG;
void Ultra_EXIT_Service(void);
float UltraGetDistance(void);
#endif
