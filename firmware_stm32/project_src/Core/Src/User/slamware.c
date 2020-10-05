#include "slamware.h"

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
uint8_t slamwareRecieveBuffer[SLAMWARE_RECIEVE_BUFFER_SIZE] = {0};
uint8_t slamwareSendBuffer[SLAMWARE_SEND_BUFFER_SIZE] = {0};

uint8_t slamwareState = SLAMWARE_STATE_DISCONNECTED;

void Slamware_Init(void)
{
	// USART
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

	// DMA RX
	hdma_usart3_rx.Instance = DMA1_Stream1;
	hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart3_rx.Init.Mode = DMA_NORMAL;
	hdma_usart3_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_DMA_Init(&hdma_usart3_rx);
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	__HAL_LINKDMA(&huart3, hdmarx, hdma_usart3_rx);

	// DMA TX
	hdma_usart3_tx.Instance = DMA1_Stream3;
	hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart3_tx.Init.Mode = DMA_NORMAL;
	hdma_usart3_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart3_tx);
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	__HAL_LINKDMA(&huart3, hdmatx, hdma_usart3_tx);

	// GPIO
	System_GPIO_Alt_Init(SLAMWARE_TX_GPIO_GROUP, SLAMWARE_TX_GPIO_ID,
		  GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3);
	  System_GPIO_Alt_Init(SLAMWARE_RX_GPIO_GROUP, SLAMWARE_RX_GPIO_ID,
		  GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART3);

	// NVIC
	HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

void Slamware_DataHandler(uint8_t* dataStream)
{

}
