#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

// 系统时钟初始化方法声明区域

void System_Clock_Config(void);

// 系统相关通用方法与定义声明区域

#define SYSTEM_VERSION 0x01
#define HARDWARE_DEBUG 1
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

void System_GPIO_CLK_Config(GPIO_TypeDef  *_group);
void System_GPIO_Init(GPIO_TypeDef  *_group, uint32_t _pin, uint32_t _mode, uint32_t _pull, uint32_t _speed);
void System_GPIO_Alt_Init(GPIO_TypeDef  *_group, uint32_t _pin, uint32_t _mode, uint32_t _pull, uint32_t _speed, uint32_t _alt);
void System_GPIO_Write(GPIO_TypeDef  *_group, uint32_t _pin, uint8_t _level);
uint8_t System_GPIO_Read(GPIO_TypeDef  *_group, uint32_t _pin);

#endif
