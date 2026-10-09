/******************************************************************************
 * Copyright (C) 2026 <Xzzker>
 * SPDX-License-Identifier: MIT
 *
 * @file    bsp_key.h
 * @brief   key
 *
 * @author  Xzzker xzzker@qq.com
 * @version v1.0
 * @date    2026-10-09
 *
 * @par 依赖
 * - gpio.h
 *
 * @note 无
******************************************************************************/

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

//******************************** Includes *********************************//

#include "gpio.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

typedef enum {
    KEY_OK        = 0,
    KEY_ERROR     = 1
} key_status_t;

typedef enum {
    KEY_PRESS     = 0,
    KEY_NOT_PRESS = 1
} key_press_status_t;

//******************************** Defines **********************************//

//******************************** Declaring ********************************//

key_status_t key_scan(key_press_status_t *p_status);

//******************************** Declaring ********************************//

#endif /* __BSP_KEY_H__ */
