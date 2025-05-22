/*
 * File      : eeprom.h
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
#include "eeprom.h"
#include <drv_i2c.h>

void ATC24C32_Test() {
}

MSH_CMD_EXPORT (ATC24C32_Test, i2c1 驱动 ATC24C32 的测试);