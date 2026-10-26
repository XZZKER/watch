/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdlib.h>
#include <string.h>
#include "adc.h"
#include "queue.h"
#include "semphr.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define BUFFER_SIZE 1

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

QueueHandle_t queue;
SemaphoreHandle_t semaphore;
uint32_t *buffer1, *buffer2;

/* USER CODE END Variables */
/* Definitions for ReadADCTask */
osThreadId_t ReadADCTaskHandle;
const osThreadAttr_t ReadADCTask_attributes = {
  .name = "ReadADCTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for DebugTask */
osThreadId_t DebugTaskHandle;
const osThreadAttr_t DebugTask_attributes = {
  .name = "DebugTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ReadADCTaskFunc(void *argument);
void DebugTaskFunc(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  semaphore = xSemaphoreCreateBinary();

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  queue = xQueueCreate(10, sizeof(uint32_t*));

  if (queue == NULL) {
    printf("queue init failed!\r\n");
    return;
  }

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ReadADCTask */
  ReadADCTaskHandle = osThreadNew(ReadADCTaskFunc, NULL, &ReadADCTask_attributes);

  /* creation of DebugTask */
  DebugTaskHandle = osThreadNew(DebugTaskFunc, NULL, &DebugTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ReadADCTaskFunc */
/**
  * @brief  Function implementing the ReadADCTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ReadADCTaskFunc */
void ReadADCTaskFunc(void *argument)
{
  /* USER CODE BEGIN ReadADCTaskFunc */

  buffer1 = malloc(sizeof(uint32_t) * BUFFER_SIZE);
  buffer2 = malloc(sizeof(uint32_t) * BUFFER_SIZE);

  if (buffer1 == NULL || buffer2 == NULL) {
    printf("buffer init failed!\r\n");
    return;
  }

  memset(buffer1, 0x00, sizeof(uint32_t) * BUFFER_SIZE);
  memset(buffer2, 0x00, sizeof(uint32_t) * BUFFER_SIZE);

  uint32_t *current_buffer = buffer1;
  if (HAL_ADC_Start_DMA(&hadc1, current_buffer, BUFFER_SIZE) != HAL_OK)
  {
    printf("adc start failed!\r\n");
    return;
  }

  /* Infinite loop */
  for(;;)
  {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    xQueueSend(queue, &current_buffer, 0);
    current_buffer = current_buffer == buffer1 ? buffer2 : buffer1;
    if (HAL_ADC_Start_DMA(&hadc1, current_buffer, BUFFER_SIZE) != HAL_OK)
    {
      printf("adc start failed!\r\n");
      return;
    }
    osDelay(1);
  }
  /* USER CODE END ReadADCTaskFunc */
}

/* USER CODE BEGIN Header_DebugTaskFunc */
/**
* @brief Function implementing the DebugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DebugTaskFunc */
void DebugTaskFunc(void *argument)
{
  /* USER CODE BEGIN DebugTaskFunc */
  /* Infinite loop */

  uint32_t *buffer;

  for(;;)
  {
    xQueueReceive(queue, &buffer, portMAX_DELAY);
    printf("adc: %u\r\n", buffer[0]);
  }
  /* USER CODE END DebugTaskFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  xSemaphoreGiveFromISR(semaphore, NULL);
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
}

/* USER CODE END Application */

