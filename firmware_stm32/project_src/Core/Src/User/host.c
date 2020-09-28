#include "host.h"

UART_HandleTypeDef huart2;
uint8_t hostRecieveBuffer[HOST_RECIEVE_BUFFER_SIZE] = {0};
uint8_t hostSendBuffer[HOST_SEND_BUFFER_SIZE] = {0};

uint8_t hostConnectionState = HOST_STATE_DISCONNECTED;
uint8_t hostStreamState = HOST_STREAM_STATE_WAITFORHEAD;

void Host_Init(void)
{
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

	System_GPIO_Alt_Init(HOST_TX_GPIO_GROUP, HOST_TX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART2);
	System_GPIO_Alt_Init(HOST_RX_GPIO_GROUP, HOST_RX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART2);

	HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void Host_SendByte(uint8_t data)
{
	HAL_UART_Transmit(&huart2, (uint8_t *) (&data), 1, 0xffff);
}

void Host_SendStream(uint8_t* dataStream, uint16_t streamLength)
{
	HAL_UART_Transmit(&huart2, dataStream, streamLength, 0xffff);
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
