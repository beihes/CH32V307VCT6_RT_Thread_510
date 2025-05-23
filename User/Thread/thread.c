/*
 * File      : thread.c
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
#include "thread.h"
#include "device.h"
#include <time.h>
#include <rtdbg.h>
#include <drv_gpio.h>

// 恢复任务
void Start_Thread (rt_thread_t thread) {
    if (thread->stat == RT_THREAD_STAT_MASK) {  // 挂起状态
        rt_thread_resume (thread);
        rt_kprintf ("%s\t:Thread RESUME\n", thread->parent.name);
    } else {
        rt_kprintf ("Thread %s is not in suspend state, cannot resume\n", thread->parent.name);
    }
}

// 暂停任务
void Stop_Thread (rt_thread_t thread) {
    char midSemName[16] = "";
    rt_sprintf (midSemName, "%sSem", thread->parent.name);
    rt_sem_t midSem = (rt_sem_t)rt_object_find (midSemName, RT_Object_Class_Semaphore);
    if (midSem != RT_NULL) {
        rt_sem_release (midSem);
        rt_kprintf ("%s\t:Thread STOP\n", thread->parent.name);
    } else {
        rt_kprintf ("Thread %s Sem connot find\n", thread->parent.name);
    }
}

void Control_Thread (int argc, char *argv[]) {
    if (argc < 3) {
        rt_kprintf ("Usage: Control_Thread\ncmd: [-c creat_thread] [-d delete_thread] [-s thread_state]\nthreadName: [thread_name]\n");
        return;
    }
    // 获取任务名称
    char *thread_name = argv[2];
    rt_thread_t thread = rt_thread_find (thread_name);  // 查找任务
    if (thread == RT_NULL) {
        rt_kprintf ("Thread %s not found\n", thread_name);
        return;
    }

    char *cmdStr = argv[1];
    char *cmdData = argv[3];
    if (rt_strcmp (cmdStr, "-c") == 0) {
        rt_kprintf ("DEBUG\t:create thread!\n");
    } else if (rt_strcmp (cmdStr, "-d") == 0) {
        rt_kprintf ("DEBUG\t:delete thread!\n");
    } else if (rt_strcmp (cmdStr, "-s") == 0) {
        switch (cmdData[0]) {
        case '0':
            // 暂停任务
            Stop_Thread (thread);
            return;
        case '1':
            // 恢复任务
            Start_Thread (thread);
            return;
        default:
            rt_kprintf ("please input 0(close) or 1(open)\n");
            return;
        }
    } else {
        rt_kprintf ("please check input\n");
    }
}

MSH_CMD_EXPORT (Control_Thread, 控制线程命令);

// 变量
rt_uint32_t frameRate = 0;
rt_uint8_t dogCmd = 0;

// 线程
rt_thread_t led = RT_NULL;   // LED
rt_thread_t oled = RT_NULL;  // OLED
rt_thread_t dog = RT_NULL;   // 舵机
rt_thread_t wifi = RT_NULL;  // 使用esp8285
// 信号量
static rt_sem_t ledSem = RT_NULL;
static rt_sem_t oledSem = RT_NULL;
static rt_sem_t dogSem = RT_NULL;
static rt_sem_t wifiSem = RT_NULL;

void led_entry() {
    while (1) {
        rt_pin_write (LED_RED_PIN, PIN_HIGH);
        rt_thread_delay (500);
        rt_pin_write (LED_RED_PIN, PIN_LOW);
        rt_thread_delay (500);
        if (rt_sem_take (ledSem, RT_WAITING_NO) == RT_EOK) {
            // 息屏
            if (rt_thread_suspend (led) != RT_EOK)        // 暂停当前线程
            {
                rt_kprintf ("led thread Stop ERROR!\n");  // 打印暂停日志
            }
            rt_schedule();                                // 强制调度
        }
    }
}

void oled_entry() {
    time_t per = time (NULL);
    time_t now = time (NULL);
    rt_uint32_t midFrameRate = 0;
    while (1) {
        rt_thread_mdelay (500);
        midFrameRate++;
        now = time (NULL);
        if (now - per >= 1) {
            frameRate = midFrameRate;
            midFrameRate = 0;
        }
        if (rt_sem_take (oledSem, RT_WAITING_NO) == RT_EOK) {
            // 息屏
            if (rt_thread_suspend (oled) != RT_EOK)        // 暂停当前线程
            {
                rt_kprintf ("oled thread Stop ERROR!\n");  // 打印暂停日志
            }
            rt_schedule();                                 // 强制调度
        }
    }
}

void Get_FrameRate() {
    rt_kprintf ("frameRate:%d\n", frameRate);
}

MSH_CMD_EXPORT (Get_FrameRate, 获取当前帧率);

void dog_entry() {
    while (1) {
        switch (dogCmd) {
        case STOP: Dog_Stop(); break;
        case FORWARD: Dog_Forward(); break;
        case RIGHT: Dog_Right(); break;
        case BACK: Dog_Back(); break;
        case LEFT: Dog_Left(); break;
        default: break;
        }
        dogCmd = DOG_CMD_NUM;
        rt_sem_release (dogSem);
        rt_thread_mdelay (500);
        if (rt_sem_take (dogSem, RT_WAITING_NO) == RT_EOK) {
            // 息屏
            if (rt_thread_suspend (dog) != RT_EOK)        // 暂停当前线程
            {
                rt_kprintf ("dog thread Stop ERROR!\n");  // 打印暂停日志
            }
            rt_schedule();                                // 强制调度
        }
    }
}

void wifi_entry() {
    while (1) {
        rt_kprintf ("%s\n", uart2.rxBuf);
        USART2_Printf ("%s\n", uart2.rxBuf);
        uart2.finishFlag = 0;
        uart2.rxBufLength = 0;
        rt_sem_release (wifiSem);
        // rt_thread_mdelay (500);
        if (rt_sem_take (wifiSem, RT_WAITING_NO) == RT_EOK) {
            // 息屏
            if (rt_thread_suspend (wifi) != RT_EOK)        // 暂停当前线程
            {
                rt_kprintf ("wifi thread Stop ERROR!\n");  // 打印暂停日志
            }
            rt_schedule();                                 // 强制调度
        }
    }
}

int Thread_Init() {
    rt_err_t result = RT_EOK;
    rt_pin_mode (LED_RED_PIN, PIN_MODE_OUTPUT);
    ledSem = rt_sem_create ("ledSem", 0, RT_IPC_FLAG_PRIO);
    led = rt_thread_create ("led", led_entry, "led 的线程", 512, 10, 10);
    if (led != RT_NULL && ledSem != RT_NULL) {
        rt_thread_startup (led);
        // rt_sem_release (ledSem);
    } else {
        rt_kprintf ("creat_thread error");
    }
    oledSem = rt_sem_create ("oledSem", 0, RT_IPC_FLAG_PRIO);
    oled = rt_thread_create ("oled", oled_entry, "oled 的线程", 1024, 10, 10);
    if (oled != RT_NULL && oledSem != RT_NULL) {
        rt_thread_startup (oled);
        rt_sem_release (oledSem);
    } else {
        rt_kprintf ("creat_thread error");
    }
    dogSem = rt_sem_create ("dogSem", 0, RT_IPC_FLAG_PRIO);
    dog = rt_thread_create ("dog", dog_entry, "dog 的线程", 1024, 10, 10);
    if (dog != RT_NULL && dogSem != RT_NULL) {
        rt_thread_startup (dog);
        rt_sem_release (dogSem);
    } else {
        rt_kprintf ("creat_thread error");
    }
    wifiSem = rt_sem_create ("wifiSem", 0, RT_IPC_FLAG_PRIO);
    wifi = rt_thread_create ("wifi", wifi_entry, "wifi 的线程", 1024, 10, 10);
    if (wifi != RT_NULL && wifiSem != RT_NULL) {
        rt_thread_startup (wifi);
        rt_sem_release (wifiSem);
    } else {
        rt_kprintf ("creat_thread error");
    }
    return result;
}

INIT_APP_EXPORT (Thread_Init);