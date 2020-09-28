#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "system.h"

// 看门狗相关常量声明区域

#define WATCHDOG_RESET_DURATION 4000

// 看门狗数据与方法声明区域

extern IWDG_HandleTypeDef hiwdg;

void Watchdog_Init(void);
void Watchdog_Feed(void);

#endif
