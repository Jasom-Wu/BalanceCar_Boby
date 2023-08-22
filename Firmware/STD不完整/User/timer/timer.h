#ifndef __TIMER_H_
#define __TIMER_H_
#include "stm32f10x.h"
// ====================TIM2=====================
#define      TIM2_APBxClock_CMD     RCC_APB1PeriphClockCmd
#define      TIM2_RCC_CLK           RCC_APB1Periph_TIM2
#define      TIM2_Period            (65536-1)
#define      TIM2_Prescaler         (72-1)

#endif
void Timer_Init(void);
float GetCaptureIntervalTime(uint8_t mode);
void TIM2_IT_UltraIntervalCapture(void);
void Timer_NVIC_Configuration(void);
