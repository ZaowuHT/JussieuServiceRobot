#ifndef HOST_H
#define HOST_H

#include "system.h"
#include "kinematic.h"
#include "streambuilder.h"
#include "motor.h"
#include "servo.h"
#include "battery.h"
#include "slamware.h"
#include "sonar.h"
#include "beep.h"

// Host相关常量声明区域

#define HOST_RECIEVE_BUFFER_SIZE 256
#define HOST_SEND_BUFFER_SIZE 256
#define HOST_USART_BAUD 115200L

#define HOST_TX_GPIO_GROUP GPIOA
#define HOST_TX_GPIO_ID GPIO_PIN_2
#define HOST_RX_GPIO_GROUP GPIOA
#define HOST_RX_GPIO_ID GPIO_PIN_3
#define USART2_STREAM_HANDLER(x) Host_DataHandler(x)
#define USART2_STREAM_BUFFER hostRecieveBuffer

#define HOST_STATE_DISCONNECTED 0
#define HOST_STATE_CONNECTED 1
#define HOST_STREAM_STATE_WAITFORHEAD 0
#define HOST_STREAM_STATE_WAITFORLENGTH 1
#define HOST_STREAM_STATE_WAITFORPAYLOAD 2

#define HOST_STREAM_HEAD 0x50
#define HOST_STREAM_CONNECT_REQ 0x01
#define HOST_STREAM_RESP_OK 0x20
#define HOST_STREAM_RESP_ERROR 0x21
#define HOST_STREAM_SPEED_REQ 0x02
#define HOST_STREAM_SERVO_REQ 0x03
#define HOST_STREAM_BATTERY_REQ 0x04
#define HOST_STREAM_SLAMCORE_STATE_REQ 0x05
#define HOST_STREAM_SONAR_REQ 0x07
#define HOST_STREAM_BEEP_REQ 0x08

#define HOST_STREAM_PTN_8 1
#define HOST_STREAM_PTN_16 2
#define HOST_STREAM_PTN_32 4

#define HOST_COMMUNICATION_TIMEOUT 2000 

// USART相关声明区域: USART2

extern UART_HandleTypeDef huart2;
extern uint8_t hostRecieveBuffer[HOST_RECIEVE_BUFFER_SIZE];
extern uint8_t hostSendBuffer[HOST_SEND_BUFFER_SIZE];

// 上位机控制数据与方法声明区域

extern uint8_t hostConnectionState;
extern uint8_t hostStreamState;

void Host_Init(void);
void Host_SendByte(uint8_t data);
void Host_SendStream(uint8_t* dataStream, uint16_t streamLength);
void Host_SendNumber(int32_t _value);
void Host_DataHandler(uint8_t* dataStream);
uint16_t Host_ProcessStream(uint8_t* dataStream, uint16_t streamLength, uint8_t builderType);

// CMSIS-RTOS 声明区域

extern osThreadId_t hostTaskHandle;
extern const osThreadAttr_t hostTask_attributes;

extern osMessageQueueId_t hostStreamQueueHandle;
extern const osMessageQueueAttr_t hostStreamQueue_attributes;

void Host_RunTask(void);
void Host_Task(void *argument);

#endif
