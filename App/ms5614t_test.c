/**
  ******************************************************************************
  * @file    ms5614t.c
  * @brief   MS5614T DAC????
  ******************************************************************************
  */

#include "ms5614t_test.h"

/**
  * @brief  ????(???)
  * @param  us: ?????
  */
static void delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while(ticks--);
}

/**
  * @brief  SPI??????
  * @param  byte: ??????
  * @retval ??????
  */
static uint8_t MS5614T_SPI_SendByte(uint8_t byte)
{
    while(SPI_I2S_GetFlagStatus(MS5614T_SPI, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(MS5614T_SPI, byte);
    
    while(SPI_I2S_GetFlagStatus(MS5614T_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(MS5614T_SPI);
}

/**
  * @brief  MS5614T GPIO???
  */
static void MS5614T_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ??GPIO?? */
    RCC_AHB1PeriphClockCmd(MS5614T_SPI_GPIO_CLK, ENABLE);
    
    /* ??SPI??: SCK, MOSI */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    
    GPIO_InitStructure.GPIO_Pin = MS5614T_SPI_SCK_PIN;
    GPIO_Init(MS5614T_SPI_SCK_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MS5614T_SPI_MOSI_PIN;
    GPIO_Init(MS5614T_SPI_MOSI_GPIO, &GPIO_InitStructure);
    
    /* ??SPI???AF */
    GPIO_PinAFConfig(MS5614T_SPI_SCK_GPIO, MS5614T_SPI_SCK_SOURCE, MS5614T_SPI_SCK_AF);
    GPIO_PinAFConfig(MS5614T_SPI_MOSI_GPIO, MS5614T_SPI_MOSI_SOURCE, MS5614T_SPI_MOSI_AF);
    
    /* ??CS, LDAC, PD????? */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = MS5614T_CS_PIN;
    GPIO_Init(MS5614T_CS_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MS5614T_LDAC_PIN;
    GPIO_Init(MS5614T_LDAC_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MS5614T_PD_PIN;
    GPIO_Init(MS5614T_PD_GPIO, &GPIO_InitStructure);
    
    /* ?????? */
    MS5614T_CS_HIGH();      // CS?????
    MS5614T_LDAC_LOW();     // LDAC?????(????)
    MS5614T_PD_HIGH();      // PD???(????)
}

/**
  * @brief  MS5614T SPI???
  */
static void MS5614T_SPI_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
    /* ??SPI?? */
    RCC_APB2PeriphClockCmd(MS5614T_SPI_CLK, ENABLE);
    
    /* SPI?? */
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  // ????
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;              // ???
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;          // 8???
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                 // ????
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;               // ????
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                  // ??NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // ???
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;         // MSB???
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    
    SPI_Init(MS5614T_SPI, &SPI_InitStructure);
    
    /* ??SPI */
    SPI_Cmd(MS5614T_SPI, ENABLE);
}

/**
  * @brief  MS5614T???
  */
void MS5614T_Init(void)
{
    MS5614T_GPIO_Init();
    MS5614T_SPI_Init();
    
    /* ????DAC???? */
    delay_us(100);
    
    /* ????????0V */
    MS5614T_SetCode(MS5614T_DAC_A, 0, MS5614T_SPEED_FAST, MS5614T_NORMAL);
    MS5614T_SetCode(MS5614T_DAC_B, 0, MS5614T_SPEED_FAST, MS5614T_NORMAL);
    MS5614T_SetCode(MS5614T_DAC_C, 0, MS5614T_SPEED_FAST, MS5614T_NORMAL);
    MS5614T_SetCode(MS5614T_DAC_D, 0, MS5614T_SPEED_FAST, MS5614T_NORMAL);
}

/**
  * @brief  ???????DAC??
  * @param  voltage: ????(V)
  * @param  vref: ????(V)
  * @retval DAC???(0-4095)
  */
uint16_t MS5614T_VoltageToCode(float voltage, float vref)
{
    uint32_t code;
    
    /* Vout = 2 * VREF * D / 4096 */
    /* D = Vout * 4096 / (2 * VREF) */
    code = (uint32_t)((voltage * MS5614T_DAC_RESOLUTION) / (2.0f * vref));
    
    /* ???? */
    if(code > 4095) code = 4095;
    
    return (uint16_t)code;
}

/**
  * @brief  ??DAC????
  * @param  channel: DAC??
  * @param  code: DAC??(0-4095)
  * @param  speed: ????
  * @param  power: ????
  */
void MS5614T_SetCode(MS5614T_Channel_t channel, uint16_t code, 
                     MS5614T_Speed_t speed, MS5614T_Power_t power)
{
    uint16_t data_word;
    uint8_t high_byte, low_byte;
    
    /* ?????? */
    if(code > 4095) code = 4095;
    
    /* ??16???? */
    /* D15-D14: ????A1-A0 */
    /* D13: PWR (????) */
    /* D12: SPD (????) */
    /* D11-D0: 12?DAC?? */
    data_word = ((uint16_t)channel << 14) | 
                ((uint16_t)power << 13) | 
                ((uint16_t)speed << 12) | 
                (code & 0x0FFF);
    
    high_byte = (uint8_t)(data_word >> 8);
    low_byte = (uint8_t)(data_word & 0xFF);
    
    /* ???? */
    MS5614T_CS_LOW();
    delay_us(1);
    
    MS5614T_SPI_SendByte(high_byte);
    MS5614T_SPI_SendByte(low_byte);
    
    delay_us(1);
    MS5614T_CS_HIGH();
    
    /* ?????? */
    if(speed == MS5614T_SPEED_FAST) {
        delay_us(6);  // ??????5.5us
    } else {
        delay_us(20); // ??????20us
    }
}

/**
  * @brief  ??DAC????
  * @param  channel: DAC??
  * @param  voltage: ????(V)
  * @param  vref: ????(V)
  */
void MS5614T_SetVoltage(MS5614T_Channel_t channel, float voltage, float vref)
{
    uint16_t code;
    
    code = MS5614T_VoltageToCode(voltage, vref);
    MS5614T_SetCode(channel, code, MS5614T_SPEED_FAST, MS5614T_NORMAL);
}

/**
  * @brief  ??DAC??(??LDAC)
  * @note   ?LDAC??????,DAC??????
  */
void MS5614T_UpdateOutput(void)
{
    MS5614T_LDAC_LOW();
    delay_us(1);
    MS5614T_LDAC_HIGH();
}

/**
  * @brief  ??DAC????
  * @param  enable: 1-??, 0-????
  */
void MS5614T_PowerDown(uint8_t enable)
{
    if(enable) {
        MS5614T_PD_LOW();  // ???????
    } else {
        MS5614T_PD_HIGH(); // ???????
    }
}
