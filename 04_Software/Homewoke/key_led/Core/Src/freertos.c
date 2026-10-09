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
#include "usart.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

QueueHandle_t key_queue;

/* USER CODE END Variables */
/* Definitions for key_task */
osThreadId_t key_taskHandle;
const osThreadAttr_t key_task_attributes = {
  .name = "key_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for blink_task */
osThreadId_t blink_taskHandle;
const osThreadAttr_t blink_task_attributes = {
  .name = "blink_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

int fputc(int ch, FILE *f)
{
  uint8_t c = ch;
  HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
  return ch;
}

/* USER CODE END FunctionPrototypes */

void key_task_func(void *argument);
void blink_task_func(void *argument);

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

  key_queue = xQueueCreate(16, sizeof(key_press_status_t));

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of key_task */
  key_taskHandle = osThreadNew(key_task_func, NULL, &key_task_attributes);

  /* creation of blink_task */
  blink_taskHandle = osThreadNew(blink_task_func, NULL, &blink_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_key_task_func */
/**
  * @brief  Function implementing the key_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_key_task_func */
void key_task_func(void *argument)
{
  /* USER CODE BEGIN key_task_func */

  key_press_status_t state = KEY_NOT_PRESS;

  /* Infinite loop */
  for(;;)
  {
    if (KEY_OK != key_scan(&state))
        printf("key error!\r\n");
    
    if (KEY_NOT_PRESS != state)
        xQueueSend(key_queue, &state, 0);
    osDelay(1);
  }
  /* USER CODE END key_task_func */
}

/* USER CODE BEGIN Header_blink_task_func */
/**
* @brief Function implementing the blink_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_blink_task_func */
void blink_task_func(void *argument)
{
  /* USER CODE BEGIN blink_task_func */

  key_press_status_t key_state = KEY_NOT_PRESS;

  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(key_queue, &key_state, portMAX_DELAY);
    if (KEY_PRESS == key_state)
        led_on();
    if (KEY_LONG_PRESS == key_state)
        led_off();
  }
  /* USER CODE END blink_task_func */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

