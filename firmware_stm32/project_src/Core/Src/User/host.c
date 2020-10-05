#include "host.h"

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
uint8_t hostRecieveBuffer[HOST_RECIEVE_BUFFER_SIZE] = {0};

uint8_t hostConnectionState = HOST_STATE_DISCONNECTED;
uint8_t hostStreamState = HOST_STREAM_STATE_WAITFORMETA;

volatile uint16_t hostLastDMAStreamSetLength = HOST_RECIEVE_BUFFER_SIZE;
volatile uint16_t hostCurrentDMAStreamLength = 0;
volatile uint16_t hostFullStreamLength = 0;
volatile uint16_t hostPayloadStreamLength = 0;

void Host_Init(void)
{
	// USART
	huart2.Instance = USART2;
	huart2.Init.BaudRate = HOST_USART_BAUD;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	__HAL_RCC_USART2_CLK_ENABLE();
	HAL_UART_Init(&huart2);

	// DMA RX
	hdma_usart2_rx.Instance = DMA1_Stream5;
	hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_rx.Init.Mode = DMA_NORMAL;
	hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_DMA_Init(&hdma_usart2_rx);
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	__HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

	// DMA TX
	hdma_usart2_tx.Instance = DMA1_Stream6;
	hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart2_tx);
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	__HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx);

	// GPIO
	System_GPIO_Alt_Init(HOST_TX_GPIO_GROUP, HOST_TX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART2);
	System_GPIO_Alt_Init(HOST_RX_GPIO_GROUP, HOST_RX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART2);

	// NVIC
	HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	HAL_UART_Receive_DMA(&huart2, (uint8_t*)hostRecieveBuffer, hostLastDMAStreamSetLength);
}

void Host_SendByte(uint8_t data)
{
	uint8_t _ret = HAL_OK;
	do
	{
		_ret = HAL_UART_Transmit_DMA(&huart2, (uint8_t*)(&data), 1);
	} while (_ret == HAL_BUSY);
}

void Host_SendStream(uint8_t* dataStream, uint16_t streamLength)
{
	uint8_t _ret = HAL_OK;
	do
	{
		_ret = HAL_UART_Transmit_DMA(&huart2, dataStream, streamLength);
	} while (_ret == HAL_BUSY);
}

void Host_SendNumber(int32_t _value)
{
	int _level = 1, _temp = 0, _cnt = 0;
	if (_value == 0) Host_SendStream((uint8_t*) "0", 1);
	else if (_value < 0) Host_SendStream((uint8_t*) "-", 1);
	_value = abs(_value);
	_temp = _value;
	while (_temp)
	{
		_temp /= 10;
		_level *= 10;
		_cnt++;
	}
	_level /= 10;
	for (int i = 0; i < _cnt; i++)
	{
		char _ch = (char)((int)(_value / _level) + '0');
		_value %= _level;
		_level /= 10;
		Host_SendStream((uint8_t*)(&_ch), 1);
	}
}

void Host_DataHandler(uint8_t* dataStream)
{
	HAL_UART_DMAStop(&huart2);

	hostCurrentDMAStreamLength = hostLastDMAStreamSetLength - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
	hostFullStreamLength += hostCurrentDMAStreamLength;

	if (hostStreamState == HOST_STREAM_STATE_WAITFORMETA)
	{
		if (dataStream[0] == HOST_STREAM_HEAD)
		{
			if (hostFullStreamLength >= 3)
			{
				hostPayloadStreamLength = StreamBuilder_GetUnsignedInt(dataStream, 1, INT16_BYTE);
				hostStreamState = HOST_STREAM_STATE_WAITFORTERMINAL;
			}
			else
			{
				hostLastDMAStreamSetLength -= hostCurrentDMAStreamLength;
				HAL_UART_Receive_DMA(&huart2, (uint8_t*)(hostRecieveBuffer + hostCurrentDMAStreamLength), hostLastDMAStreamSetLength);
			}
		}
		else
		{
			hostFullStreamLength = 0;
			hostLastDMAStreamSetLength = HOST_RECIEVE_BUFFER_SIZE;
			HAL_UART_Receive_DMA(&huart2, (uint8_t*)hostRecieveBuffer, hostLastDMAStreamSetLength);
		}
	}
	if (hostStreamState == HOST_STREAM_STATE_WAITFORTERMINAL)
	{
		if (hostFullStreamLength >= hostPayloadStreamLength + 3)
		{
			hostLastDMAStreamSetLength = HOST_RECIEVE_BUFFER_SIZE;
			hostStreamState = HOST_STREAM_STATE_WAITFORMETA;
			osMessageQueuePut(hostStreamQueueHandle, (uint32_t*) (&hostFullStreamLength), 0, 0);
		}
		else
		{
			hostLastDMAStreamSetLength -= hostCurrentDMAStreamLength;
			HAL_UART_Receive_DMA(&huart2, (uint8_t*)(hostRecieveBuffer + hostCurrentDMAStreamLength), hostLastDMAStreamSetLength);
		}
	}
}

