#ifndef __DELAY_H_
#define __DELAY_H_ 	
#include "stm32f1xx_hal.h"
#include "main.h"

#define INNER_TIM htim2
void delay_us(uint32_t nus);
void delay_ms(uint32_t ms);
void delay_s(uint32_t s);
#endif
