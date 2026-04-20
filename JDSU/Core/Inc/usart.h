#ifndef __USART_H
#define __USART_H

#include "stm32f2xx.h"
#include "datastruct.h"

#define TX_QUEUE_SIZE 5
#define USART_RX_SIZE 13
#define PACK_SIZE 4+Number*8+2+4*15+4+1

extern uint8_t getData;
extern uint8_t lastGet;
extern uint8_t ReceEndFlag;
extern uint8_t txHead;
extern uint8_t txTail;
extern uint8_t aRxBuffer[USART_RX_SIZE];
extern uint8_t uartFrame[USART_RX_SIZE];


extern uint16_t txCount;
extern uint8_t dma_transfer_complete;
extern uint8_t txQueue[TX_QUEUE_SIZE][USART_RX_SIZE];
extern uint16_t txLen[USART_RX_SIZE];

void USART_Queue_Send(uint8_t *data, uint16_t len);
void USART_DMA_Send(void);

#endif
