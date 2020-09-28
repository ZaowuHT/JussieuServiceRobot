#include "system.h"
#include "motor.h"
#include "host.h"
#include "observer.h"
#include "beep.h"
#include "servo.h"
#include "battery.h"
#include "sonar.h"

int main(void)
{
	// 固件与实时操作系统内核初始化
	HAL_Init();
	System_Clock_Config();
	osKernelInitialize();

	// 硬件外设初始化
	Host_Init();
	Motor_Init();
	Servo_Init();
	Beep_Init();
	Battery_Init();
	Sonar_Init();
	Watchdog_Init();

	// 实时操作系统任务初始化
	Motor_RunTask();
	Servo_RunTask();
	Beep_RunTask();
	Battery_RunTask();
	Sonar_RunTask();
	Observer_RunTask();

	// 实时操作系统任务调度器启动
	osKernelStart();
}
