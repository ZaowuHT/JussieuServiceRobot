#include "sonar.h"

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_rx;
uint8_t sonarRecieveBuffer[SONAR_RECIEVE_BUFFER_SIZE] = {0};

void Sonar_Multiplexer_Init(void)
{
	System_GPIO_Init(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, 
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, 
			GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, 
			GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, 
			GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
}

void Sonar_Multiplexer_SetChannel(uint8_t channelID)
{
	switch (channelID)
	{
	case 0:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	case 1:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	case 2:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	case 3:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	case 4:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	case 5:
	{
		System_GPIO_Write(SONAR_ROUTER_S0_GPIO_GROUP, SONAR_ROUTER_S0_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S1_GPIO_GROUP, SONAR_ROUTER_S1_GPIO_ID, LOW);
		System_GPIO_Write(SONAR_ROUTER_S2_GPIO_GROUP, SONAR_ROUTER_S2_GPIO_ID, HIGH);
		System_GPIO_Write(SONAR_ROUTER_S3_GPIO_GROUP, SONAR_ROUTER_S3_GPIO_ID, LOW);
		break;
	}
	}
}

uint8_t sonarIter = 0;
uint8_t sonarState = SONAR_STATE_WAITHEAD;
SonarData sonarData;

volatile uint16_t sonarLastDMAStreamSetLength = SONAR_RECIEVE_BUFFER_SIZE;
volatile uint16_t sonarCurrentDMAStreamLength = 0;
volatile uint16_t sonarFullStreamLength = 0;

void Sonar_Init(void)
{
	// UART
	huart4.Instance = UART4;
	huart4.Init.BaudRate = SONAR_UART_BAUD;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	__HAL_RCC_UART4_CLK_ENABLE();
	HAL_UART_Init(&huart4);

	// DMA RX
	hdma_uart4_rx.Instance = DMA1_Stream2;
	hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_uart4_rx.Init.Mode = DMA_NORMAL;
	hdma_uart4_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
	hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_DMA_Init(&hdma_uart4_rx);
	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
	__HAL_LINKDMA(&huart4, hdmarx, hdma_uart4_rx);

	// GPIO
	System_GPIO_Alt_Init(SONAR_TX_GPIO_GROUP, SONAR_TX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF8_UART4);
	System_GPIO_Alt_Init(SONAR_RX_GPIO_GROUP, SONAR_RX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF8_UART4);

	// NVIC
	HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);

	// SWITCH
	Sonar_Multiplexer_Init();
	Sonar_Multiplexer_SetChannel(0);
}

void Sonar_SendByte(uint8_t data)
{
	HAL_UART_Transmit(&huart4, (uint8_t *) (&data), 1, 0xffff);
}

void Sonar_DataHandler(uint8_t* dataStream)
{
	HAL_UART_DMAStop(&huart4);

	sonarCurrentDMAStreamLength = sonarLastDMAStreamSetLength - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
	sonarFullStreamLength += sonarCurrentDMAStreamLength;

	if (sonarState == SONAR_STATE_WAITHEAD)
	{
		if (dataStream[0] == SONAR_FRAME_HEAD)
		{
			sonarState = SONAR_STATE_WAITPAYLOAD;
		}
		else
		{
			sonarFullStreamLength = 0;
			sonarLastDMAStreamSetLength = SONAR_RECIEVE_BUFFER_SIZE;
			HAL_UART_Receive_DMA(&huart4, (uint8_t*)sonarRecieveBuffer, sonarLastDMAStreamSetLength);
		}
	}
	if (sonarState == SONAR_STATE_WAITPAYLOAD)
	{
		if (sonarFullStreamLength >= 4)
		{
			sonarState = SONAR_STATE_WAITHEAD;
			sonarFullStreamLength = 0;
			sonarLastDMAStreamSetLength = SONAR_RECIEVE_BUFFER_SIZE;
			uint16_t _checksum = 
				(dataStream[0] + dataStream[1] + dataStream[2]) & 0x00ff;
			if (_checksum == dataStream[3])
			{
				uint32_t _distance = dataStream[1] * 256 + dataStream[2];
				osMessageQueuePut(sonarDataQueueHandle, &_distance, 0, 0);
			}
		}
		else
		{
			sonarLastDMAStreamSetLength -= sonarCurrentDMAStreamLength;
			HAL_UART_Receive_DMA(&huart4, (uint8_t*)(sonarRecieveBuffer + sonarCurrentDMAStreamLength), sonarLastDMAStreamSetLength);
		}
	}
}

osThreadId_t sonarTaskHandle;
const osThreadAttr_t sonarTask_attributes = {
	.name = "sonarTask",
	.priority = (osPriority_t) osPriorityHigh2,
	.stack_size = 512 * 4
};

osMutexId_t sonarDataMutexHandle;
const osMutexAttr_t sonarDataMutex_attributes = {
	.name = "sonarDataMutex"
};

osMessageQueueId_t sonarDataQueueHandle;
const osMessageQueueAttr_t sonarDataQueue_attributes = {
	.name = "sonarDataQueue"
};

void Sonar_RunTask(void)
{
	sonarDataMutexHandle = 
		osMutexNew(&sonarDataMutex_attributes);
	sonarDataQueueHandle = 
		osMessageQueueNew(1, sizeof(uint32_t*), &sonarDataQueue_attributes);
	sonarTaskHandle = 
		osThreadNew(Sonar_Task, NULL, &sonarTask_attributes);
}

void Sonar_Task(void *argument)
{
	while(1)
	{
		osMutexAcquire(sonarDataMutexHandle, osWaitForever);
		Sonar_Multiplexer_SetChannel(sonarIter);

		sonarState = SONAR_STATE_WAITHEAD;
		sonarFullStreamLength = 0;
		sonarLastDMAStreamSetLength = SONAR_RECIEVE_BUFFER_SIZE;
		HAL_UART_Receive_DMA(&huart4, (uint8_t*)sonarRecieveBuffer, sonarLastDMAStreamSetLength);

		Sonar_SendByte(SONAR_TRIGGER);

		osMessageQueueGet(sonarDataQueueHandle, &(sonarData[sonarIter]), 0, SONAR_TIMEOUT);
		osMessageQueueReset(sonarDataQueueHandle);

		sonarIter = sonarIter + 1 >= SONAR_NUMBER ? 0 : sonarIter + 1;
		
		osMutexRelease(sonarDataMutexHandle);
		osDelay(SONAR_REFRESH_DURATION);
	}
}
