/* App/ms5614t.h */
#ifndef __MS5614T_H
#define __MS5614T_H

#include "stm32f2xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MS5614T_DAC_A = 0x00,
    MS5614T_DAC_B = 0x01,
    MS5614T_DAC_C = 0x02,
    MS5614T_DAC_D = 0x03
} MS5614T_Channel_t;

typedef enum
{
    MS5614T_SPEED_SLOW = 0,
    MS5614T_SPEED_FAST = 1
} MS5614T_Speed_t;

typedef enum
{
    MS5614T_NORMAL    = 0,
    MS5614T_POWERDOWN = 1
} MS5614T_Power_t;

/* ===================== DAC1(SPI3)?? ===================== */
#define DAC1_CS_PORT      GPIOB
#define DAC1_CS_PIN       GPIO_PIN_12
#define DAC1_FS_PORT      GPIOB
#define DAC1_FS_PIN       GPIO_PIN_14
#define DAC1_LDAC_PORT    GPIOC
#define DAC1_LDAC_PIN     GPIO_PIN_9
#define DAC1_PD_PORT      GPIOA
#define DAC1_PD_PIN       GPIO_PIN_8

#define DAC1_CS_LOW()    HAL_GPIO_WritePin(DAC1_CS_PORT, DAC1_CS_PIN, GPIO_PIN_RESET)
#define DAC1_CS_HIGH()   HAL_GPIO_WritePin(DAC1_CS_PORT, DAC1_CS_PIN, GPIO_PIN_SET)
#define DAC1_FS_LOW()    HAL_GPIO_WritePin(DAC1_FS_PORT, DAC1_FS_PIN, GPIO_PIN_RESET)
#define DAC1_FS_HIGH()   HAL_GPIO_WritePin(DAC1_FS_PORT, DAC1_FS_PIN, GPIO_PIN_SET)
#define DAC1_LDAC_LOW()  HAL_GPIO_WritePin(DAC1_LDAC_PORT, DAC1_LDAC_PIN, GPIO_PIN_RESET)
#define DAC1_PD_LOW()    HAL_GPIO_WritePin(DAC1_PD_PORT, DAC1_PD_PIN, GPIO_PIN_RESET)
#define DAC1_PD_HIGH()   HAL_GPIO_WritePin(DAC1_PD_PORT, DAC1_PD_PIN, GPIO_PIN_SET)

/* ===================== DAC2(SPI2)?? ===================== */
#define DAC2_CS_PORT      GPIOA
#define DAC2_CS_PIN       GPIO_PIN_15
#define DAC2_FS_PORT      GPIOC
#define DAC2_FS_PIN       GPIO_PIN_11
#define DAC2_LDAC_PORT    GPIOD
#define DAC2_LDAC_PIN     GPIO_PIN_2
#define DAC2_PD_PORT      GPIOB
#define DAC2_PD_PIN       GPIO_PIN_3

#define DAC2_CS_LOW()    HAL_GPIO_WritePin(DAC2_CS_PORT, DAC2_CS_PIN, GPIO_PIN_RESET)
#define DAC2_CS_HIGH()   HAL_GPIO_WritePin(DAC2_CS_PORT, DAC2_CS_PIN, GPIO_PIN_SET)
#define DAC2_FS_LOW()    HAL_GPIO_WritePin(DAC2_FS_PORT, DAC2_FS_PIN, GPIO_PIN_RESET)
#define DAC2_FS_HIGH()   HAL_GPIO_WritePin(DAC2_FS_PORT, DAC2_FS_PIN, GPIO_PIN_SET)
#define DAC2_LDAC_LOW()  HAL_GPIO_WritePin(DAC2_LDAC_PORT, DAC2_LDAC_PIN, GPIO_PIN_RESET)
#define DAC2_PD_LOW()    HAL_GPIO_WritePin(DAC2_PD_PORT, DAC2_PD_PIN, GPIO_PIN_RESET)
#define DAC2_PD_HIGH()   HAL_GPIO_WritePin(DAC2_PD_PORT, DAC2_PD_PIN, GPIO_PIN_SET)

#define GAIN 3357
#define SOA 3357

extern uint16_t frame;
extern uint16_t adcData;
extern uint16_t IDACData[5];
extern uint16_t uADCOriginvalues[4];

extern float tempData;
extern uint16_t tempInt;
extern uint16_t tempDec;

/* ?1? DAC */
void MS5614T_Init(void);
void MS5614T_SetCode(MS5614T_Channel_t channel, uint16_t code,
                     MS5614T_Speed_t speed, MS5614T_Power_t power);

/* ?2? DAC */
void MS5614T2_Init(void);
void MS5614T2_SetCode(MS5614T_Channel_t channel, uint16_t code,
                      MS5614T_Speed_t speed, MS5614T_Power_t power);

void write_ms5614t_table(void);
void write_ms5614t_manual(void);
void modify_table_loop(void);
void ClearRxBuff(void);
void sampleVoltage(void);
void sampleVoltageStable(void);
void sampleTemperature(void);
void sendTxBuffer(int dac_size, int p1, int p2, int p3, int p4);

void delay_us(__IO uint32_t us);
void delay_ms(__IO uint32_t ms);
void delay_s(__IO uint32_t s);
void short_delay(volatile uint32_t n);

#ifdef __cplusplus
}
#endif

#endif
