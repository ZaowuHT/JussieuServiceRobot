#include "slamware.h"

UART_HandleTypeDef huart3;
uint8_t slamwareRecieveBuffer[SLAMWARE_RECIEVE_BUFFER_SIZE] = {0};
uint8_t slamwareSendBuffer[SLAMWARE_SEND_BUFFER_SIZE] = {0};

uint8_t slamwareState = SLAMWARE_STATE_DISCONNECTED;

void Slamware_Init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = SLAMWARE_USART_BAUD;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    __HAL_RCC_USART3_CLK_ENABLE();
    HAL_UART_Init(&huart3);

    System_GPIO_Alt_Init(SLAMWARE_TX_GPIO_GROUP, SLAMWARE_TX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3);
	System_GPIO_Alt_Init(SLAMWARE_RX_GPIO_GROUP, SLAMWARE_RX_GPIO_ID,
		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3);

    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

void Slamware_DataHandler(uint8_t* dataStream)
{

}
