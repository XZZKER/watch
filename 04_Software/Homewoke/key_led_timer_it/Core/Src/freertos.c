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

#include "rtos.h"
#include "queue.h"
#include "gpio.h"

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

QueueHandle_t it_to_key, key_to_led;
uint8_t toggle_cnt;

/* USER CODE END Variables */
/* Definitions for key_task */
osThreadId_t key_taskHandle;
const osThreadAttr_t key_task_attributes = {
  .name = "key_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for led_task */
osThreadId_t led_taskHandle;
const osThreadAttr_t led_task_attributes = {
  .name = "led_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void key_task_func(void *argument);
void led_task_func(void *argument);

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

  it_to_key = xQueueCreate(16, sizeof(it_msg_t*));
  key_to_led = xQueueCreate(16, sizeof(key_press_status_t*));

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of key_task */
  key_taskHandle = osThreadNew(key_task_func, NULL, &key_task_attributes);

  /* creation of led_task */
  led_taskHandle = osThreadNew(led_task_func, NULL, &led_task_attributes);

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

  it_msg_t *p_msg = NULL;
  uint32_t last_tick = 0;
  key_press_status_t status = KEY_NOT_PRESS;

  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(it_to_key, &p_msg, portMAX_DELAY);
    if (p_msg->type == IT_FALLING) {
      last_tick = p_msg->tick;
    }
    else {
      if (p_msg->tick - last_tick > 500)
        status = KEY_LONG_PRESS;
      else if (p_msg->tick - last_tick > 20)
        status = KEY_PRESS;
      else
        continue;
      key_press_status_t *p_status = &status;
      xQueueSendToBack(key_to_led, &p_status, portMAX_DELAY);
    }

    osDelay(1);
  }
  /* USER CODE END key_task_func */
}

/* USER CODE BEGIN Header_led_task_func */
/**
* @brief Function implementing the led_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_task_func */
void led_task_func(void *argument)
{
  /* USER CODE BEGIN led_task_func */

  key_press_status_t *p_status = NULL;

  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(key_to_led, &p_status, portMAX_DELAY);
    key_press_status_t status = *p_status;
    if (status == KEY_PRESS)
      toggle_cnt++;
    else if (status == KEY_LONG_PRESS)
      toggle_cnt += 5;

    osDelay(1);
  }
  /* USER CODE END led_task_func */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

