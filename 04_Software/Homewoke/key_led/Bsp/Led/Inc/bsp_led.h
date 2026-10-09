/******************************************************************************
 * Copyright (C) 2026 <Xzzker>
 * SPDX-License-Identifier: MIT
 *
 * @file    bsp_led.h
 * @brief   led
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

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

//******************************** Includes *********************************//

#include "gpio.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

typedef enum {
    LED_OK        = 0,
    LED_ERROR     = 1
} led_status_t;

//******************************** Defines **********************************//

//******************************** Declaring ********************************//

led_status_t led_on(void);
led_status_t led_off(void);
led_status_t led_toggle(void);

//******************************** Declaring ********************************//

#endif /* __BSP_LED_H__ */
