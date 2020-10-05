#include "beep.h"

uint32_t beepDuration = 0;

void Beep_Init(void)
{
	System_GPIO_Init(BEEP_GPIO_GROUP, BEEP_GPIO_ID,
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW);
}

void Beep_Start(uint32_t beepDuration)
{
	if (beepDuration > 0)
	{
		if (osMutexAcquire(beepStateMutexHandle, 0) == osOK)
		{
			osMessageQueuePut(beepDurationQueueHandle, &beepDuration, 0, 0);
			osMutexRelease(beepStateMutexHandle);
		}
	}
}

osThreadId_t beepTaskHandle;
const osThreadAttr_t beepTask_attributes = {
	.name = "beepTask",
	.priority = (osPriority_t) osPriorityLow1,
	.stack_size = 256 * 4
};

osMutexId_t beepStateMutexHandle;
const osMutexAttr_t beepStateMutex_attributes = {
	.name = "beepStateMutex"
};

osMessageQueueId_t beepDurationQueueHandle;
const osMessageQueueAttr_t beepDurationQueue_attributes = {
	.name = "beepDurationQueue"
};

void Beep_RunTask(void)
{
	beepStateMutexHandle = 
		osMutexNew(&beepStateMutex_attributes);
	beepDurationQueueHandle = 
		osMessageQueueNew (1, sizeof(uint32_t*), &beepDurationQueue_attributes);
	beepTaskHandle = 
		osThreadNew(Beep_Task, NULL, &beepTask_attributes);
}

void Beep_Task(void *argument)
{
	while(1)
	{
		if (osMessageQueueGet(beepDurationQueueHandle, &beepDuration, 0, osWaitForever) == osOK)
		{
			osMutexAcquire(beepStateMutexHandle, osWaitForever);
			System_GPIO_Write(BEEP_GPIO_GROUP, BEEP_GPIO_ID, HIGH);
			osDelay(beepDuration);
			System_GPIO_Write(BEEP_GPIO_GROUP, BEEP_GPIO_ID, LOW);
			osMessageQueueReset(beepDurationQueueHandle);
			osMutexRelease(beepStateMutexHandle);
		}
	}
}
