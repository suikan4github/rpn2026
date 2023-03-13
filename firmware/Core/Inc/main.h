/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KC0_Pin GPIO_PIN_0
#define KC0_GPIO_Port GPIOA
#define KC1_Pin GPIO_PIN_1
#define KC1_GPIO_Port GPIOA
#define KC2_Pin GPIO_PIN_2
#define KC2_GPIO_Port GPIOA
#define KR0_Pin GPIO_PIN_3
#define KR0_GPIO_Port GPIOA
#define KR1_Pin GPIO_PIN_4
#define KR1_GPIO_Port GPIOA
#define KR2_Pin GPIO_PIN_5
#define KR2_GPIO_Port GPIOA
#define KR3_Pin GPIO_PIN_6
#define KR3_GPIO_Port GPIOA
#define KR4_Pin GPIO_PIN_0
#define KR4_GPIO_Port GPIOB
#define KR5_Pin GPIO_PIN_1
#define KR5_GPIO_Port GPIOB
#define KR6_Pin GPIO_PIN_2
#define KR6_GPIO_Port GPIOB
#define KR7_Pin GPIO_PIN_6
#define KR7_GPIO_Port GPIOC
#define KR8_Pin GPIO_PIN_10
#define KR8_GPIO_Port GPIOA
#define LOAD_Pin GPIO_PIN_11
#define LOAD_GPIO_Port GPIOA
#define BLANK_Pin GPIO_PIN_12
#define BLANK_GPIO_Port GPIOA
#define EN24V_Pin GPIO_PIN_8
#define EN24V_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
