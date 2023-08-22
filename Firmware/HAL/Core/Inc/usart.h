/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#include <stdio.h>
#include <string.h>
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define TX_DATA_SIZE  256
#define RXBUFFERSIZE  256     //最大接收字节数
#define TXBUFFERSIZE 256
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern char RxBuffer2[RXBUFFERSIZE];   //接收数据
extern char TxBuffer2[TXBUFFERSIZE];	//发送数据
extern char RxBuffer1[RXBUFFERSIZE];   //接收数据
extern char TxBuffer1[TXBUFFERSIZE];	//发送数据
extern __IO uint8_t aRxBuffer1;
extern __IO uint8_t aRxBuffer2;
extern __IO uint16_t Rec_Length1;
extern __IO _Bool Rec_Complete1;
extern __IO uint16_t Rec_Length2;
extern __IO _Bool Rec_Complete2;
extern __IO uint8_t USART_FLAG;
int Printf(USART_TypeDef *USARTx,char *formatString,...);
uint16_t DMA_Printf(USART_TypeDef *USARTx,char *formatString,...);
void USART_DMARec_Stop(UART_HandleTypeDef *huart);
char Getchar(USART_TypeDef *USARTx);
void USART1_IDLE_Handler(void);
void USART2_IDLE_Handler(void);
void USART1_REC_Handler(void);
void USART2_REC_Handler(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

