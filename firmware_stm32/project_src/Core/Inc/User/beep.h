#ifndef BEEP_H
#define BEEP_H

#include "system.h"

// 蜂鸣器相关常量声明区域

#define BEEP_GPIO_GROUP GPIOB
#define BEEP_GPIO_ID GPIO_PIN_4

// 蜂鸣器数据与方法声明区域

extern uint32_t beepDuration;

void Beep_Init(void);
void Beep_Start(uint32_t beepDuration);

// CMSIS-RTOS 声明区域

extern osThreadId_t beepTaskHandle;
extern const osThreadAttr_t beepTask_attributes;

extern osMutexId_t beepStateMutexHandle;
extern const osMutexAttr_t beepStateMutex_attributes;

extern osMessageQueueId_t beepDurationQueueHandle;
extern const osMessageQueueAttr_t beepDurationQueue_attributes;

void Beep_RunTask(void);
void Beep_Task(void *argument);

#endif
