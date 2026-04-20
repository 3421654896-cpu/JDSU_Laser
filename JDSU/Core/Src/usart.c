#include "main.h"

uint8_t getData = 0;
uint8_t lastGet = 0;
uint8_t ReceEndFlag = 0;
__IO uint8_t uhRxCounter = 0;
uint8_t txHead = 0;
uint8_t txTail = 0;
uint8_t aRxBuffer[USART_RX_SIZE] = {0};
uint8_t uartFrame[USART_RX_SIZE] = {0};
uint16_t tbuffer[USART_RX_SIZE] = {0};

uint16_t txCount = 0;
uint8_t dma_transfer_complete = 1;
uint8_t txQueue[TX_QUEUE_SIZE][USART_RX_SIZE] = {0};
uint16_t txLen[USART_RX_SIZE] = {0};

void USART_Queue_Send(uint8_t *data, uint16_t len)
{
    memcpy(txQueue[txHead], data, len);
    txLen[txHead] = len;
    txHead = (txHead + 1) % TX_QUEUE_SIZE;

    if(dma_transfer_complete)
    {
        USART_DMA_Send();
    }
}

void USART_DMA_Send(void)
{
	if(txTail == txHead) return;

	dma_transfer_complete = 0;

	HAL_UART_Transmit_DMA(&huart1,
												txQueue[txTail],
												txLen[txTail]);

	txTail = (txTail + 1) % TX_QUEUE_SIZE;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		if(workState == MANUAL_STATE)
		{
//				dma_transfer_complete = 1;
				USART_DMA_Send();
		}
		else
		{
//				uint8_t flag = 0x21;
//				HAL_UART_Transmit_DMA(&huart1, &flag, 1);
		}
		dma_transfer_complete = 1;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
//		HAL_UART_DMAStop(&huart1);
		
		memcpy(aRxBuffer, uartFrame, USART_RX_SIZE*sizeof(uint8_t));
		
		ReceEndFlag = 1;
//		USART_Queue_Send(aRxBuffer, USART_RX_SIZE);

		HAL_UART_Receive_DMA(&huart1, uartFrame, USART_RX_SIZE);
	}
}
