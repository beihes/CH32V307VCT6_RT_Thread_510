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
#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

#define AT24C32_ADDR 0xA0

#define EEPROM_USE_I2C "i2c2Soft"

int EEPROM_Init();

#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H */
