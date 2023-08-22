/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "sys.h"
#include "timer.h"
#include "bsp_usart.h"
#include "encoder.h"
#include "pwm.h"
#include "blue_teeth.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "ultra.h"
extern float pitch,roll,yaw; 										//欧拉角
extern short gyrox,gyroy,gyroz;									//陀螺仪原始数据
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
	{
		int x,count;
		//count = BlueTeethProcess();
		if(BLUE_RXCONPLETED == 1)
		{
			for(x=0;x<count-1;x++)
			Printf(USART1,"%c",TDY_data[x]);
			BLUE_RXCONPLETED = 0;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)!=RESET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
}
/*
在MPU6050的采样频率设置中，设置成100HZ，即可保证6050的数据是10ms更新一次。
				 读者可在imv_mpu.h文件第26行的宏定义进行修改(#define DEFAULT_MPU_HZ  (100))
*/
void MPU6050_EXIT_Handler(void) 
{
	if(MPU6050_INT == 0)
	{
		static short Gyroy_error = 0;
		static float Mechanical_angle = 0;
		static _Bool PIDONCEFLAG = 1;
		static uint8_t ultra_delay = 0;
			if(PIDONCEFLAG)
			{
				/*velocity_PID.Kp = 75.0;
				velocity_PID.Ki = 3.5;
				velocity_PID.target_val = 0;
				velocity_PID.output_val=0;
				balance_PID.Kp = 300;
				balance_PID.Kd = 0.3;
				balance_PID.output_val=0;*/
				
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
				mpu_dmp_get_data(&pitch,&roll,&yaw);
				//Gyroy_error = gyrox;
				//Mechanical_angle = pitch;
				PIDONCEFLAG = 0;
			}
			//4左3右
		int right_pwm=0,left_pwm=0,rightencoder,leftencoder;
		leftencoder = Read_Encoder(4);  
		rightencoder = Read_Encoder(3);
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		gyroy -= Gyroy_error;
		pitch -= Mechanical_angle;
		//PID_Process(&velocity_PID,leftencoder,rightencoder);
		//balance_UP(&balance_PID,pitch,gyroy);
		//left_pwm = right_pwm = -velocity_PID.output_val + balance_PID.output_val;
		//printf("%d\n%d\n",leftencoder,rightencoder);
		//printf("%d\n",gyroy);
		//printf("%f\n",pitch);
		//printf("A=%d\r\nB=%d\r\nC=%d\r\nD=%d\r\n",leftencoder,left_pwm,rightencoder,right_pwm);
		//if((ultra_delay+=1)>5)
		//{
		//	DMA_Printf(USART1,"%fcm\n",UltraGetDistance());
		//	ultra_delay = 0;
		//}
			
		if(pitch<-35||pitch>35)
			left_pwm = right_pwm = 0;
		//Set_Pwm(left_pwm,right_pwm);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);//记得改中断清除！！！
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		Ultra_EXIT_Service();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
void TIM2_IRQHandler(void)
{//printf("ERROR\n");
		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//TIM_IT_Update
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
void TIM3_IRQHandler(void)
{//printf("ERROR\n");
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//TIM_IT_Update
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void TIM4_IRQHandler(void)
{
	
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//TIM_IT_Update
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM6_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//TIM_IT_Update
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
