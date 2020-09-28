#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "system.h"
#include "motor.h"

// 底盘运动学相关常量声明区域

#define PI 3.1415926f
#define KINEMATIC_CHASSIS_RADIUS 190.22f
#define KINEMATIC_WHEEL_RADIUS 32.5f
#define KINEMATIC_CHASSIS_CONFIG_SIZE 331

// 底盘运动学数据结构声明区域
struct RobotDisplacement
{
    float displacementX;
    float displacementY;
    float displacementYaw;
};

// 底盘运动学数据与方法声明区域

extern uint8_t Kinematic_ChassisConfigBinary[KINEMATIC_CHASSIS_CONFIG_SIZE];
extern struct RobotDisplacement robotDisplacement;

void Kinematic_SetSpeed(float speedX, float speedYaw);
void Kinematic_UpdateDisplacement(void);
void Kinematic_ClearDisplacement(void);

#endif
