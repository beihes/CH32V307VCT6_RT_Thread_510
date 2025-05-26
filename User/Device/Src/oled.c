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
#include "oled.h"
#include <rtthread.h>
#include <drv_font.h>
#include <drv_i2c.h>
#include <rtdevice.h>

struct rt_i2c_bus_device *oledDevice = RT_NULL;

static rt_uint8_t midOledWrData[2] = {0};

void OLED_WriteOneByte (rt_uint8_t writeAddr, rt_uint8_t dataToWrite) {
    midOledWrData[0] = writeAddr;
    midOledWrData[1] = dataToWrite;
    rt_i2c_master_send (oledDevice, OLED_ADDRESS, RT_I2C_WR, midOledWrData, 2);
}

/**
 * @brief  OLED设置光标位置
 * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
 * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
 * @retval 无
 */
void OLED_SetCursor (uint8_t Y, uint8_t X) {
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xB0 | Y);                  // 设置Y位置
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x10 | ((X & 0xF0) >> 4));  // 设置X位置高4位
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x00 | (X & 0x0F));         // 设置X位置低4位
}

/**
 * @brief  OLED清屏
 * @param  无
 * @retval 无
 */
void OLED_Clear (void) {
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor (j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteOneByte (OLED_WRITE_ADDRESS, 0x00);
        }
    }
}

/**
 * @brief  OLED显示一个字符
 * @param  Line 行位置，范围：1~4
 * @param  Column 列位置，范围：1~16
 * @param  Char 要显示的一个字符，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowChar (uint8_t Line, uint8_t Column, char Char) {
    uint8_t i;
    OLED_SetCursor ((Line - 1) * 2, (Column - 1) * 8);                          // 设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteOneByte (OLED_WRITE_ADDRESS, ascii16X08[Char - ' '][i]);      // 显示上半部分内容
    }
    OLED_SetCursor ((Line - 1) * 2 + 1, (Column - 1) * 8);                      // 设置光标位置在下半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteOneByte (OLED_WRITE_ADDRESS, ascii16X08[Char - ' '][i + 8]);  // 显示下半部分内容
    }
}

/**
 * @brief  OLED显示字符串
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  String 要显示的字符串，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowString (uint8_t Line, uint8_t Column, char *String) {
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar (Line, Column + i, String[i]);
    }
}

/**
 * @brief  OLED次方函数
 * @retval 返回值等于X的Y次方
 */
uint32_t OLED_Pow (uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  OLED显示数字（十进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~4294967295
 * @param  Length 要显示数字的长度，范围：1~10
 * @retval 无
 */
void OLED_ShowNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar (Line, Column + i, Number / OLED_Pow (10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED显示数字（十进制，带符号数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：-2147483648~2147483647
 * @param  Length 要显示数字的长度，范围：1~10
 * @retval 无
 */
void OLED_ShowSignedNum (uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length) {
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar (Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar (Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar (Line, Column + i + 1, Number1 / OLED_Pow (10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED显示数字（十六进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
 * @param  Length 要显示数字的长度，范围：1~8
 * @retval 无
 */
void OLED_ShowHexNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow (16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar (Line, Column + i, SingleNumber + '0');
        } else {
            OLED_ShowChar (Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief  OLED显示数字（二进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
 * @param  Length 要显示数字的长度，范围：1~16
 * @retval 无
 */
void OLED_ShowBinNum (uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar (Line, Column + i, Number / OLED_Pow (2, Length - i - 1) % 2 + '0');
    }
}

/**
 * @brief  OLED初始化
 * @param  无
 * @retval 无
 */
int OLED_Init() {
    rt_err_t result = RT_EOK;
    oledDevice = (struct rt_i2c_bus_device *)rt_device_find (EEPROM_USE_I2C);
    if (oledDevice == RT_NULL) {
        rt_kprintf ("can't find %s\n", EEPROM_USE_I2C);
        return -RT_ERROR;
    }
    rt_device_open ((rt_device_t)oledDevice, RT_DEVICE_FLAG_RDWR);

    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xAE);  // 关闭显示
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xD5);  // 设置显示时钟分频比/振荡器频率
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x80);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xA8);  // 设置多路复用率
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x3F);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xD3);  // 设置显示偏移
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x00);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x40);  // 设置显示开始行
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xA1);  // 设置左右方向，0xA1正常 0xA0左右反置
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xC8);  // 设置上下方向，0xC8正常 0xC0上下反置
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xDA);  // 设置COM引脚硬件配置
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x12);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x81);  // 设置对比度控制
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xCF);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xD9);  // 设置预充电周期
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xF1);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xDB);  // 设置VCOMH取消选择级别
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x30);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xA4);  // 设置整个显示打开/关闭
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xA6);  // 设置正常/倒转显示
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x8D);  // 设置充电泵
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x14);
    // OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xAF);  // 开启显示

    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xAE);  // display off
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x20);  // Set Memory Addressing Mode
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xc8);  // Set COM Output Scan Direction
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x00);  //---set low column address
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x10);  //---set high column address
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x40);  //--set start line address
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x81);  //--set contrast control register
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xff);  // brightness 0x00~0xff
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xa1);  //--set segment re-map 0 to 127
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xa6);  //--set normal display
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xa8);  //--set multiplex ratio(1 to 64)
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x3F);  //
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xa4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xd3);  //-set display offset
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x00);  //-not offset
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xd5);  //--set display clock divide ratio/oscillator frequency
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xf0);  //--set divide ratio
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xd9);  //--set pre-charge period
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x22);  //
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xda);  //--set com pins hardware configuration
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x12);
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xdb);  //--set vcomh
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x20);  // 0x20,0.77xVcc
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x8d);  //--set DC-DC enable
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0x14);  //
    OLED_WriteOneByte (OLED_COMMAND_ADDRESS, 0xaf);  //--turn on oled panel

    OLED_Clear();                                    // OLED清屏
    return result;
}

INIT_ENV_EXPORT (OLED_Init);
