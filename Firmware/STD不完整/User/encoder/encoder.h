#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "stm32f10x.h"
#define ENCODER_RESOLUTION 13
#define ENCODER_MULTIPLE 4
#define MOTOR_REDUXTION_RATIO 30
#define TOTAL_RESOLUTION (ENCODER_RESOLUTION * ENCODER_MULTIPLE * MOTOR_REDUXTION_RATIO)
//============TIM3编码器1===========
#define      TIM3_APBxClock_CMD     RCC_APB1PeriphClockCmd
#define      TIM3_RCC_CLK           RCC_APB1Periph_TIM3
#define      TIM3_Period            (uint16_t)65535
#define      TIM3_Prescaler         0
//端口AFIO
// TIM3输出比较通道1
#define            TIM3_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM3_CH1_PORT          GPIOB
#define            TIM3_CH1_PIN           GPIO_Pin_4

// TIM3 输出比较通道2
#define            TIM3_CH2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM3_CH2_PORT          GPIOB
#define            TIM3_CH2_PIN           GPIO_Pin_5

// TIM3 输出比较通道3
#define            TIM3_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM3_CH3_PORT          GPIOB
#define            TIM3_CH3_PIN           GPIO_Pin_0

// TIM3 输出比较通道4
#define            TIM3_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM3_CH4_PORT          GPIOB
#define            TIM3_CH4_PIN           GPIO_Pin_1




//============TIM4编码器2===========
#define      TIM4_APBxClock_CMD     RCC_APB1PeriphClockCmd
#define      TIM4_RCC_CLK           RCC_APB1Periph_TIM4
#define      TIM4_Period            (uint16_t)65535
#define      TIM4_Prescaler         0

// TIM4输出比较通道1
#define            TIM4_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM4_CH1_PORT          GPIOB
#define            TIM4_CH1_PIN           GPIO_Pin_6

// TIM4 输出比较通道2
#define            TIM4_CH2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM4_CH2_PORT          GPIOB
#define            TIM4_CH2_PIN           GPIO_Pin_7

// TIM4 输出比较通道3
#define            TIM4_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM4_CH3_PORT          GPIOB
#define            TIM4_CH3_PIN           GPIO_Pin_8

// TIM4 输出比较通道4
#define            TIM4_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            TIM4_CH4_PORT          GPIOB
#define            TIM4_CH4_PIN           GPIO_Pin_9

void Encoder_Init(void);
int Read_Encoder(int8_t TIMx);
float GetMotorSpeed(int8_t leftORright);
void IT_TIM3_4EncoderService(void);
#endif
