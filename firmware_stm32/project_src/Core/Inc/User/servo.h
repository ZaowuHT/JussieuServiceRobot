#ifndef SERVO_H
#define SERVO_H

#include "system.h"

// 舵机相关常量声明区域

#define SERVO_NUMBER 4
#define SERVO_DEFAULT_ANGLE 90
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_ACTION_DURATION 10
#define SERVO_TIMEOUT 1000

#define SERVO_USART_BAUD 9600L

#define SERVO_TX_GPIO_GROUP GPIOA
#define SERVO_TX_GPIO_ID GPIO_PIN_9
#define SERVO_RX_GPIO_GROUP GPIOA
#define SERVO_RX_GPIO_ID GPIO_PIN_10

// USART相关声明区域: USART1

extern UART_HandleTypeDef huart1;

// 舵机数据结构声明区域

struct ServoAction
{
    uint8_t servoID;
    uint8_t servoAngle;
};

// 舵机数据与方法声明区域

typedef uint8_t ServoData[SERVO_NUMBER];

extern ServoData servoData;

void Servo_Init(void);
void Servo_SendByte(uint8_t data);
void Servo_SendControlStream(uint8_t servoID, uint8_t servoAngle);
void Servo_SetAngle(uint8_t servoID, uint8_t servoAngle);

// CMSIS-RTOS 声明区域

extern osThreadId_t servoTaskHandle;
extern const osThreadAttr_t servoTask_attributes;

extern osMutexId_t servoStateMutexHandle;
extern const osMutexAttr_t servoStateMutex_attributes;

extern osMessageQueueId_t servoDataQueueHandle;
extern const osMessageQueueAttr_t servoDataQueue_attributes;

void Servo_RunTask(void);
void Servo_Task(void *argument);

#endif
