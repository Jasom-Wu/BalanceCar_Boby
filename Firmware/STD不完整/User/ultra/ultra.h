#ifndef __ULTRA_H_
#define __ULTRA_H_	
#include "stm32f10x.h"
#include "sys.h"
#define     ULTRA_TRIG_GPIO_CLK      RCC_APB2Periph_GPIOA
#define     ULTRA_TRIG_GPIO_PORT     GPIOA
#define     ULTRA_TRIG_GPIO_PIN      GPIO_Pin_12

#define            ULTRA_ECHO_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ULTRA_ECHO_GPIO_PORT     GPIOA
#define            ULTRA_ECHO_GPIO_PIN      GPIO_Pin_15

#define 	  ULTRA_TRIG          PAout(12)
#define			ULTRA_ECHO					PAin(15)
void Ultra_EXIT_Service(void);
float UltraGetDistance(void);
void UltraConfig(void);
void Ultra_EXTI_NVIC_Config(void);
#endif
