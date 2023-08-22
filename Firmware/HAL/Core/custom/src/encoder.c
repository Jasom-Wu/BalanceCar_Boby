#include "encoder.h"
#include "oled.h"
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

int Read_Encoder(int8_t TIMx)
{
	int Encoder_TIM;    
   switch(TIMx)
	 {
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;	TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT; 	TIM4 -> CNT=0;break;			 
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

float GetMotorSpeed(int8_t leftORright)
{
	int encoderNum = 0;
	float speed=0;
	if(leftORright)
	{
		encoderNum = Read_Encoder(3);
		speed = (float)encoderNum/TOTAL_RESOLUTION *10;
	}
	else
	{
		encoderNum = Read_Encoder(4);
		speed = (float)encoderNum/TOTAL_RESOLUTION *10;
	}
	return speed;
}

void IT_TIM3_4EncoderService(void)
{
		/*float rightspeed,leftspeed;
		rightspeed = GetMotorSpeed(0);
		printf("右轮转速：%.3f\n",rightspeed);
		if(rightspeed>=0)
		OLED_ShowStr(0,2,1,"RIGHT_SPEED: %.2f",rightspeed);
		else
		OLED_ShowStr(0,2,1,"RIGHT_SPEED:%.2f",rightspeed);
		leftspeed = GetMotorSpeed(1);
		printf("左轮转速：%.3f\n",leftspeed);
		if(leftspeed>=0)
		OLED_ShowStr(0,3,1,"LEFT_SPEED: %.2f",leftspeed);
		else
		OLED_ShowStr(0,3,1,"LEFT_SPEED:%.2f",leftspeed);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);*/
	
}
