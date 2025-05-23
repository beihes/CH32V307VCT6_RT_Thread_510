/*
 * File      : wifi.h
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
#ifndef __WIFI_H
#define __WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include <drv_usart.h>

extern UARTData uart2;

#define ESP8285_PORT 8285

int USART2_Printf (const char *format, ...);

void Wifi_SendData (const char *buf, rt_uint16_t len);

int Wifi_Init();

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_H */
