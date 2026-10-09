/******************************************************************************
 * Copyright (C) 2026 <Xzzker>
 * SPDX-License-Identifier: MIT
 *
 * @file    bsp_led.c
 * @brief   led
 *
 * @author  Xzzker xzzker@qq.com
 * @version v1.0
 * @date    2026-10-09
 *
 * @par 依赖
 * - bsp_led.h
 *
 * @note 无
******************************************************************************/

//******************************** Includes *********************************//

#include "bsp_led.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//



//******************************** Defines **********************************//

//******************************** Declaring ********************************//



//******************************** Declaring ********************************//

led_status_t led_on(void)
{
    HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_RESET);
    return LED_OK;
}

led_status_t led_off(void)
{
    HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_SET);
    return LED_OK;
}

led_status_t led_toggle(void)
{
    HAL_GPIO_TogglePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin);
    return LED_OK;
}
