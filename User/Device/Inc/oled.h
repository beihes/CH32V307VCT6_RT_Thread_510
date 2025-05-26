/*
 * File      : oled.h
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
#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>

#define EEPROM_USE_I2C "i2c2Soft"

#define OLED_COMMAND_ADDRESS 0x00
#define OLED_WRITE_ADDRESS 0x40
#define OLED_ADDRESS 0x78

void OLED_WriteOneByte (rt_uint8_t writeAddr, rt_uint8_t dataToWrite);
void OLED_Clear (void);
void OLED_ShowChar (uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString (uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum (uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

int OLED_Init();

#ifdef __cplusplus
}
#endif

#endif /* __OLED_H */