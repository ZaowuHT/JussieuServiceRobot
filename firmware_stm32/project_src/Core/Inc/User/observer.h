#ifndef OBSERVER_H
#define OBSERVER_H

#include "system.h"
#include "battery.h"
#include "beep.h"
#include "host.h"
#include "motor.h"
#include "sonar.h"
#include "watchdog.h"
#include "servo.h"
#include "kinematic.h"

// 观察者进程相关常量声明区域

#define OBSERVER_REFRESH_DURATION 2000

// CMSIS-RTOS 声明区域

extern osThreadId_t observerTaskHandle;
extern const osThreadAttr_t observerTask_attributes;

void Observer_RunTask(void);
void Observer_Task(void *argument);

#endif
