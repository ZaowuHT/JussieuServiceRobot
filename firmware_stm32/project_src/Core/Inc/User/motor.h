#ifndef MOTOR_H
#define MOTOR_H

#include "system.h"

// 电机相关常量声明区域

#define MOTOR_NUMBER 2

#define MOTOR_RIGHT 0
#define MOTOR_LEFT 1

#define MOTOR_R_PWM_GROUP GPIOA
#define MOTOR_R_PWM_GPIO GPIO_PIN_8
#define MOTOR_R_A_GROUP GPIOB
#define MOTOR_R_A_GPIO GPIO_PIN_6
#define MOTOR_R_B_GROUP GPIOB
#define MOTOR_R_B_GPIO GPIO_PIN_7
#define MOTOR_R_ENCA_GROUP GPIOA
#define MOTOR_R_ENCA_GPIO GPIO_PIN_15
#define MOTOR_R_ENCB_GROUP GPIOB
#define MOTOR_R_ENCB_GPIO GPIO_PIN_3

#define MOTOR_L_PWM_GROUP GPIOA
#define MOTOR_L_PWM_GPIO GPIO_PIN_11
#define MOTOR_L_A_GROUP GPIOB
#define MOTOR_L_A_GPIO GPIO_PIN_8
#define MOTOR_L_B_GROUP GPIOB
#define MOTOR_L_B_GPIO GPIO_PIN_9
#define MOTOR_L_ENCA_GROUP GPIOA
#define MOTOR_L_ENCA_GPIO GPIO_PIN_6
#define MOTOR_L_ENCB_GROUP GPIOA
#define MOTOR_L_ENCB_GPIO GPIO_PIN_7

#define MOTOR_MAX_PWM 8200
#define MOTOR_PID_KP 0.2f
#define MOTOR_PID_KI 0.1f
#define MOTOR_PPR 6600.0f
#define MOTOR_CONTROL_PERIOD 30

// 电机数据结构声明区域

struct MotorData
{
    int32_t currentSpeedPPS;
    int32_t currentSpeedRPM;
    int32_t targetSpeedPPS;
    int32_t encoderCountAbs;
    int32_t currentPWM;
    uint32_t encoderCountPeriod;
    int32_t lastError;
};

typedef struct MotorData RobotMotorData[MOTOR_NUMBER];

// 定时器相关声明区域: TIM1

extern TIM_HandleTypeDef htim1;

void Motor_Timer_Init(void);

// 旋转编码器相关声明区域：TIM2, TIM3

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void Motor_Encoder_Init(void);

// 电机数据与方法声明区域

extern RobotMotorData robotMotorData;

void Motor_Init(void);
void Motor_SetPWM(uint8_t _index, int32_t _value);
void Motor_ClearEncoder(uint8_t _index);
short Motor_ReadEncoder(uint8_t _index);

// CMSIS-RTOS 声明区域

extern osTimerId_t motorControlTaskHandle;
extern const osTimerAttr_t motorControlTask_attributes;

extern osMutexId_t motorDataMutexHandle;
extern const osMutexAttr_t motorDataMutexHandle_attributes;

void Motor_RunTask(void);
void Motor_SetSpeedPPS(uint8_t _index, int32_t _value);
void Motor_Task(void *argument);

#endif
