/*
 * File      : led.h
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
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#define LED_RED_PIN GET_PIN (A, 15)   // PA15
#define LED_GREEN_PIN GET_PIN (B, 4)  // PA15


#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
