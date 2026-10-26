/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUFFER_SIZE 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

QueueHandle_t queue;
uint32_t *buffer1, *buffer2;

/* USER CODE END Variables */
/* Definitions for data_acceptance */
osThreadId_t data_acceptanceHandle;
const osThreadAttr_t data_acceptance_attributes = {
  .name = "data_acceptance",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for data_treating_t */
osThreadId_t data_treating_tHandle;
const osThreadAttr_t data_treating_t_attributes = {
  .name = "data_treating_t",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void DataAcceptanceTask(void *argument);
void DataTreatingTaskFunc(void *argument);

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
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  queue = xQueueCreate(1, sizeof(uint32_t*));
  if (queue == NULL)
  {
    printf("queue create failed!\r\n");
  }

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of data_acceptance */
  data_acceptanceHandle = osThreadNew(DataAcceptanceTask, NULL, &data_acceptance_attributes);

  /* creation of data_treating_t */
  data_treating_tHandle = osThreadNew(DataTreatingTaskFunc, NULL, &data_treating_t_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_DataAcceptanceTask */
/**
  * @brief  Function implementing the data_acceptance thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_DataAcceptanceTask */
void DataAcceptanceTask(void *argument)
{
  /* USER CODE BEGIN DataAcceptanceTask */
  /* Infinite loop */

  buffer1 = malloc(sizeof(uint32_t) * BUFFER_SIZE);
  buffer2 = malloc(sizeof(uint32_t) * BUFFER_SIZE);
  if (buffer1 == NULL || buffer2 == NULL)
  {
    printf("buffer init failed!\r\n");
  }
  memset(buffer1, 0x00, BUFFER_SIZE);
  memset(buffer2, 0x00, BUFFER_SIZE);

  uint32_t *current_buffer = buffer1;
  if (HAL_ADC_Start_DMA(&hadc1, current_buffer, BUFFER_SIZE) != HAL_OK)
  {
    printf("adc start failed!\r\n");
  }

  for(;;)
  {
    if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
    {
      if (HAL_ADC_Start_DMA(&hadc1, current_buffer, BUFFER_SIZE) != HAL_OK)
      {
        printf("adc start failed!\r\n");
      }
      if (xQueueSend(queue, &current_buffer, portMAX_DELAY) != pdTRUE)
      {
        printf("queue send failed!\r\n");
      }
    }
  }
  /* USER CODE END DataAcceptanceTask */
}

/* USER CODE BEGIN Header_DataTreatingTaskFunc */
/**
* @brief Function implementing the data_treating_t thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataTreatingTaskFunc */
void DataTreatingTaskFunc(void *argument)
{
  /* USER CODE BEGIN DataTreatingTaskFunc */
  /* Infinite loop */

  uint32_t *buffer = NULL;
  float mv;
  
  for(;;)
  {
    if (xQueuePeek(queue, &buffer, portMAX_DELAY) != pdTRUE)
    {
      printf("queue peek failed!\r\n");
    }
    mv = *buffer * 3300 / 4095;
    printf("ADC1: %.2fmV\r\n", mv);
    if (xQueueReceive(queue, &buffer, 0) != pdTRUE)
    {
      printf("queue receive failed!\r\n");
    }
  }
  /* USER CODE END DataTreatingTaskFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  UNUSED(hadc);

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(data_acceptanceHandle, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* USER CODE END Application */

