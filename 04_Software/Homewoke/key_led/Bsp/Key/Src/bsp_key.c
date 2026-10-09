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
    uint16_t cnt = 1000;
	
		if (NULL == p_status)
				return KEY_ERROR;

    *p_status = KEY_NOT_PRESS;
    while (cnt--) {
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIO_KEY_GPIO_Port, GPIO_KEY_Pin)) {
            *p_status = KEY_PRESS;
        }
    }
    return KEY_OK;
}
