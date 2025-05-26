/*
 * File      : serwa.h
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
#ifndef __SERWA_H
#define __SERWA_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SerwaDegreeType {
    DEGREE_0,
    DEGREE_45,
    DEGREE_90,
    DEGREE_135,
    DEGREE_180,
    SERWA_DEGREE_NUM,
} SerwaCmdType;

typedef enum ActiveLegType {
    SERWA_RIGHT_0,
    SERWA_RIGHT_1,
    SERWA_LEFT_0,
    SERWA_LEFT_1,
    ACTIVE_LEG_NUM,
} ActiveLegType;

typedef enum DogCmdType {
    STOP,
    FORWARD,
    RIGHT,
    BACK,
    LEFT,
    DOG_CMD_NUM,
} DogCmdType;

void Dog_Stop();
void Dog_Forward();
void Dog_Right();
void Dog_Back();
void Dog_Left();

int Serwa_Init();

#ifdef __cplusplus
}
#endif

#endif /* __SERWA_H */
