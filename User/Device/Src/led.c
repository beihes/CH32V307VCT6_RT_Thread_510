/*
 * File      : led.c
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
#include "led.h"
#include <rtthread.h>
#include <drv_gpio.h>
#include <rtdbg.h>

/*********************************************************************
 * @fn      led
 *
 * @brief   Test using the driver interface to operate the I/O port
 *
 * @return  none
 */
int Led_Test (void) {
    rt_uint8_t count;
    rt_pin_mode (LED_GREEN_PIN, PIN_MODE_OUTPUT);
    LOG_I ("led_SP:%08lx\r\n", __get_SP());
    for (count = 0; count < 10; count++) {
        rt_pin_write (LED_GREEN_PIN, PIN_HIGH);
        rt_thread_mdelay (200);
        rt_pin_write (LED_GREEN_PIN, PIN_LOW);
        rt_thread_mdelay (200);
    }
    rt_kprintf ("led cmd END\n");
    return 0;
}

MSH_CMD_EXPORT (Led_Test, led sample by using I / O drivers);