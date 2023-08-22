#include "encoder.h"
#include "timer.h"
#include "oled.h"
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)


static void Encoder_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* ����TIM4_IRQ�ж�Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
static void Encoder_GPIO_Configuration(void)
{ 
	//============TIM3������1===========
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(TIM3_RCC_CLK, ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
  RCC_APB2PeriphClockCmd(TIM3_CH1_GPIO_CLK, ENABLE);//ʹ��PC�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PIN|TIM3_CH2_PIN;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOC
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	//============TIM4������2===========
	RCC_APB1PeriphClockCmd(TIM4_RCC_CLK, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(TIM4_CH1_GPIO_CLK, ENABLE);//ʹ��PC�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = TIM4_CH1_PIN|TIM4_CH2_PIN;	//�˿�����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
static void Encoder_Config(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//============TIM3������1===========
	TIM3_APBxClock_CMD(TIM3_RCC_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period ;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_SetCounter(TIM3,0);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);	// ������ºͲ����жϱ�־λ
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // �������ºͲ����ж�  
	TIM_Cmd(TIM3, ENABLE);
	
	//============TIM4������2===========
	TIM4_APBxClock_CMD(TIM4_RCC_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period ;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_SetCounter(TIM4,0);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	// ������ºͲ����жϱ�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // �������ºͲ����ж�  
	TIM_Cmd(TIM4, ENABLE);
  
}
int Read_Encoder(int8_t TIMx)
{
	int Encoder_TIM;    
   switch(TIMx)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;	TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT; 	TIM4 -> CNT=0;break;
		 case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;			 
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
void Encoder_Init(void)
{
	Encoder_GPIO_Configuration();
	Encoder_NVIC_Configuration();
	Encoder_Config();
}

float GetMotorSpeed(int8_t TIMx)
{
	int EncoderNum = 0;
	float speed=0;
	switch(TIMx)
	{
		case 2:{EncoderNum = Read_Encoder(2);speed = (float)EncoderNum/TOTAL_RESOLUTION *10;}
		case 3:{EncoderNum = Read_Encoder(3);speed = (float)EncoderNum/TOTAL_RESOLUTION *10;}
		case 4:{EncoderNum = Read_Encoder(4);speed = (float)EncoderNum/TOTAL_RESOLUTION *10;}
		case 5:{EncoderNum = Read_Encoder(5);speed = (float)EncoderNum/TOTAL_RESOLUTION *10;}
		default: speed=0;
	}
	return speed;
}

void IT_TIM3_4EncoderService(void)
{
		/*float rightspeed,leftspeed;
		rightspeed = GetMotorSpeed(0);
		printf("����ת�٣�%.3f\n",rightspeed);
		if(rightspeed>=0)
		OLED_ShowStr(0,2,1,"RIGHT_SPEED: %.2f",rightspeed);
		else
		OLED_ShowStr(0,2,1,"RIGHT_SPEED:%.2f",rightspeed);
		leftspeed = GetMotorSpeed(1);
		printf("����ת�٣�%.3f\n",leftspeed);
		if(leftspeed>=0)
		OLED_ShowStr(0,3,1,"LEFT_SPEED: %.2f",leftspeed);
		else
		OLED_ShowStr(0,3,1,"LEFT_SPEED:%.2f",leftspeed);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);*/
	
}
