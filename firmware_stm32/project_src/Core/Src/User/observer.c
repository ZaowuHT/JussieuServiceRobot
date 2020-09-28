#include "observer.h"

osThreadId_t observerTaskHandle;
const osThreadAttr_t observerTask_attributes = {
	.name = "observerTask",
	.priority = (osPriority_t) osPriorityLow,
	.stack_size = 512 * 4
};

void Observer_RunTask(void)
{
	observerTaskHandle = 
		osThreadNew(Observer_Task, NULL, &observerTask_attributes);
}

void Observer_Task(void *argument)
{
	uint8_t _servoAngle = 90;
	uint32_t _beepDuration = 0;
	while(1)
	{
		// Feed watchdog
		Watchdog_Feed();

		// Kinematic test
		Kinematic_SetSpeed(50, 0);
		Kinematic_UpdateDisplacement();
		Host_SendStream((uint8_t*) "KINEMATIC: ", 11);
		Host_SendNumber((int32_t) robotDisplacement.displacementX);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber((int32_t) robotDisplacement.displacementY);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber((int32_t) robotDisplacement.displacementYaw);
		Host_SendStream((uint8_t*) "\n", 1);
		Kinematic_ClearDisplacement();

		// Motor test
		osMutexAcquire(motorDataMutexHandle, osWaitForever);
		Host_SendStream((uint8_t*) "MOTOR_LEFT: ", 12);
		Host_SendNumber(robotMotorData[MOTOR_LEFT].currentPWM);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_LEFT].targetSpeedPPS);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_LEFT].currentSpeedPPS);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_LEFT].currentSpeedRPM);
		Host_SendStream((uint8_t*) "\n", 1);
		Host_SendStream((uint8_t*) "MOTOR_RIGHT: ", 13);
		Host_SendNumber(robotMotorData[MOTOR_RIGHT].currentPWM);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_RIGHT].targetSpeedPPS);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_RIGHT].currentSpeedPPS);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(robotMotorData[MOTOR_RIGHT].currentSpeedRPM);
		Host_SendStream((uint8_t*) "\n", 1);
		osMutexRelease(motorDataMutexHandle);

		// Servo test
		for (int i = 0; i < SERVO_NUMBER; i++)
		{
			_servoAngle = _servoAngle + 15 > 180 ? 180 : _servoAngle + 15;
			Servo_SetAngle(i, _servoAngle);
			osMutexAcquire(servoStateMutexHandle, osWaitForever);
			Host_SendStream((uint8_t*) "Servo: ", 7);
			Host_SendNumber(i);
			Host_SendStream((uint8_t*) " ", 1);
			Host_SendNumber(servoData[i]);
			Host_SendStream((uint8_t*) "\n", 1);
			osMutexRelease(servoStateMutexHandle);
		}

		// Beep test
		_beepDuration = _beepDuration + 100 > 1000 ? 1000 : _beepDuration + 100;
		Beep_Start(_beepDuration);

		// Battery test
		osMutexAcquire(batteryDataMutexHandle, osWaitForever);
		Host_SendStream((uint8_t*) "BATTERY: ", 9);
		Host_SendNumber(batteryData.batteryState);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(batteryData.batteryVoltageRaw);
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber((int32_t) (batteryData.batteryVoltage * 100));
		Host_SendStream((uint8_t*) " ", 1);
		Host_SendNumber(batteryData.batteryPercent);
		Host_SendStream((uint8_t*) "\n", 1);
		osMutexRelease(batteryDataMutexHandle);

		// Sonar test
		for (int i = 0; i < SONAR_NUMBER; i++)
		{
			osMutexAcquire(sonarDataMutexHandle, osWaitForever);
			Host_SendStream((uint8_t*) "SONAR: ", 7);
			Host_SendNumber(i);
			Host_SendStream((uint8_t*) " ", 1);
			Host_SendNumber(sonarData[i]);
			Host_SendStream((uint8_t*) "\n", 1);
			osMutexRelease(sonarDataMutexHandle);
		}

		osDelay(OBSERVER_REFRESH_DURATION);
	}
}
