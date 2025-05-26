/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author        Notes
 * 2021-08-10   charlown      first version
 * 2022-09-22   hg0720        the first version which add from wch
 */

#ifndef DRV_FONT_H__
#define DRV_FONT_H__

#include <stdio.h>

#define ASCII12X06
#define ASCII16X08
#define ASCII24X12

extern const uint8_t ascii12X06[95][12];
extern const uint8_t ascii16X08[95][16];
extern const uint8_t ascii24X12[95][24];

#endif