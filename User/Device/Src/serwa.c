/*
 * File      : serwa.h
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
#include "serwa.h"
#include <rtthread.h>

void Dog_Stop() {
}

void Dog_Forward() {
}

void Dog_Right() {
}

void Dog_Back() {
}

void Dog_Left() {
}

int Serwa_Init() {
    rt_err_t result = RT_EOK;
    return result;
}

INIT_APP_EXPORT (Serwa_Init);
