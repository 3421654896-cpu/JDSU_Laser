/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f2xx.h"
#include "datastruct.h"
#include "ms5614t.h"
#include "peak_detection.h"
#include "stm32f2xx_it.h"
#include "adc.h"
#include "usart.h"
#include "temperature.h"
#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
    TABLE_STATE = 0,
    MANUAL_STATE = 1,
		EXTRA_STATE = 2,
}MAIN_LOOP_STATE;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define   Para_Addr               0x08020000
#define   Wave_Addr               0x08030000
#define   Pwr_Addr								0x08040000

#define LED_MANUAL_PORT    				GPIOC
#define LED_MANUAL_PIN						GPIO_PIN_0
#define LED_TABLE_PORT						GPIOC
#define LED_TABLE_PIN							GPIO_PIN_1
#define LED_MANUAL_LOW()    			HAL_GPIO_WritePin(LED_MANUAL_PORT, LED_MANUAL_PIN, GPIO_PIN_RESET)
#define LED_MANUAL_HIGH()   			HAL_GPIO_WritePin(LED_MANUAL_PORT, LED_MANUAL_PIN, GPIO_PIN_SET)
#define LED_TABLE_LOW()						HAL_GPIO_WritePin(LED_TABLE_PORT, LED_TABLE_PIN, GPIO_PIN_RESET)
#define LED_TABLE_HIGH()					HAL_GPIO_WritePin(LED_TABLE_PORT, LED_TABLE_PIN, GPIO_PIN_SET)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern const uint16_t Wave_DAC[Number][5];
extern const uint16_t Wave_DATA[Number][2];
extern const uint16_t Pwr_DATA[Number][2];

extern uint8_t txBuffer[PACK_SIZE];
extern uint8_t aTxBuffer[USART_TX_SIZE];

extern uint32_t wave_time;
extern uint8_t workState;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
