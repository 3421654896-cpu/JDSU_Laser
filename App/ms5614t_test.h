/**
  ******************************************************************************
  * @file    ms5614t.h
  * @brief   MS5614T DAC?????
  * @note    ???STM32F205RET6??SPI??MS5614T
  ******************************************************************************
  */

#ifndef __MS5614T_H
#define __MS5614T_H

#include "stm32f2xx.h"
#include <stdint.h>

/* DAC???? */
typedef enum {
    MS5614T_DAC_A = 0x00,  // A1=0, A0=0
    MS5614T_DAC_B = 0x01,  // A1=0, A0=1
    MS5614T_DAC_C = 0x02,  // A1=1, A0=0
    MS5614T_DAC_D = 0x03   // A1=1, A0=1
} MS5614T_Channel_t;

/* ?????? */
typedef enum {
    MS5614T_SPEED_SLOW = 0,  // ???? (????9us)
    MS5614T_SPEED_FAST = 1   // ???? (????3us)
} MS5614T_Speed_t;

/* ?????? */
typedef enum {
    MS5614T_NORMAL = 0,    // ??????
    MS5614T_POWERDOWN = 1  // ????
} MS5614T_Power_t;

/* SPI???? - ?????????? */
#define MS5614T_SPI                 SPI1
#define MS5614T_SPI_CLK             RCC_APB2Periph_SPI1
#define MS5614T_SPI_GPIO_CLK        RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB

#define MS5614T_SPI_SCK_PIN         GPIO_Pin_5
#define MS5614T_SPI_SCK_GPIO        GPIOA
#define MS5614T_SPI_SCK_SOURCE      GPIO_PinSource5
#define MS5614T_SPI_SCK_AF          GPIO_AF_SPI1

#define MS5614T_SPI_MOSI_PIN        GPIO_Pin_7
#define MS5614T_SPI_MOSI_GPIO       GPIOA
#define MS5614T_SPI_MOSI_SOURCE     GPIO_PinSource7
#define MS5614T_SPI_MOSI_AF         GPIO_AF_SPI1

#define MS5614T_CS_PIN              GPIO_Pin_4
#define MS5614T_CS_GPIO             GPIOA

#define MS5614T_LDAC_PIN            GPIO_Pin_0
#define MS5614T_LDAC_GPIO           GPIOB

#define MS5614T_PD_PIN              GPIO_Pin_1
#define MS5614T_PD_GPIO             GPIOB

/* ??????? */
#define MS5614T_CS_LOW()            GPIO_ResetBits(MS5614T_CS_GPIO, MS5614T_CS_PIN)
#define MS5614T_CS_HIGH()           GPIO_SetBits(MS5614T_CS_GPIO, MS5614T_CS_PIN)
#define MS5614T_LDAC_LOW()          GPIO_ResetBits(MS5614T_LDAC_GPIO, MS5614T_LDAC_PIN)
#define MS5614T_LDAC_HIGH()         GPIO_SetBits(MS5614T_LDAC_GPIO, MS5614T_LDAC_PIN)
#define MS5614T_PD_LOW()            GPIO_ResetBits(MS5614T_PD_GPIO, MS5614T_PD_PIN)
#define MS5614T_PD_HIGH()           GPIO_SetBits(MS5614T_PD_GPIO, MS5614T_PD_PIN)

/* DAC???? */
#define MS5614T_DAC_RESOLUTION      4096    // 12????
#define MS5614T_VREF_DEFAULT        2.048f  // ??????(V)

/* ???? */
void MS5614T_Init(void);
void MS5614T_SetVoltage(MS5614T_Channel_t channel, float voltage, float vref);
void MS5614T_SetCode(MS5614T_Channel_t channel, uint16_t code, 
                     MS5614T_Speed_t speed, MS5614T_Power_t power);
void MS5614T_UpdateOutput(void);
void MS5614T_PowerDown(uint8_t enable);
uint16_t MS5614T_VoltageToCode(float voltage, float vref);

#endif /* __MS5614T_H */
