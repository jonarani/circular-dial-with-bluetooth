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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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
#define MS1_Pin GPIO_PIN_13
#define MS1_GPIO_Port GPIOB
#define MS2_Pin GPIO_PIN_14
#define MS2_GPIO_Port GPIOB
#define MS3_Pin GPIO_PIN_15
#define MS3_GPIO_Port GPIOB
#define Step_Pin GPIO_PIN_13
#define Step_GPIO_Port GPIOA
#define Direction_Pin GPIO_PIN_14
#define Direction_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define CLOCKWISE_DIRECTION (1)
#define COUNTERCLOCKWISE_DIRECTION (-1)
#define STEP_SIZE_IN_DEGREES (1.8)
#define MIN_DEGREES (0.0)
#define MAX_DEGREES (360.0)
#define	max(a, b) (((a) >= (b)) ? (a) : (b))
#define min(a, b) (((a) <= (b)) ? (a) : (b))

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
