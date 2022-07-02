/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern osMessageQueueId_t rotationQueueHandle;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void processConfigData(uint8_t *data, uint8_t size)
{
	uint8_t *curData = data;
	uint8_t buffer[7] = {0};

	// stepSize 4, stepSpeed 3, isAutomatic 1, scannableArea 6, CR 1, LF 1
	memcpy(buffer, curData, 4);
	stepSize = atof((char*)buffer);
	curData += 4;

	memset(buffer, 0, 7);
	memcpy(buffer, curData, 3);
	stepSpeed = atoi((char*)buffer);
	curData += 3;

	automaticControlEnabled = curData[0] == '1' ? true : false;
	curData++;

	memset(buffer, 0, 7);
	memcpy(buffer, curData, 6);
	areaToScan = atof((char*)buffer);

	printf("stepSize: %f\r\n", stepSize);
	printf("stepSpeed: %d\r\n", stepSpeed);
	printf("automaticControlEnabled: %d\r\n", automaticControlEnabled);
	printf("areaToScan: %f\r\n", areaToScan);
}

void StartUartSend(void *argument)
{
	for(;;)
	{
		printf("Hello\r\n");
		osDelay(1000);
	}
}

void StartServoControl(void *argument)
{
  /* USER CODE BEGIN 5 */
	double currentRotation = 0.0;
	double newRotation = 0.0;
	double stepsToTake = 0.0;
	uint16_t i = 0;
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(rotationQueueHandle, &newRotation, NULL, osWaitForever);

	  stepsToTake = fabs((double)(newRotation - currentRotation) / (double)1.800);

	  if (stepsToTake > (MAX_DEGREES / STEP_SIZE_IN_DEGREES) / 2)
	  {
		  stepsToTake = (MAX_DEGREES / STEP_SIZE_IN_DEGREES) - stepsToTake;
		  if (newRotation > currentRotation)
		  {
			  servoSetCounterClockwiseDirection();
		  }
		  else
		  {
			  servoSetClockwiseDirection();
		  }
	  }
	  else if (newRotation < currentRotation)
	  {
		  servoSetCounterClockwiseDirection();
	  }
	  else
	  {
		  servoSetClockwiseDirection();
	  }

	  for (i = 0; i < stepsToTake; i++)
	  {
		  servoOneStep();
	  }

	  printf("%f %f %f\r\n", currentRotation, newRotation, stepsToTake);

	  currentRotation = newRotation;
  }
  /* USER CODE END 5 */
}

void StartUartPoll(void *argument)
{
  /* USER CODE BEGIN StartUartPoll */
	uint16_t rxLen;
	osStatus_t status;
	char transmitData[4] = {'O', 'K', '\r', '\n'};

  /* Infinite loop */
	for(;;)
	{
		HAL_UARTEx_ReceiveToIdle(&huart6, rxDataFromBt, 17, &rxLen, 0xFFFFFFFF);

		// 7F is a start byte for config info
		if (rxDataFromBt[0] == 0x7F)
		{
			processConfigData(&rxDataFromBt[1], rxLen - 1 - 2);
			while (HAL_UART_Transmit(&huart6, (uint8_t*)transmitData, 4, 1000) != HAL_OK);
		}
		else
		{
			targetRotation = atof((char*)rxDataFromBt);
			printf ("%f\r\n", targetRotation);
			if ( (status = osMessageQueuePut(rotationQueueHandle, &targetRotation, 0U, 0U)) != osOK)
			{
			  printf("status: %d\r\n", status);
			}
		}
	}
  /* USER CODE END StartUartPoll */
}

/* USER CODE END Application */

