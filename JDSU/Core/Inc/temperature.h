#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#include "stm32f2xx.h"

#define TEMP_PORT							GPIOB
#define TEMP_PIN							GPIO_PIN_4

#define SKIP_ROM 							0xCC
#define CONVERT_T							0x44
#define READ_SCRATCHPAD				0xBE

extern uint16_t temperature;

void DQ_IN(void);
void DQ_OUT(void);

uint8_t M1820Z_Reset(void);
void M1820Z_WriteBit(uint8_t bit);
uint8_t M1820Z_ReadBit(void);
void M1820Z_WriteByte(uint8_t data);
uint8_t M1820Z_ReadByte(void);

float M1820Z_GetTmp(void);

#endif
