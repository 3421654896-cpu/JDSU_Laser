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

typedef enum
{
    TABLE_STATE = 0,
    MANUAL_STATE = 1
}MAIN_LOOP_STATE;


extern const u16 Wave_DAC[Number][3];
extern const u16 Wave_DATA[Number][2];
extern const u16 Pwr_DATA[Number][2];

extern uint8_t uSyncConvertedvalues[8];

extern uint16_t wave_time;
extern uint8_t getData;
extern uint8_t workState;
extern uint8_t lastGet;

void sampleVoltage();

#define   Para_Addr               0x08010000
#define   Wave_Addr               0x08020000
#define   Pwr_Addr								0x08030000			

#endif