uint16_t Host_ProcessStream(uint8_t* dataStream, uint16_t streamLength, uint8_t builderType)
{
	uint16_t _length = 0;

	// Head
	_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_HEAD, dataStream, _length, BYTE);

	// Protocols
	if (builderType == HOST_STREAM_CONNECT_REQ)
	{
		// Operation
		hostConnectionState = HOST_STATE_CONNECTED;

		// Resp Stream
		// Length
		_length += StreamBuilder_AddUnsignedInt(3, dataStream, _length, INT16_BYTE);
		// Resp ok
		_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);
		// System version
		_length += StreamBuilder_AddUnsignedInt(SYSTEM_VERSION, dataStream, _length, BYTE);
	}
	else if (hostConnectionState == HOST_STATE_CONNECTED)
	{
		if (builderType == HOST_STREAM_SPEED_REQ)
		{
			// Operation
			float _speedX = StreamBuilder_GetFloat(dataStream, 4, FLOAT_BYTE_Q8);
			float _speedYaw = StreamBuilder_GetFloat(dataStream, 8, FLOAT_BYTE_Q8);
			Kinematic_SetSpeed(_speedX, _speedYaw);

			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(30, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);

			// Motor data
			osMutexAcquire(motorDataMutexHandle, osWaitForever);
			// Motor left speed pps
			_length += StreamBuilder_AddInt(robotMotorData[MOTOR_LEFT].currentSpeedPPS, dataStream, _length, INT32_BYTE);
			// Motor left target speed pps
			_length += StreamBuilder_AddInt(robotMotorData[MOTOR_LEFT].targetSpeedPPS, dataStream, _length, INT32_BYTE);
			// Motor right speed pps
			_length += StreamBuilder_AddInt(robotMotorData[MOTOR_RIGHT].currentSpeedPPS, dataStream, _length, INT32_BYTE);
			// Motor right target speed pps
			_length += StreamBuilder_AddInt(robotMotorData[MOTOR_RIGHT].targetSpeedPPS, dataStream, _length, INT32_BYTE);
			osMutexRelease(motorDataMutexHandle);

			// Kinematic data
			Kinematic_UpdateDisplacement();
			// Dispalcement X
			_length += StreamBuilder_AddFloat(robotDisplacement.displacementX, dataStream, _length, FLOAT_BYTE_Q8);
			// Dispalcement Y
			_length += StreamBuilder_AddFloat(robotDisplacement.displacementY, dataStream, _length, FLOAT_BYTE_Q8);
			// Dispalcement Yaw
			_length += StreamBuilder_AddFloat(robotDisplacement.displacementYaw, dataStream, _length, FLOAT_BYTE_Q8);
		}
		else if (builderType == HOST_STREAM_SERVO_REQ)
		{
			// Operation
			for (uint8_t i = 0; i < SERVO_NUMBER; i++)
				Servo_SetAngle(i, StreamBuilder_GetUnsignedInt(dataStream, i + 4, BYTE));

			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(SERVO_NUMBER + 2, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);

			// Servo data
			osMutexAcquire(servoStateMutexHandle, osWaitForever);
			for (uint8_t i = 0; i < SERVO_NUMBER; i++)
				_length += StreamBuilder_AddUnsignedInt(servoData[i], dataStream, _length, BYTE);
			osMutexRelease(servoStateMutexHandle);
		}
		else if (builderType == HOST_STREAM_BATTERY_REQ)
		{
			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(8, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);

			// Battery data
			osMutexAcquire(batteryDataMutexHandle, osWaitForever);
			// Battery state
			_length += StreamBuilder_AddUnsignedInt(batteryData.batteryState, dataStream, _length, BYTE);
			// Battery voltage
			_length += StreamBuilder_AddUnsignedFloat(batteryData.batteryVoltage, dataStream, _length, FLOAT_BYTE_Q16);
			// Battery percent
			_length += StreamBuilder_AddUnsignedInt(batteryData.batteryPercent, dataStream, _length, BYTE);
			osMutexRelease(batteryDataMutexHandle);
		}
		else if (builderType == HOST_STREAM_SLAMCORE_STATE_REQ)
		{
			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(3, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);

			// Slamware state
			_length += StreamBuilder_AddUnsignedInt(slamwareState, dataStream, _length, BYTE);
		}
		else if (builderType == HOST_STREAM_SONAR_REQ)
		{
			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(SONAR_NUMBER * 4 + 2, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);

			// Sonar data
			osMutexAcquire(sonarDataMutexHandle, osWaitForever);
			for (int i = 0; i < SONAR_NUMBER; i++)
				_length += StreamBuilder_AddUnsignedInt(sonarData[i], dataStream, _length, INT32_BYTE);
			osMutexRelease(sonarDataMutexHandle);
		}
		else if (builderType == HOST_STREAM_BEEP_REQ)
		{
			// Operation
			Beep_Start(StreamBuilder_GetUnsignedInt(dataStream, 4, INT32_BYTE));

			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(2, dataStream, _length, INT16_BYTE);
			// Resp ok
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_OK, dataStream, _length, BYTE);
		}
	}
	else
	{
			// Resp Stream
			// Length
			_length += StreamBuilder_AddUnsignedInt(2, dataStream, _length, INT16_BYTE);
			// Resp error
			_length += StreamBuilder_AddUnsignedInt(HOST_STREAM_RESP_ERROR, dataStream, _length, BYTE);
	}

	// Checksum
	_length += StreamBuilder_AddCheckSum(dataStream, 0, _length - 1, _length);

	return _length;
}

