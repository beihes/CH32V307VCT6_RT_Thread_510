/*
 * File      : oled.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-05-17     BHS          the first version
 */
#include "oled.h"
#include <rtthread.h>

int OLED_Init() {
    rt_err_t result = RT_EOK;

    return result;
}

INIT_APP_EXPORT (OLED_Init);