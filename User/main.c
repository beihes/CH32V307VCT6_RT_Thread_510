/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include <drv_gpio.h>
#include <rtdbg.h>
#include "device.h"
#include "thread.h"

/* Global typedef */

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      main
 *
 * @brief   main is just one of the threads, in addition to tshell,idle
 * This main is just a flashing LED, the main thread is registered in
 * rtthread_startup, tshell uses the serial port to receive interrupts,
 * and the interrupt stack and thread stack are used separately.
 *
 * @return  none
 */
int main (void) {
    rt_kprintf ("\r\n MCU: CH32V307\r\n");
    SystemCoreClockUpdate();
    rt_kprintf (" SysClk: %dHz\r\n", SystemCoreClock);
    rt_kprintf (" ChipID: %08x\r\n", DBGMCU_GetCHIPID());
    rt_kprintf (" www.wch.cn\r\n");
    LOG_I ("main_SP:%08lx\r\n", __get_SP());
    while (1) {
        if (uart2.finishFlag == 1) {
            rt_thread_resume (wifi);
        }
        rt_thread_mdelay (10);
    }
}
