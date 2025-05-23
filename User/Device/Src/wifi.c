/*
 * File      : wifi.c
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
#include "wifi.h"
#include <thread.h>

UARTData uart2 = {0};

int USART2_Printf (const char *format, ...) {
    rt_uint32_t length = 0;
    va_list args;
    va_start (args, format);
    length = rt_vsnprintf ((char *)uart2.txBuf, sizeof (uart2.txBuf), format, args);
    rt_device_write (uart2.device, 0, uart2.txBuf, length);
    return length;
}

void Wifi_SendData (const char *buf, rt_uint16_t len) {
}

static rt_err_t uart2_rx_callback (rt_device_t dev, rt_size_t size) {
    if (rt_device_read (dev, 0, &uart2.ch, 1) > 0 && uart2.rxBufLength < RX_BUF_LENGTH) {
        uart2.rxBuf[uart2.rxBufLength++] = uart2.ch;
        uart2.rxBuf[uart2.rxBufLength] = '\0';
        uart2.finishFlag = ENABLE;
    }
    return RT_EOK;
}

int Wifi_Init() {
    uart2.device = rt_device_find ("uart2");
    if (!uart2.device) {
        rt_kprintf ("can not find uart2 device");
        return -RT_ERROR;
    }
    rt_device_set_rx_indicate (uart2.device, uart2_rx_callback);
    return rt_device_open (uart2.device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);  // 可加 INT_RX 支持接收
}

INIT_APP_EXPORT (Wifi_Init);