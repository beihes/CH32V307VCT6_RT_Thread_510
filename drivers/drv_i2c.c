/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-4-1       IceBear003   the first version
 */

#include "board.h"
#include "drv_i2c.h"

#ifdef BSP_USING_HWI2C

#define LOG_TAG "drv.hwi2c"
#include "drv_log.h"

#if !defined(BSP_USING_I2C1) && !defined(BSP_USING_I2C2)
#error "Please define at least one BSP_USING_I2Cx"
/* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

#define HWI2C_TIMEOUT 0xFFF

static struct ch32_i2c_device ch32I2CDevice[] = {
#ifdef BSP_USING_I2C1
    CH32_I2C1_DEVICE,
#endif
#ifdef BSP_USING_I2C2
    CH32_I2C2_DEVICE,
#endif
};

int rt_hw_i2c_gpio_init (struct ch32_i2c_device *device) {
    rt_err_t result = RT_EOK;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if (device->i2cx == I2C1) {
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1, ENABLE);
        // SCL
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_Init (GPIOB, &GPIO_InitStructure);
        // SDA
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_Init (GPIOB, &GPIO_InitStructure);
    } else if (device->i2cx == I2C2) {
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C2, ENABLE);
        // SCL
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init (GPIOB, &GPIO_InitStructure);
        // SDA
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init (GPIOB, &GPIO_InitStructure);
    }
    I2C_InitTSturcture.I2C_ClockSpeed = device->config.clockSpeed;
    I2C_InitTSturcture.I2C_Mode = device->config.mode;
    I2C_InitTSturcture.I2C_DutyCycle = device->config.dutyCycle;
    I2C_InitTSturcture.I2C_OwnAddress1 = device->config.ownAddress;
    I2C_InitTSturcture.I2C_Ack = device->config.isEnableAck ? I2C_Ack_Enable : I2C_Ack_Disable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = device->config.is7BitAddress ? I2C_AcknowledgedAddress_7bit : I2C_AcknowledgedAddress_10bit;
    I2C_Init (device->i2cx, &I2C_InitTSturcture);
    I2C_Cmd (device->i2cx, ENABLE);
    return result;
}

rt_err_t rt_hw_i2c_readOneByte_start (I2C_TypeDef *i2cx, rt_uint8_t driverAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    while (I2C_GetFlagStatus (i2cx, I2C_FLAG_BUSY) != RESET) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_GenerateSTART (i2cx, ENABLE);
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_Send7bitAddress (i2cx, driverAddr, I2C_Direction_Transmitter);
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

rt_err_t rt_hw_i2c_readOneByte_8bit (I2C_TypeDef *i2cx, rt_uint16_t readAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    I2C_SendData (i2cx, (uint8_t)(readAddr & 0x00FF));
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

rt_err_t rt_hw_i2c_readOneByte_16bit (I2C_TypeDef *i2cx, rt_uint16_t readAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    I2C_SendData (i2cx, (uint8_t)(readAddr >> 8));
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_SendData (i2cx, (uint8_t)(readAddr & 0x00FF));
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

uint8_t rt_hw_i2c_readOneByte_end (I2C_TypeDef *i2cx, rt_uint8_t driverAddr) {
    uint8_t temp = 0;
    rt_uint16_t midTime = 0;
    I2C_GenerateSTART (i2cx, ENABLE);
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return temp;
        }
    }
    I2C_Send7bitAddress (i2cx, driverAddr, I2C_Direction_Receiver);
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return temp;
        }
    }
    while (I2C_GetFlagStatus (i2cx, I2C_FLAG_RXNE) == RESET)
        I2C_AcknowledgeConfig (i2cx, DISABLE);
    temp = I2C_ReceiveData (i2cx);
    I2C_GenerateSTOP (i2cx, ENABLE);
    return temp;
}

rt_err_t rt_hw_i2c_writeOneByte_start (I2C_TypeDef *i2cx, rt_uint8_t driverAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    while (I2C_GetFlagStatus (i2cx, I2C_FLAG_BUSY) != RESET) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_GenerateSTART (i2cx, ENABLE);
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_Send7bitAddress (i2cx, driverAddr, I2C_Direction_Transmitter);
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

rt_err_t rt_hw_i2c_writeOneByte_8bit (I2C_TypeDef *i2cx, rt_uint16_t writeAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    I2C_SendData (i2cx, (uint8_t)(writeAddr & 0x00FF));
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

rt_err_t rt_hw_i2c_writeOneByte_16bit (I2C_TypeDef *i2cx, rt_uint16_t writeAddr) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    I2C_SendData (i2cx, (uint8_t)(writeAddr >> 8));
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_SendData (i2cx, (uint8_t)(writeAddr & 0x00FF));
    midTime = 0;
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    return result;
}

rt_err_t rt_hw_i2c_writeOneByte_end (I2C_TypeDef *i2cx, rt_uint8_t dataToWrite) {
    rt_err_t result = RT_EOK;
    rt_uint16_t midTime = 0;
    if (I2C_GetFlagStatus (i2cx, I2C_FLAG_TXE) != RESET) {
        I2C_SendData (i2cx, dataToWrite);
    }
    while (!I2C_CheckEvent (i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if (midTime++ == HWI2C_TIMEOUT) {
            return result = -RT_ERROR;
        }
    }
    I2C_GenerateSTOP (i2cx, ENABLE);
    return result;
}

static rt_err_t i2c_read (ch32_i2c_device_t device, rt_uint16_t readAddr, rt_uint8_t flags, rt_uint16_t len, rt_uint8_t *buf) {
    rt_err_t result = RT_EOK;
    rt_uint8_t *midBuf = buf;
    while (len--) {
        result = rt_hw_i2c_readOneByte_start (device->i2cx, device->deviceAddr);
        if (flags & RT_I2C_ADDR_10BIT) {
            result = rt_hw_i2c_readOneByte_16bit (device->i2cx, readAddr);
        } else {
            result = rt_hw_i2c_readOneByte_8bit (device->i2cx, readAddr);
        }
        midBuf[0] = rt_hw_i2c_readOneByte_end (device->i2cx, device->deviceAddr);
        // rt_kprintf ("read:%04x:%x\n", len, midBuf[0]);
        midBuf++;
        readAddr++;
    }
    return result;
}

static rt_err_t i2c_write (ch32_i2c_device_t device, rt_uint16_t writeAddr, rt_uint8_t flags, rt_uint8_t *buf, rt_uint16_t len) {
    rt_err_t result = RT_EOK;
    rt_uint8_t *midBuf = buf;
    while (len--) {
        result = rt_hw_i2c_writeOneByte_start (device->i2cx, device->deviceAddr);
        if (flags & RT_I2C_ADDR_10BIT) {
            result = rt_hw_i2c_writeOneByte_16bit (device->i2cx, writeAddr);
        } else {
            result = rt_hw_i2c_writeOneByte_8bit (device->i2cx, writeAddr);
        }
        result = rt_hw_i2c_writeOneByte_end (device->i2cx, midBuf[0]);
        // rt_kprintf ("write:%04x:%x\n", len, midBuf[0]);
        midBuf++;
        writeAddr++;
        rt_thread_mdelay (4);
    }
    return result;
}

rt_ssize_t ch32_i2c_master_xfer (struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num) {
    struct rt_i2c_msg *msg;
    ch32_i2c_device_t i2cBusDev;
    rt_uint32_t index;

    i2cBusDev = (ch32_i2c_device_t)bus->priv;

    for (index = 0; index < num; index++) {
        msg = &msgs[index];
        if (msg->flags & RT_I2C_RD) {
            i2c_read (i2cBusDev, msg->addr, msg->flags, msg->len, msg->buf);
        } else {
            i2c_write (i2cBusDev, msg->addr, msg->flags, msg->buf, msg->len);
        }
    }
    return index;
}

rt_err_t ch32_i2c_bus_control (struct rt_i2c_bus_device *bus, int cmd, void *args) {
    rt_err_t result = RT_EOK;
    return result;
}

static const struct rt_i2c_bus_device_ops ch32_i2c_ops =
    {
        .master_xfer = ch32_i2c_master_xfer,  // 主机传输函数：必须实现
        .slave_xfer = RT_NULL,                // 不实现从机模式（通常用不到）
        .i2c_bus_control = RT_NULL            // 控制函数（例如切速率、复位）可选
};


static struct rt_i2c_bus_device i2cBusDevice[sizeof (ch32I2CDevice) / sizeof (struct ch32_i2c_device)] = {0};

int rt_hw_i2c_init (void) {
    rt_err_t result = RT_EOK;
    for (int i = 0; i < sizeof (ch32I2CDevice) / sizeof (struct ch32_i2c_device); i++) {
        result = rt_hw_i2c_gpio_init (&ch32I2CDevice[i]);
        RT_ASSERT (result == RT_EOK);
        i2cBusDevice[i].ops = &ch32_i2c_ops;
        i2cBusDevice[i].timeout = HWI2C_TIMEOUT;
        i2cBusDevice[i].priv = &ch32I2CDevice[i];
        result = rt_i2c_bus_device_register (&i2cBusDevice[i], ch32I2CDevice[i].name);
        RT_ASSERT (result == RT_EOK);
    }
    return result;
}

INIT_BOARD_EXPORT (rt_hw_i2c_init);

#endif  // BSP_USING_HWI2C
