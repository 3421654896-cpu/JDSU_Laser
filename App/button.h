/**
  ******************************************************************************
  * @file    button.h
  * @brief   ???????
  ******************************************************************************
  */

#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f2xx.h"

/* ?????? */
#define BUTTON_PIN              GPIO_Pin_1
#define BUTTON_GPIO             GPIOA
#define BUTTON_GPIO_CLK         RCC_AHB1Periph_GPIOA

/* LED?????? */
#define LED_PIN                 GPIO_Pin_0
#define LED_GPIO                GPIOC
#define LED_GPIO_CLK            RCC_AHB1Periph_GPIOC

/* LED??? */
#define LED_ON()                GPIO_SetBits(LED_GPIO, LED_PIN)
#define LED_OFF()               GPIO_ResetBits(LED_GPIO, LED_PIN)
#define LED_TOGGLE()            GPIO_ToggleBits(LED_GPIO, LED_PIN)

/* ????? */
#define BUTTON_PRESSED          0  // ??????
#define BUTTON_RELEASED         1  // ??????

/* ????(ms) */
#define BUTTON_DEBOUNCE_TIME    20

/* ???? */
void Button_Init(void);
void Button_Scan(void);
uint8_t Button_GetState(void);

#endif /* __BUTTON_H */
