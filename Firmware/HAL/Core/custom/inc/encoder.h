#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "stm32f1xx_hal.h"
#include "tim.h"

#define ENCODER_RESOLUTION 13
#define ENCODER_MULTIPLE 4
#define MOTOR_REDUXTION_RATIO 30
#define TOTAL_RESOLUTION (ENCODER_RESOLUTION * ENCODER_MULTIPLE * MOTOR_REDUXTION_RATIO)

int Read_Encoder(int8_t TIMx);
float GetMotorSpeed(int8_t leftORright);
void IT_TIM3_4EncoderService(void);
#endif
