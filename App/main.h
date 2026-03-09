#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f2xx.h"
#include "stm32f2xx_it.h"
#include "ms5614t.h"
#include "peak_detection.h"
#include "init.h"
#include "datastruct.h"
#include "adc.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define PACK_SIZE 4+Number*8+2+4*15+4+1

typedef enum
{
    TABLE_STATE = 0,
    MANUAL_STATE = 1
}MAIN_LOOP_STATE;


extern const u16 Wave_DAC[Number][5];
extern const u16 Wave_DATA[Number][2];
extern const u16 Pwr_DATA[Number][2];

extern uint8_t txBuffer[PACK_SIZE];

extern uint16_t wave_time;
extern uint8_t getData;
extern uint8_t workState;
extern uint8_t lastGet;
extern uint16_t txCount;

void sampleVoltage();

#define   Para_Addr               0x08010000
#define   Wave_Addr               0x08020000
#define   Pwr_Addr								0x08030000			

#endif