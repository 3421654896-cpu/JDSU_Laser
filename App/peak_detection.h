#ifndef PEAK_DETECTION_H
#define PEAK_DETECTION_H

#include "stm32f2xx.h"
#include "datastruct.h"

#define INTERVAL 70
#define PEAKS_COUNT 15

extern uint16_t item;

extern uint16_t adc1[Number];
extern uint16_t adc2[Number];
extern uint16_t adc3[Number];
extern uint16_t adc4[Number];

extern float adc_norvec[Number];

extern uint16_t peaks1[PEAKS_COUNT];
extern uint16_t peaks2[PEAKS_COUNT];
extern uint16_t peaks3[PEAKS_COUNT];
extern uint16_t peaks4[PEAKS_COUNT];

extern uint8_t packPeaks[4*4*PEAKS_COUNT+5];

uint8_t Find_Peaks(uint16_t *adc_vec, uint16_t *peaks_vec, uint16_t adc_length);

void Find_Initial(float *adc_norvec, uint16_t *initials, uint16_t adc_length, float threshold);

void SendPeaks(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

#endif
