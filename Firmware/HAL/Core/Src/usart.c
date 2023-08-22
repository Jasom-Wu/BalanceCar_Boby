/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "stdarg.h"
#include "blue_teeth.h"
 

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
//USART1
char RxBuffer1[RXBUFFERSIZE];   //接收数据
char TxBuffer1[TXBUFFERSIZE];		//发送数据
__IO uint16_t Rec_Length1 = 0;
__IO _Bool Rec_Complete1 = 0;
__IO uint8_t aRxBuffer1;				//接收中断缓冲
uint8_t Uart1_Rx_Cnt = 0;			//接收缓冲计数
//USART2
char RxBuffer2[RXBUFFERSIZE];   //接收数据
char TxBuffer2[TXBUFFERSIZE];		//发送数据
__IO uint16_t Rec_Length2 = 0;
__IO _Bool Rec_Complete2 = 0;
__IO uint8_t aRxBuffer2;				//接收中断缓冲
uint8_t Uart2_Rx_Cnt = 0;			//接收缓冲计数

_Bool Uart_Error_Flag =0;
__IO uint8_t USART_FLAG = 1;//1表示串口1输出输入模式，2表示串口2输出输入模式，依此推类。
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	//DMA接收函数，此句一定要加，不加接收不到第一次传进来的实数据，是空的，且此时接收到的数据长度为缓存器的数据长度
	HAL_UART_Receive_DMA(&huart1,(uint8_t *)RxBuffer1,RXBUFFERSIZE);
	while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	}
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //使能IDLE中断

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
	//DMA接收函数，此句一定要加，不加接收不到第一次传进来的实数据，是空的，且此时接收到的数据长度为缓存器的数据长度
	HAL_UART_Receive_DMA(&huart2,(uint8_t *)RxBuffer2,RXBUFFERSIZE);
	
	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	}
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); //使能IDLE中断
  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Channel7;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