osThreadId_t hostTaskHandle;
const osThreadAttr_t hostTask_attributes = {
	.name = "hostTask",
	.priority = (osPriority_t) osPriorityHigh,
	.stack_size = 2048 * 4
};

osMessageQueueId_t hostStreamQueueHandle;
const osMessageQueueAttr_t hostStreamQueue_attributes = {
	.name = "hostStreamQueue"
};

void Host_RunTask(void)
{
	hostStreamQueueHandle = 
		osMessageQueueNew (1, sizeof(uint16_t*), &hostStreamQueue_attributes);
	hostTaskHandle = 
		osThreadNew(Host_Task, NULL, &hostTask_attributes);
}

void Host_Task(void *argument)
{
	uint16_t _length = 0;
	uint16_t _sendLength = 0;
	while(1)
	{
		if (osMessageQueueGet(hostStreamQueueHandle, &_length, 0, HOST_COMMUNICATION_TIMEOUT) == osOK)
		{
			osMessageQueueReset(hostStreamQueueHandle);
			if (StreamBuilder_VerifyCheckSum(hostRecieveBuffer, 0, _length - 2, hostRecieveBuffer[_length - 1]) == CHECKSUM_OK)
				_sendLength = Host_ProcessStream(hostRecieveBuffer, _length, hostRecieveBuffer[3]);
			else _sendLength = Host_ProcessStream(hostRecieveBuffer, _length, HOST_STREAM_RESP_ERROR);

			hostStreamState = HOST_STREAM_STATE_WAITFORMETA;
			hostFullStreamLength = 0;
			hostLastDMAStreamSetLength = HOST_RECIEVE_BUFFER_SIZE;

			Host_SendStream(hostRecieveBuffer, _sendLength);

			HAL_UART_Receive_DMA(&huart2, (uint8_t*)hostRecieveBuffer, hostLastDMAStreamSetLength);
		}
		else
		{
			HAL_UART_DMAStop(&huart2);
			hostConnectionState = HOST_STATE_DISCONNECTED;
			hostStreamState = HOST_STREAM_STATE_WAITFORMETA;
			hostFullStreamLength = 0;
			hostLastDMAStreamSetLength = HOST_RECIEVE_BUFFER_SIZE;
			HAL_UART_Receive_DMA(&huart2, (uint8_t*)hostRecieveBuffer, hostLastDMAStreamSetLength);
		}
	}
}
