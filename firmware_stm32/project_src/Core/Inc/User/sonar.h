#ifndef SONAR_H
#define SONAR_H

#include "system.h"

// 超声波传感器相关常量声明区域

#define SONAR_NUMBER 2

#define SONAR_CHANNEL 6
#define SONAR_ROUTER_S0_GPIO_GROUP GPIOE
#define SONAR_ROUTER_S0_GPIO_ID GPIO_PIN_1
#define SONAR_ROUTER_S1_GPIO_GROUP GPIOE
#define SONAR_ROUTER_S1_GPIO_ID GPIO_PIN_2
#define SONAR_ROUTER_S2_GPIO_GROUP GPIOE
#define SONAR_ROUTER_S2_GPIO_ID GPIO_PIN_3
#define SONAR_ROUTER_S3_GPIO_GROUP GPIOE
#define SONAR_ROUTER_S3_GPIO_ID GPIO_PIN_4

#define SONAR_TX_GPIO_GROUP GPIOA
#define SONAR_TX_GPIO_ID GPIO_PIN_0
#define SONAR_RX_GPIO_GROUP GPIOA
#define SONAR_RX_GPIO_ID GPIO_PIN_1
#define UART4_STREAM_HANDLER(x) Sonar_DataHandler(x)
#define UART4_STREAM_BUFFER sonarRecieveBuffer

#define SONAR_TRIGGER 0xff
#define SONAR_FRAME_HEAD 0xff
#define SONAR_FRAME_PAYLOAD_LENGHT 3
#define SONAR_UART_BAUD 9600L
#define SONAR_RECIEVE_BUFFER_SIZE 8
#define SONAR_STATE_WAITHEAD 0
#define SONAR_STATE_WAITPAYLOAD 1
#define SONAR_TIMEOUT 300

#define SONAR_REFRESH_DURATION 100

// UART相关声明区域: UART4

extern UART_HandleTypeDef huart4;
extern uint8_t sonarRecieveBuffer[SONAR_RECIEVE_BUFFER_SIZE];

// 多路复用器相关声明区域

void Sonar_Multiplexer_Init(void);
void Sonar_Multiplexer_SetChannel(uint8_t channelID);

// 超声波传感器数据与方法声明区域

typedef int32_t SonarData[SONAR_NUMBER];

extern uint8_t sonarIter;
extern uint8_t sonarState;
extern SonarData sonarData;

void Sonar_Init(void);
void Sonar_SendByte(uint8_t data);
void Sonar_DataHandler(uint8_t* dataStream);

// CMSIS-RTOS 声明区域

extern osThreadId_t sonarTaskHandle;
extern const osThreadAttr_t sonarTask_attributes;

extern osMutexId_t sonarDataMutexHandle;
extern const osMutexAttr_t sonarDataMutex_attributes;

extern osMessageQueueId_t sonarDataQueueHandle;
extern const osMessageQueueAttr_t sonarDataQueue_attributes;

void Sonar_RunTask(void);
void Sonar_Task(void *argument);

#endif
