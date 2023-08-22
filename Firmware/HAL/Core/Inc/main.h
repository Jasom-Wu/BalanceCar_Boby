/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EEPROM_SDA_Pin GPIO_PIN_6
#define EEPROM_SDA_GPIO_Port GPIOA
#define EEPROM_SCL_Pin GPIO_PIN_7
#define EEPROM_SCL_GPIO_Port GPIOA
#define MPU6050_SCL_Pin GPIO_PIN_0
#define MPU6050_SCL_GPIO_Port GPIOB
#define MPU6050_SDA_Pin GPIO_PIN_1
#define MPU6050_SDA_GPIO_Port GPIOB
#define MPU6050_AD0_Pin GPIO_PIN_10
#define MPU6050_AD0_GPIO_Port GPIOB
#define MPU6050_INT_Pin GPIO_PIN_11
#define MPU6050_INT_GPIO_Port GPIOB
#define MPU6050_INT_EXTI_IRQn EXTI15_10_IRQn
#define AIN1_Pin GPIO_PIN_12
#define AIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_13
#define AIN2_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_14
#define BIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_15
#define BIN2_GPIO_Port GPIOB
#define PWMA_Pin GPIO_PIN_8
#define PWMA_GPIO_Port GPIOA
#define PWMB_Pin GPIO_PIN_11
#define PWMB_GPIO_Port GPIOA
#define ULTRA_TRIG_Pin GPIO_PIN_12
#define ULTRA_TRIG_GPIO_Port GPIOA
#define ULTRA_ECHO_Pin GPIO_PIN_15
#define ULTRA_ECHO_GPIO_Port GPIOA
#define ULTRA_ECHO_EXTI_IRQn EXTI15_10_IRQn
#define OLED_SDA_Pin GPIO_PIN_8
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_9
#define OLED_SCL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
