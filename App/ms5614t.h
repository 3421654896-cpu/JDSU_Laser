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
#define DAC1_CS_PIN       GPIO_Pin_12
#define DAC1_FS_PORT      GPIOB
#define DAC1_FS_PIN       GPIO_Pin_14
#define DAC1_LDAC_PORT    GPIOC
#define DAC1_LDAC_PIN     GPIO_Pin_9
#define DAC1_PD_PORT      GPIOA
#define DAC1_PD_PIN       GPIO_Pin_8

#define DAC1_CS_LOW()    GPIO_ResetBits(DAC1_CS_PORT, DAC1_CS_PIN)
#define DAC1_CS_HIGH()   GPIO_SetBits(DAC1_CS_PORT, DAC1_CS_PIN)
#define DAC1_FS_LOW()    GPIO_ResetBits(DAC1_FS_PORT, DAC1_FS_PIN)
#define DAC1_FS_HIGH()   GPIO_SetBits(DAC1_FS_PORT, DAC1_FS_PIN)
#define DAC1_LDAC_LOW()  GPIO_ResetBits(DAC1_LDAC_PORT, DAC1_LDAC_PIN)
#define DAC1_PD_LOW()    GPIO_ResetBits(DAC1_PD_PORT, DAC1_PD_PIN)
#define DAC1_PD_HIGH()   GPIO_SetBits(DAC1_PD_PORT, DAC1_PD_PIN)

/* ===================== DAC2(SPI2)?? ===================== */
#define DAC2_CS_PORT      GPIOA
#define DAC2_CS_PIN       GPIO_Pin_15
#define DAC2_FS_PORT      GPIOC
#define DAC2_FS_PIN       GPIO_Pin_11
#define DAC2_LDAC_PORT    GPIOD
#define DAC2_LDAC_PIN     GPIO_Pin_2
#define DAC2_PD_PORT      GPIOB
#define DAC2_PD_PIN       GPIO_Pin_3

#define DAC2_CS_LOW()    GPIO_ResetBits(DAC2_CS_PORT, DAC2_CS_PIN)
#define DAC2_CS_HIGH()   GPIO_SetBits(DAC2_CS_PORT, DAC2_CS_PIN)
#define DAC2_FS_LOW()    GPIO_ResetBits(DAC2_FS_PORT, DAC2_FS_PIN)
#define DAC2_FS_HIGH()   GPIO_SetBits(DAC2_FS_PORT, DAC2_FS_PIN)
#define DAC2_LDAC_LOW()  GPIO_ResetBits(DAC2_LDAC_PORT, DAC2_LDAC_PIN)
#define DAC2_PD_LOW()    GPIO_ResetBits(DAC2_PD_PORT, DAC2_PD_PIN)
#define DAC2_PD_HIGH()   GPIO_SetBits(DAC2_PD_PORT, DAC2_PD_PIN)

#define GAIN 3357
#define SOA 3357

extern uint16_t uADCOriginvalues[4];

/* ?1? DAC */
void MS5614T_Init(void);
void MS5614T_SetCode(MS5614T_Channel_t channel, uint16_t code,
                     MS5614T_Speed_t speed, MS5614T_Power_t power);

/* ?2? DAC */
void MS5614T2_Init(void);
void MS5614T2_SetCode(MS5614T_Channel_t channel, uint16_t code,
                      MS5614T_Speed_t speed, MS5614T_Power_t power);

void write_ms5614t_table();
void write_ms5614t_manual();
void modify_table_loop();
void ClearRxBuff();
void sendTxBuffer(int dac_size, int p1, int p2, int p3, int p4);

#ifdef __cplusplus
}
#endif

#endif
