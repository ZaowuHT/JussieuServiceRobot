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
	while(1)
	{
		// Feed watchdog
		Watchdog_Feed();

		// Check battery
		osMutexAcquire(batteryDataMutexHandle, osWaitForever);
		if (batteryData.batteryState == BATT_LOW_LEVEL) Beep_Start(BATT_LOW_BEEP_DURATION);
		osMutexRelease(batteryDataMutexHandle);

		osDelay(OBSERVER_REFRESH_DURATION);
	}
}
