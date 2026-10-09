/******************************************************************************
 * Copyright (C) 2026 <Xzzker>
 * SPDX-License-Identifier: MIT
 *
 * @file    bsp_key.c
 * @brief   key
 *
 * @author  Xzzker xzzker@qq.com
 * @version v1.0
 * @date    2026-10-09
 *
 * @par 依赖
 * - bsp_key.h
 *
 * @note 无
******************************************************************************/

//******************************** Includes *********************************//

#include "bsp_key.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//



//******************************** Defines **********************************//

//******************************** Declaring ********************************//



//******************************** Declaring ********************************//

key_status_t key_scan(key_press_status_t *p_status)
{
    static uint8_t key_state = 0;
    static uint16_t cnt = 0;

    if (NULL == p_status)
        return KEY_ERROR;

    *p_status = KEY_NOT_PRESS;
    if (0 == key_state) {
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port, GPIO_KEY_Pin)) {
            key_state = 1;
        }
    }
    else if (1 == key_state) {
        if (++cnt >= 500)
            cnt = 500;
        if (GPIO_PIN_RESET != HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port, GPIO_KEY_Pin)) {
            if (cnt >= 500)
        *p_status = KEY_LONG_PRESS;
            else if (cnt >= 20)
                *p_status = KEY_PRESS;
            key_state = 0;
            cnt = 0;
        }
    }

    return KEY_OK;
}
