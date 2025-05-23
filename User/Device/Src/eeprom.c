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
#include <rtthread.h>

void ATC24C32_Test() {
    struct rt_i2c_bus_device *bus;
    struct rt_i2c_msg msgs[2] = {0};
    char writeData[] = "CH32V307VCT6";
    char readData[sizeof (writeData)] = "";
    // 获取 I2C 总线设备
    bus = (struct rt_i2c_bus_device *)rt_device_find ("i2c2");
    if (bus == RT_NULL) {
        rt_kprintf ("can't find i2c2\n");
        return;
    }
    // 1. 写寄存器地址
    msgs[0].addr = 100;                             // 从设备写地址
    msgs[0].flags = RT_I2C_WR | RT_I2C_ADDR_10BIT;  // 写
    msgs[0].buf = (rt_uint8_t *)&writeData;         // 寄存器地址
    msgs[0].len = sizeof (writeData);               // 数据长度
    // 2. 读寄存器数据
    msgs[1].addr = 100;
    msgs[1].flags = RT_I2C_RD | RT_I2C_ADDR_10BIT;  // 读
    msgs[1].buf = (rt_uint8_t *)&readData;
    msgs[1].len = sizeof (writeData);
    // 执行读操作（写后读）
    if (rt_i2c_transfer (bus, msgs, 2) == 2) {
        rt_kprintf ("char\tdata: %s\n", readData);
        rt_kprintf ("hex\tdata: ");
        for (int i = 0; i < sizeof (writeData); i++) {
            rt_kprintf ("%x ", readData[i]);
        }
        rt_kprintf ("\n");
    } else {
        rt_kprintf ("read failed\n");
    }
}

MSH_CMD_EXPORT (ATC24C32_Test, i2c1 驱动 ATC24C32 的测试);