int Printf(USART_TypeDef *USARTx,char *formatString,...)
{
	char TXBuffer[TX_DATA_SIZE];
	int x;int length;
	va_list args;
	va_start(args,formatString);
	length = vsnprintf(TXBuffer,TX_DATA_SIZE,formatString,args);
	if(USARTx == USART1)
			USART_FLAG = 1;
	else if (USARTx == USART2)
			USART_FLAG = 2;
	va_end(args);
	for(x=0;x<length;x++)
	printf("%c",TXBuffer[x]);
	return length;
}
char Getchar(USART_TypeDef *USARTx)
{
	char temp;
	if(USARTx == USART1)
		{
			USART_FLAG = 1;
			temp = getchar();
		}
	else if (USARTx == USART2)
		{
			USART_FLAG = 2;
			temp = getchar();
		}
		return temp;
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		if(USART_FLAG ==1)
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
			while(!(USART1->SR & UART_FLAG_TXE));
		}
		
		else if (USART_FLAG ==2)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
			while(!(USART1->SR & UART_FLAG_TXE));
		}
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
	uint8_t temp;
	if(USART_FLAG ==1)
	{
		HAL_UART_Receive(&huart1, &temp, 1, 0xffff);
	}
		
	else if (USART_FLAG ==2)
	{
		HAL_UART_Receive(&huart2, &temp, 1, 0xffff);
	}
	return temp;
}
 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		;
	}
	else if(huart == &huart2)
	{
		;
	}
	
}

 /**************************************************************************
  * @brief  串口错误回调函数
  * @retval 
**************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_PE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00000", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_PEFLAG(huart);
    }
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_FE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00001", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_FEFLAG(huart);
    }
    
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_NE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00002", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_NEFLAG(huart);
    }       
    
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00003", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_OREFLAG(huart);
    }    
 
 // 错误标志位置 
 Uart_Error_Flag = 1;
}

uint16_t DMA_Printf(USART_TypeDef *USARTx,char *formatString,...)
{
	char TXBuffer[TX_DATA_SIZE];
	uint16_t length;
	va_list args;
	va_start(args,formatString);
	length = vsnprintf(TXBuffer,TX_DATA_SIZE,formatString,args);
	if(USARTx == USART1)
	{	
		while(HAL_DMA_GetState(&hdma_usart1_tx)==HAL_DMA_STATE_BUSY);
		if(huart1.gState != HAL_UART_STATE_READY)HAL_UART_Abort(&huart1);
		__HAL_DMA_DISABLE(&hdma_usart1_tx);
		if(HAL_UART_Transmit_DMA(&huart1,(uint8_t *)TXBuffer,length)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
		{
			//return;
		}
		while(__HAL_DMA_GET_COUNTER(&hdma_usart1_tx)!=0);//等发送完才能清
	}
	else if (USARTx == USART2)
	{
		while(HAL_DMA_GetState(&hdma_usart2_tx)==HAL_DMA_STATE_BUSY);
		if(huart2.gState != HAL_UART_STATE_READY)HAL_UART_Abort(&huart2);
		__HAL_DMA_DISABLE(&hdma_usart2_tx);
		if(HAL_UART_Transmit_DMA(&huart2,(uint8_t *)TXBuffer,length)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
		{
			//return;
		}
		while(__HAL_DMA_GET_COUNTER(&hdma_usart2_tx)!=0);//等发送完才能清
	}
	va_end(args);
	return length;
}
void USART_DMARec_Stop(UART_HandleTypeDef *huart)
{
	uint32_t dmarequest = 0x00U;
  /* Stop UART DMA Rx request if ongoing */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
  {
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* Abort the UART DMA Rx channel */
    if (huart->hdmarx != NULL)
    {
      HAL_DMA_Abort(huart->hdmarx);
    }
			/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
		CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
		CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

		/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
		if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
		{
			CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
		}

		/* At end of Rx process, restore huart->RxState to Ready */
		huart->RxState = HAL_UART_STATE_READY;
		huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
  }
}
#define USART_IDLE_FLAG	(hdma_usart1_tx.State!=HAL_DMA_STATE_BUSY && hdma_usart2_tx.State!=HAL_DMA_STATE_BUSY)
#define USART_IDLE_OK 1
void USART1_IDLE_Handler(void)
{
	uint16_t temp;
	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)
	{
		if(hdma_usart1_tx.State!=HAL_DMA_STATE_BUSY&&Rec_Complete1 == 0)
			//检测DMA发送是否完成，不加这句的话，当数据流进空闲中断太频繁，会导致DMA提前结束以致发送中止卡住
		{ 
			USART_DMARec_Stop(&huart1); 
			temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// 获取DMA中未传输的数据个数   //temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
			Rec_Length1 =  RXBUFFERSIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
			Rec_Complete1 = 1;	// 接受完成标志位置1	
			
		 }
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位要放出来！！不然无法清除标志位导致一直在中断handler死循环
	}
}
void USART2_IDLE_Handler(void)
{
	uint16_t temp;
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET)
	{
		if(hdma_usart2_tx.State!=HAL_DMA_STATE_BUSY&&Rec_Complete2 == 0)//idle标志被置位
			//检测DMA发送是否完成，不加这句的话，当数据流进空闲中断太频繁，会导致DMA提前结束以致发送中止卡住
		{ 
			USART_DMARec_Stop(&huart2); //__HAL_DMA_DISABLE(huart2->hdmarx);
			temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中未传输的数据个数   //temp  = hdma_usart2_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
			Rec_Length2 =  RXBUFFERSIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
			Rec_Complete2 = 1;	// 接受完成标志位置1	
			
		 }
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);//清除标志位要放出来！！不然无法清除标志位导致一直在中断handler死循环
	}
	
}
void USART1_REC_Handler(void)
{
		 if(Rec_Complete1 == 1)  //接收完成标志
	{
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
		//======USART1_LOGIC_BEGIN======
		if(HAL_UART_Transmit_DMA(&huart1,(uint8_t *)RxBuffer1,Rec_Length1)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
		{
		 //return;
		}
		while(__HAL_DMA_GET_COUNTER(&hdma_usart1_tx)!=0);//等发送完才能清
		//======USART1_LOGIC_END======
		memset(RxBuffer1,0,Rec_Length1);
		Rec_Length1 = 0;//清除计数
		Rec_Complete1 = 0;//清除接收结束标志位
		HAL_UART_Receive_DMA(&huart1,(uint8_t *)RxBuffer1,RXBUFFERSIZE);//重新打开DMA接收
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	}
}
void USART2_REC_Handler(void)
{
	if(Rec_Complete2 == 1)  //接收完成标志
	{
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
		//======USART2_LOGIC_BEGIN======
		BLUE_Process();
		//======USART2_LOGIC_END======
		memset(RxBuffer2,0,Rec_Length2);
		Rec_Length2 = 0;//清除计数
		Rec_Complete2 = 0;//清除接收结束标志位
		HAL_UART_Receive_DMA(&huart2,(uint8_t *)RxBuffer2,RXBUFFERSIZE);//重新打开DMA接收
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
		/*__HAL_DMA_SET_COUNTER(huart2->hdmarx, RXBUFFERSIZE);
    __HAL_DMA_ENABLE(huart2->hdmarx);*/
	}
}
/* USER CODE END 1 */
