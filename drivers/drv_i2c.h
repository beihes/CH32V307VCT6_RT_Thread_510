/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-4-1       IceBear003   the first version
 */

#ifndef __DRV_I2C_H_
#define __DRV_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include <rtdevice.h>
#include <ch32v30x.h>

#ifdef BSP_USING_HWI2C

#define BSP_I2C1_Remap 1

#define HWI2C_TIMEOUT 0xFF

typedef struct I2CData {
    struct rt_i2c_bus_device *bus;
    struct rt_i2c_msg msg;
} I2CData;

struct i2c_config {
    rt_uint32_t clockSpeed;    // I2C 时钟频率
    rt_uint16_t mode;          // 工作模式（主从机、I2C/SPI 模式等）
    rt_uint16_t dutyCycle;     // 占空比（标准或快速模式）
    rt_uint16_t ownAddress;    // 设备自身地址
    rt_uint8_t isEnableAck;    // 是否使能应答
    rt_uint8_t is7BitAddress;  // 使用 7 位地址（否则为 10 位）
};

struct ch32_i2c_device {
    struct i2c_config config;  // 配置参数
    I2C_TypeDef *i2cx;         // 硬件外设指针（I2C1, I2C2）
    rt_tick_t timeOut;         // 超时时间
    const char *name;          // 设备名称，用于注册到 RT-Thread
};

typedef struct ch32_i2c_device *ch32_i2c_device_t;

#ifdef BSP_USING_I2C1
#define CH32_I2C1_DEVICE                            \
    {                                               \
        .config = {                                 \
            .clockSpeed = 400000,                   \
            .mode = I2C_Mode_I2C,                   \
            .dutyCycle = I2C_DutyCycle_2,           \
            .ownAddress = 0x0,                      \
            .isEnableAck = ENABLE,                  \
            .is7BitAddress = ENABLE},               \
        .timeOut = 1, .i2cx = I2C1, .name = "i2c1", \
    }
#endif
#ifdef BSP_USING_I2C2
#define CH32_I2C2_DEVICE                            \
    {                                               \
        .config = {                                 \
            .clockSpeed = 400000,                   \
            .mode = I2C_Mode_I2C,                   \
            .dutyCycle = I2C_DutyCycle_2,           \
            .ownAddress = 0xA0,                     \
            .isEnableAck = ENABLE,                  \
            .is7BitAddress = ENABLE},               \
        .timeOut = 1, .i2cx = I2C2, .name = "i2c2", \
    }
#endif

rt_err_t rt_hw_i2c_read (ch32_i2c_device_t device, struct rt_i2c_msg *msg);
rt_err_t rt_hw_i2c_write (ch32_i2c_device_t device, struct rt_i2c_msg *msg);
int rt_hw_i2c_init (void);
#endif

#ifdef __cplusplus
}
#endif

#endif
