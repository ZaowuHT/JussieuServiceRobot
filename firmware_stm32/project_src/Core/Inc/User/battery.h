#ifndef BATTERY_H
#define BATTERY_H

#include "system.h"
#include "beep.h"

// 电池相关常量声明区域

#define BATT_FULL_LEVEL 12.1f
#define BATT_LOW_LEVEL 11.1f
#define BATT_K_VOL 6.4f
#define BATT_K_PERCENT 100.0
#define BATT_B_PERCENT 1110

#define BATT_ADC_REF 3300.0f
#define BATT_ADC_FULL_REF 1.9f
#define BATT_SAMPLE_PERIOD 5000
#define BATT_ADC_RESOLU 4096.0f

#define BATT_LOW_PERCENT 15
#define BATT_LOW_BEEP_DURATION 1000

#define BATT_STATE_NORMAL 0
#define BATT_STATE_LOW 1
#define BATT_STATE_CHARGING 2

// ADC相关常量声明区域

#define BATT_ADC_INPUT_GPIO_GROUP GPIOB
#define BATT_ADC_INPUT_GPIO_ID GPIO_PIN_0
#define BATT_ADC_TIMEOUT 20

// 电池数据结构声明区域

struct BatteryData
{
    uint8_t batteryState;
    float batteryVoltage;
    int32_t batteryVoltageRaw;
    int32_t batteryPercent;
};

// 模数转换相关声明区域: ADC1

extern ADC_HandleTypeDef hadc1;
void Battery_ADC_Init(void);

// 电池数据与方法声明区域

extern struct BatteryData batteryData;

void Battery_Init(void);
void Battery_UpdateData(void);

// CMSIS-RTOS 声明区域

extern osThreadId_t batterySamplingTaskHandle;
extern const osThreadAttr_t batterySamplingTask_attributes;

extern osMutexId_t batteryDataMutexHandle;
extern const osMutexAttr_t batteryDataMutex_attributes;

void Battery_RunTask(void);
void Battery_Task(void *argument);

#endif
