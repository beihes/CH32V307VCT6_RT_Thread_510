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

struct rt_i2c_bus_device *eepromDevice = RT_NULL;

rt_err_t at24c32_write (struct rt_i2c_bus_device *i2cBusDevice, rt_uint16_t mem_addr, rt_uint8_t data) {
    rt_uint8_t buf[3];
    buf[0] = (mem_addr >> 8) & 0xFF;  // 高 8 位地址
    buf[1] = mem_addr & 0xFF;         // 低 8 位地址
    buf[2] = data;
    return rt_i2c_master_send (i2cBusDevice, AT24C32_ADDR, RT_I2C_WR, buf, 3);
}

rt_err_t at24c32_read (struct rt_i2c_bus_device *i2cBusDevice, rt_uint16_t mem_addr, rt_uint8_t *data) {
    rt_uint8_t addr_buf[2];
    addr_buf[0] = (mem_addr >> 8) & 0xFF;
    addr_buf[1] = mem_addr & 0xFF;
    // 第一步：发送地址
    if (rt_i2c_master_send (i2cBusDevice, AT24C32_ADDR, RT_I2C_WR, addr_buf, 2) != 2)
        return -RT_ERROR;
    // 第二步：读取数据
    if (rt_i2c_master_recv (i2cBusDevice, AT24C32_ADDR, RT_I2C_RD, data, 1) != 1)
        return -RT_ERROR;
    return RT_EOK;
}

void at24c32_write_str (struct rt_i2c_bus_device *i2cBusDevice, rt_uint16_t mem_addr, rt_uint8_t *writeBuffer, rt_uint16_t len) {
    rt_uint8_t *midBuffer = writeBuffer;
    while (len) {
        at24c32_write (i2cBusDevice, mem_addr, midBuffer[0]);
        mem_addr++;
        midBuffer++;
        len--;
    }
}

void at24c32_read_str (struct rt_i2c_bus_device *i2cBusDevice, rt_uint16_t mem_addr, rt_uint8_t *readBuffer, rt_uint16_t len) {
    rt_uint8_t *midBuffer = readBuffer;
    while (len) {
        at24c32_read (i2cBusDevice, mem_addr, midBuffer);
        mem_addr++;
        midBuffer++;
        len--;
    }
}

int EEPROM_Init() {
    rt_err_t result = RT_EOK;
    eepromDevice = (struct rt_i2c_bus_device *)rt_device_find (EEPROM_USE_I2C);
    if (eepromDevice == RT_NULL) {
        rt_kprintf ("can't find %s\n", EEPROM_USE_I2C);
        return -RT_ERROR;
    }
    rt_device_open ((rt_device_t)eepromDevice, RT_DEVICE_FLAG_RDWR);
    return result;
}

INIT_ENV_EXPORT (EEPROM_Init);

void ATC24C32_Test() {
    // rt_uint16_t memAddr = 100;
    // rt_uint8_t midWriteData[] = "CH32V307VCT6";
    // rt_uint8_t midReadData[sizeof (midWriteData)] = "";
    // at24c32_write_str (eepromDevice, memAddr, midWriteData, sizeof (midWriteData));
    // at24c32_write_str (eepromDevice, memAddr, midReadData, sizeof (midWriteData));
    // rt_kprintf ("readBuffer:%s\n", midReadData);

    // struct rt_i2c_bus_device *bus;
    // struct rt_i2c_msg msgs[2] = {0};
    // char writeData[] = "CH32V307VCT6";
    // char readData[sizeof (writeData)] = "";
    // // 获取 I2C 总线设备
    // bus = (struct rt_i2c_bus_device *)rt_device_find ("i2c2");
    // if (bus == RT_NULL) {
    //     rt_kprintf ("can't find i2c2\n");
    //     return;
    // }
    // // 1. 写寄存器地址
    // msgs[0].addr = 100;                             // 寄存器地址
    // msgs[0].flags = RT_I2C_WR | RT_I2C_ADDR_10BIT;  // 写
    // msgs[0].buf = (rt_uint8_t *)&writeData;         // 寄存器地址
    // msgs[0].len = sizeof (writeData);               // 数据长度
    // // 2. 读寄存器数据
    // msgs[1].addr = 100;
    // msgs[1].flags = RT_I2C_RD | RT_I2C_ADDR_10BIT;  // 读
    // msgs[1].buf = (rt_uint8_t *)&readData;
    // msgs[1].len = sizeof (writeData);
    // // 执行读操作（写后读）
    // if (rt_i2c_transfer (bus, msgs, 2) == 2) {
    //     rt_kprintf ("char\tdata: %s\n", readData);
    //     rt_kprintf ("hex\tdata: ");
    //     for (int i = 0; i < sizeof (writeData); i++) {
    //         rt_kprintf ("%x ", readData[i]);
    //     }
    //     rt_kprintf ("\n");
    // } else {
    //     rt_kprintf ("read failed\n");
    // }
}

MSH_CMD_EXPORT (ATC24C32_Test, i2c1 驱动 ATC24C32 的测试);