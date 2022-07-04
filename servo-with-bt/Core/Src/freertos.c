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
extern osMutexId_t configParamsMutexHandle;
extern osThreadId_t servoAutoControlHandle;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SERVO_QUARTER_STEP 	(0.45)	// Has some issues due to the fact that QML app uses 1 precision integer
#define SERVO_HALF_STEP  	(0.9)
#define SERVO_FULL_STEP  	(1.8)

typedef enum ServoStates {
	SERVO_STATE_IDLE = 0,
	SERVO_STATE_RUNNING,
} ServoStates;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static uint8_t rxDataFromBt[50] = {0};
static uint8_t txDataToBt[50] = {0};

// 0.45, 0.9 and 1.8 possible step sizes
static double gStepSize = SERVO_FULL_STEP;

// How much delay between low and high voltages in servo motor
static uint16_t gStepSpeed = 1;

// If enabled then cant control manually, automatically scans configured area
static bool gAutomaticControlEnabled = false;

// Goes from 0 to 180 degrees continously
static double gAreaToScan = 180.0;

static ServoStates servoState = SERVO_STATE_IDLE;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

static void servoOneStep()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
  osDelay(gStepSpeed);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
  osDelay(gStepSpeed);
}

static void servoSetClockwiseDirection()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_RESET);
}

static void servoSetCounterClockwiseDirection()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
}

static void servoReverseDirection()
{
	HAL_GPIO_TogglePin(Direction_GPIO_Port, Direction_Pin);
}

static void setQuarterStep()
{
	HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
}

static void setHalfStep()
{
	HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
}

static void setFullStep()
{
	HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
}

void setStepSize()
{
	if (gStepSize == SERVO_QUARTER_STEP)
	{
		printf("Set 1/4 step\r\n");
		setQuarterStep();
	}
	else if (gStepSize == SERVO_HALF_STEP)
	{
		printf("Set 1/2 step\r\n");
		setHalfStep();
	}
	else if (gStepSize == SERVO_FULL_STEP)
	{
		printf("Set 1 step\r\n");
		setFullStep();
	}
	else
	{
		printf("Set default step\r\n");
		setFullStep();
	}
}

static void processAndSetServoParams(uint8_t *data, uint8_t size)
{
	uint8_t *curData = data;
	uint8_t buffer[7] = {0};

	// stepSize 4, stepSpeed 3, isAutomatic 1, scannableArea 6, CR 1, LF 1
	memcpy(buffer, curData, 4);
	gStepSize = atof((char*)buffer);
	curData += 4;

	memset(buffer, 0, 7);
	memcpy(buffer, curData, 3);
	gStepSpeed = atoi((char*)buffer);
	curData += 3;

	gAutomaticControlEnabled = curData[0] == '1' ? true : false;
	curData++;

	memset(buffer, 0, 7);
	memcpy(buffer, curData, 6);
	gAreaToScan = atof((char*)buffer);

	setStepSize();

	printf("stepSize: %f\r\n", gStepSize);
	printf("stepSpeed: %d\r\n", gStepSpeed);
	printf("automaticControlEnabled: %d\r\n", gAutomaticControlEnabled);
	printf("areaToScan: %f\r\n", gAreaToScan);
}

void StartUartSend(void *argument)
{
	for(;;)
	{
		osThreadSuspend(osThreadGetId());
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
	  servoState = SERVO_STATE_RUNNING;
	  stepsToTake = fabs((double)(newRotation - currentRotation) / gStepSize);

	  if (stepsToTake > (MAX_DEGREES / gStepSize) / 2)
	  {
		  stepsToTake = (MAX_DEGREES / gStepSize) - stepsToTake;
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

	  servoState = SERVO_STATE_IDLE;

	  printf("%f %f %f\r\n", currentRotation, newRotation, stepsToTake);

	  currentRotation = newRotation;
  }
  /* USER CODE END 5 */
}

void StartServoAutoControl(void *argument)
{
	uint32_t flags = 0;
	uint16_t i = 0;

	for (;;)
	{
		flags = osThreadFlagsGet(); // TODO: why is this call required
		//printf ("%lu\r\n", flags);
		flags = osThreadFlagsWait(0x03U, osFlagsWaitAny | osFlagsNoClear, osWaitForever);

		if (flags & 0x02U)
		{
			printf ("Clearing flags\r\n");
			osThreadFlagsClear(0x03U);
		}

		if (flags & 0x01)
		{
			//printf ("Auto ctrl\r\n");
			if (gAreaToScan == 0.0)
			{
				// The same location always
				// Do nothing
			}
			else if (gAreaToScan == 360.0)
			{
				servoState = SERVO_STATE_RUNNING;

				// Rotate endlessly in circles
				for (i = 0; i < MAX_DEGREES / gStepSize; i++)
				{
					servoOneStep();
					// TODO: check flags here
					// and if flag 0x02 is set then end and clear
				}
			}
			else
			{
				servoState = SERVO_STATE_RUNNING;

				for (i = 0; i < gAreaToScan / gStepSize; i++)
				{
					servoOneStep();
					// TODO: check flags here
					// and if flag 0x02 is set then end and clear
				}

				servoReverseDirection();
			}

			servoState = SERVO_STATE_IDLE;
		}

		//osDelay(1000);
	}
}

void StartUartPoll(void *argument)
{
  /* USER CODE BEGIN StartUartPoll */
	uint16_t rxLen;
	osStatus_t status;

	// The rotation to set on a servo motor
	double targetRotation = 0.;

  /* Infinite loop */
	for(;;)
	{
		HAL_UARTEx_ReceiveToIdle(&huart6, rxDataFromBt, 17, &rxLen, 0xFFFFFFFF);

		// 7F is a start byte for config info
		if (rxDataFromBt[0] == 0x7F)
		{
			printf ("7F\r\n");
			if (gAutomaticControlEnabled == false)
			{
				targetRotation = 0.0;
				osMessageQueuePut(rotationQueueHandle, &targetRotation, 0U, 0U);

				// Process already queued elements
				while (osMessageQueueGetCount(rotationQueueHandle) != 0)
				{
					osThreadYield();
				}
			}
			else
			{
				osThreadFlagsSet(servoAutoControlHandle, 0x02U);
			}

			osDelay(5);
			while (servoState != SERVO_STATE_IDLE) {}

			// Set new params
			processAndSetServoParams(&rxDataFromBt[1], rxLen - 1 - 2);

			if (gAutomaticControlEnabled == true)
			{
				osThreadFlagsSet(servoAutoControlHandle, 0x01U);
			}
			else
			{
				osThreadFlagsSet(servoAutoControlHandle, 0x02U);
			}

			txDataToBt[0] = 'O';
			txDataToBt[1] = 'K';
			txDataToBt[2] = '\r';
			txDataToBt[3] = '\n';
			while (HAL_UART_Transmit(&huart6, (uint8_t*)txDataToBt, 4, 3000) != HAL_OK);
		}
		else
		{
			if (gAutomaticControlEnabled == false)
			{
				targetRotation = atof((char*)rxDataFromBt);
				//printf ("%f\r\n", targetRotation);
				if (targetRotation < 0.0 || targetRotation > 360.0)
				{
					continue;
				}

				if ( (status = osMessageQueuePut(rotationQueueHandle, &targetRotation, 0U, 0U)) != osOK)
				{
					printf("status: %d\r\n", status);
				}
			}
		}
	}
  /* USER CODE END StartUartPoll */
}

/* USER CODE END Application */

