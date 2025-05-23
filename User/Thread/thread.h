/*
 * File      : thread.h
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
#ifndef __THREAD_H
#define __THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>

// 线程
extern rt_thread_t led;   // LED
extern rt_thread_t oled;  // OLED
extern rt_thread_t dog;   // 舵机
extern rt_thread_t wifi;  // 使用esp8285

// 恢复任务
void Start_Thread (rt_thread_t thread);

// 暂停任务
void Stop_Thread (rt_thread_t thread);

void Control_Thread (int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* __THREAD_H */
