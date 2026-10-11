/******************************************************************************
 * Copyright (C) 2026 <Xzzker>
 * SPDX-License-Identifier: MIT
 *
 * @file    rtos.h
 * @brief   
 *
 * @author  Xzzker xzzker@qq.com
 * @version v1.0
 * @date    2026-10-10
 *
 * @par 依赖
 * - FreeRTOS.h
 * - queue.h
 * - main.h
 *
 * @note 无
******************************************************************************/

#ifndef __RTOS_H__
#define __RTOS_H__

//******************************** Includes *********************************//

#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

typedef enum {
  IT_FALLING     = 0,
  IT_RISING      = 1
} it_type_t;

typedef enum {
  KEY_NOT_PRESS  = 0,
  KEY_PRESS      = 1,
  KEY_LONG_PRESS = 2
} key_press_status_t;

typedef struct {
  it_type_t type;
  uint32_t  tick;
} it_msg_t;

//******************************** Defines **********************************//

//******************************** Declaring ********************************//

extern QueueHandle_t it_to_key;
extern uint8_t toggle_cnt;

//******************************** Declaring ********************************//

#endif /* __RTOS_H__ */
