#include "servo.h"

UART_HandleTypeDef huart1;

ServoData servoData;

void Servo_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = SERVO_USART_BAUD;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	__HAL_RCC_USART1_CLK_ENABLE();
	HAL_UART_Init(&huart1);

	System_GPIO_Alt_Init(SERVO_TX_GPIO_GROUP, SERVO_TX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1);
	System_GPIO_Alt_Init(SERVO_RX_GPIO_GROUP, SERVO_RX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1);

    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

	for (uint8_t i = 0; i < SERVO_NUMBER; i++)
	{
		servoData[i] = SERVO_DEFAULT_ANGLE;
		Servo_SendControlStream(i, SERVO_DEFAULT_ANGLE);
	}
}

void Servo_SendByte(uint8_t data)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) (&data), 1, 0xffff);
}

void Servo_SendControlStream(uint8_t servoID, uint8_t servoAngle)
{
	short servoPulse = (short)(servoAngle * 11.11f + 500);
    servoPulse = servoPulse < 500 ? 500 : (servoPulse > 2500 ? 2500 : servoPulse);
    Servo_SendByte(0xff);
    Servo_SendByte(0x02);
    Servo_SendByte(servoID);
    Servo_SendByte(servoPulse & 0xff);
    Servo_SendByte(servoPulse >> 8);
}

void Servo_SetAngle(uint8_t servoID, uint8_t servoAngle)
{
	if (osMutexAcquire(servoStateMutexHandle, SERVO_TIMEOUT) == osOK)
	{
		struct ServoAction _action;
		_action.servoID = servoID;
		_action.servoAngle = servoAngle;
		osMessageQueuePut(servoDataQueueHandle, &_action, 0, 0);
		osMutexRelease(servoStateMutexHandle);
	}
}

osThreadId_t servoTaskHandle;
const osThreadAttr_t servoTask_attributes = {
	.name = "servoTask",
	.priority = (osPriority_t) osPriorityHigh,
	.stack_size = 512 * 4
};

osMutexId_t servoStateMutexHandle;
const osMutexAttr_t servoStateMutex_attributes = {
	.name = "servoStateMutex"
};

osMessageQueueId_t servoDataQueueHandle;
const osMessageQueueAttr_t servoDataQueue_attributes = {
	.name = "servoDataQueue"
};

void Servo_RunTask(void)
{
	servoStateMutexHandle = 
		osMutexNew(&servoStateMutex_attributes);
	servoDataQueueHandle = 
		osMessageQueueNew (1, sizeof(struct ServoAction*), &servoDataQueue_attributes);
	servoTaskHandle = 
		osThreadNew(Servo_Task, NULL, &servoTask_attributes);
}

void Servo_Task(void *argument)
{
	struct ServoAction _action;
	while(1)
	{
		if (osMessageQueueGet(servoDataQueueHandle, &_action, 0, osWaitForever) == osOK)
		{
			osMutexAcquire(servoStateMutexHandle, osWaitForever);
			servoData[_action.servoID] = _action.servoAngle;
			Servo_SendControlStream(_action.servoID, _action.servoAngle);
			osMessageQueueReset(servoDataQueueHandle);
			osDelay(SERVO_ACTION_DURATION);
			osMutexRelease(servoStateMutexHandle);
		}
	}
}
