/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-27     liYony       the first version
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <ch32v30x.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

/* Do not use GPIO_Remap*/
#define GPIO_Remap_NONE 0

// GPIO_Remap_USART2 1
#define BSP_USART2_Remap 1
// GPIO_FullRemap_USART3 3
#define BSP_USART3_Remap 3

#define RX_BUF_LENGTH 255  // 不用256的原因是要把最后一位给'\0'

typedef struct UARTData {
    rt_uint8_t rxBuf[128];            // 数据缓冲区
    rt_uint8_t txBuf[RX_BUF_LENGTH];  // 发送缓冲区
    rt_uint8_t ch;                    // 中断接收缓冲区
    rt_uint8_t finishFlag;            // 接收完成标志
    rt_uint16_t rxBufLength;          // 缓冲区索引
    rt_device_t device;               // 设备指针
} UARTData;

/* ch32 hardware config class */
struct ch32_uart_hw_config {
    rt_uint32_t uart_periph_clock;
    rt_uint32_t gpio_periph_clock;
    GPIO_TypeDef *tx_gpio_port;
    rt_uint16_t tx_gpio_pin;
    GPIO_TypeDef *rx_gpio_port;
    rt_uint16_t rx_gpio_pin;
    rt_uint32_t remap;
};

/* ch32 config class */
struct ch32_uart_config {
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
};

/* ch32 uart dirver class */
struct ch32_uart {
    struct ch32_uart_hw_config *hw_config;
    struct ch32_uart_config *config;
    USART_InitTypeDef Init;
    struct rt_serial_device serial;
};

int rt_hw_usart_init (void);

#ifdef __cplusplus
}
#endif

#endif
