#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include <stdio.h>

#define ADC_CS_PORT					GPIOA
#define ADC_CS_PIN					GPIO_Pin_4
#define ADC_GPIO_PORT 			GPIOC
#define ADC_GPIO_PIN 				GPIO_Pin_4

#define ADC_CS_LOW()				GPIO_ResetBits(ADC_CS_PORT, ADC_CS_PIN);
#define ADC_CS_HIGH()				GPIO_SetBits(ADC_CS_PORT, ADC_CS_PIN);

#define ADC_START_FRAME			(0x1 << 12) | (0x1 << 11)
#define ADC_RESET_FRAME			(0x2 << 12) | (0x1 << 11) | (0x1 << 10)
#define ADC_ENTER_FRAME			(0x2 << 12) | (0x1 << 11)
#define ADC_LOOP_FRAME			(0x2 << 12)

void ADC_SPI_Init(void);
uint16_t ADC_SPI_Cmd(uint16_t cmdF);
uint16_t ADC_Write_Read(uint8_t ch);
void ADC_Loop_Start();
uint16_t ADC_Write_Loop();

#endif /* __ADC_H */
