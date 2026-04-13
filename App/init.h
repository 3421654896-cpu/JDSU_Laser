#ifndef __INIT_H
#define __INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"

#define USART_DMA_CHANNEL					DMA_Channel_4
#define USART_DMA_STREAM					DMA2_Stream7

#define BUTTON_PORT 							GPIOA
#define BUTTON_PIN								GPIO_Pin_1

#define LED_MANUAL_PORT    				GPIOC
#define LED_MANUAL_PIN						GPIO_Pin_0
#define LED_TABLE_PORT						GPIOC
#define LED_TABLE_PIN							GPIO_Pin_1
#define LED_MANUAL_LOW()    			GPIO_ResetBits(LED_MANUAL_PORT, LED_MANUAL_PIN)
#define LED_MANUAL_HIGH()   			GPIO_SetBits(LED_MANUAL_PORT, LED_MANUAL_PIN)
#define LED_TABLE_LOW()						GPIO_ResetBits(LED_TABLE_PORT, LED_TABLE_PIN)
#define LED_TABLE_HIGH()					GPIO_SetBits(LED_TABLE_PORT, LED_TABLE_PIN)

void Clock_Init(void);
void NVIC_Config(void);

void USART_Config(void);
void USART_DMA_Send(uint8_t *data, uint16_t length);

void BUTTON_Config(void);
void LED_Config(void);

void delay_us(uint32_t us);
void delay_ms(__IO uint32_t ms);
void delay_s(__IO uint32_t s);

void short_delay(volatile uint32_t n);

extern uint8_t tx_buffer[32768];

#endif /* __MAIN_H */

