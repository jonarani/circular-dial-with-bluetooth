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
extern uint8_t rxDataFromBt[50];
extern double targetRotation;

// 0.45, 0.9 and 1.8 possible step sizes
extern double stepSize;

// how much delay between low and high voltages in servo motor
extern uint16_t stepSpeed;

// if enabled then cant control manually, automatically scans configured area
extern bool automaticControlEnabled;

// goes from 0 to 180 degrees continously
extern double areaToScan;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void processConfigData(uint8_t *data, uint8_t size);
void servoOneStep();
void servoSetClockwiseDirection();
void servoSetCounterClockwiseDirection();
void servoReverseDirection();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
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
