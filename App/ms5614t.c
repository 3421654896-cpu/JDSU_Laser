/* AI/ms5614t.c */
#include "main.h"

#define DAC_DELAY 3

uint16_t wave_time = 10;

uint16_t uADCOriginvalues[4] = {0};

/* ------------------ ???:???? FS/CS ?????(ns?,?NOP??) ------------------ */
static inline void short_delay(volatile uint32_t n)
{
    while (n--) __NOP();
}

/* SPI3 ?? 16bit */
static void SPI3_Send16(uint16_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, data);
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY) == SET);
}

/* SPI2 ?? 16bit */
static void SPI2_Send16(uint16_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
}

/* ??:D15..D12 = A1 A0 PWR SPD, D11..D0 = code */
static uint16_t MakeFrame(MS5614T_Channel_t ch, uint16_t code, MS5614T_Speed_t spd, MS5614T_Power_t pwr)
{
    if (code > 4095u) code = 4095u;

    return (uint16_t)((((uint16_t)ch  & 0x03u) << 14) |
                      (((uint16_t)pwr & 0x01u) << 13) |
                      (((uint16_t)spd & 0x01u) << 12) |
                      (code & 0x0FFFu));
}

/* ===================== DAC1:Init / SetCode ===================== */
void MS5614T_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

		/* SPI2: PB13(SCK), PB15(MOSI) -> AF5 */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
    
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ???:?? */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    GPIO_InitStructure.GPIO_Pin = DAC1_FS_PIN;   GPIO_Init(DAC1_FS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = DAC1_CS_PIN;   GPIO_Init(DAC1_CS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = DAC1_LDAC_PIN; GPIO_Init(DAC1_LDAC_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = DAC1_PD_PIN;   GPIO_Init(DAC1_PD_PORT, &GPIO_InitStructure);

    SPI_I2S_DeInit(SPI2);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_16b;

    /* ?????:????? + ??? -> Mode 2 */
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;

    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);

    DAC1_CS_HIGH();
    DAC1_FS_HIGH();
    DAC1_PD_HIGH();
    DAC1_LDAC_LOW();
}

void MS5614T_SetCode(MS5614T_Channel_t ch, uint16_t code, MS5614T_Speed_t spd, MS5614T_Power_t pwr)
{
    uint16_t frame = MakeFrame(ch, code, spd, pwr);

    if (pwr == MS5614T_POWERDOWN) DAC1_PD_LOW(); else DAC1_PD_HIGH();
    DAC1_LDAC_LOW();

    DAC1_FS_HIGH();
    short_delay(DAC_DELAY);

    DAC1_CS_LOW();
    short_delay(DAC_DELAY);

    DAC1_FS_LOW();
    short_delay(DAC_DELAY);

    SPI2_Send16(frame);

    short_delay(DAC_DELAY);
    DAC1_FS_HIGH();
    short_delay(DAC_DELAY);
    DAC1_CS_HIGH();
}

/* ===================== DAC2:Init / SetCode ===================== */
void MS5614T2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    /* SPI3: PC10(SCK), PC12(MOSI) -> AF6 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* CS PB12?FS PB14:?? */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Pin  = DAC2_FS_PIN; GPIO_Init(DAC2_FS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = DAC2_CS_PIN; GPIO_Init(DAC2_CS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = DAC1_LDAC_PIN;GPIO_Init(DAC2_LDAC_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = DAC1_PD_PIN;  GPIO_Init(DAC2_PD_PORT, &GPIO_InitStructure);

    SPI_I2S_DeInit(SPI3);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_16b;

    /* ?????:????? + ??? -> Mode 2 */
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;

    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI3, &SPI_InitStructure);
    SPI_Cmd(SPI3, ENABLE);

    DAC2_CS_HIGH();
    DAC2_FS_HIGH();
    DAC2_PD_HIGH();
    DAC2_LDAC_LOW();
}

void MS5614T2_SetCode(MS5614T_Channel_t ch, uint16_t code, MS5614T_Speed_t spd, MS5614T_Power_t pwr)
{
    uint16_t frame = MakeFrame(ch, code, spd, pwr);

    if (pwr == MS5614T_POWERDOWN) DAC2_PD_LOW(); else DAC2_PD_HIGH();
    DAC2_LDAC_LOW();

    DAC2_FS_HIGH();
    short_delay(DAC_DELAY);

    DAC2_CS_LOW();
    short_delay(DAC_DELAY);

    DAC2_FS_LOW();
    short_delay(DAC_DELAY);

    SPI3_Send16(frame);

    short_delay(DAC_DELAY);
    DAC2_FS_HIGH();
    short_delay(DAC_DELAY);
    DAC2_CS_HIGH();
}

void write_ms5614t_table(){
		int i;
	  int j;
		uint8_t Head = 0xFF;
		u16 IDACData[3];	
	
		MS5614T2_SetCode(MS5614T_DAC_A, GAIN, MS5614T_SPEED_FAST, MS5614T_NORMAL);
		MS5614T2_SetCode(MS5614T_DAC_C, SOA, MS5614T_SPEED_FAST, MS5614T_NORMAL);
		
		memset(txBuffer, 0, PACK_SIZE*sizeof(uint8_t));
		txBuffer[0] = 0xEE;
		txBuffer[1] = 0xEE;
		txCount = 2;
	
		for (i = 0; i < Number;) 
		{
				if(workState == MANUAL_STATE) break;
				if(ReceEndFlag==1 && aRxBuffer[0] == Head && aRxBuffer[1] == Head) modify_table_loop();
			  // 提前把一个波长的通道数据取出来
			  for(j = 0; j < 3; j++)
			  {
					  IDACData[j] = ((u16*)&Wave_DAC[i])[j];
				}
				i++;
			
				// 波长数据0x00就跳过
				if ((IDACData[0] == 0xFFFF) && (IDACData[1] == 0xFFFF) && (IDACData[2] == 0xFFFF)){
						uint8_t p1 = Find_Peaks(adc1, peaks1, i-1);
						uint8_t p2 = Find_Peaks(adc2, peaks2, i-1);
						uint8_t p3 = Find_Peaks(adc3, peaks3, i-1);
						uint8_t p4 = Find_Peaks(adc4, peaks4, i-1);
				
						txBuffer[txCount++] = ((i-1) >> 8) & 0xFF;
						txBuffer[txCount++] = (i-1) & 0xFF;
						FillPeaks(p1,p2,p3,p4);
					
						sendTxBuffer(i-1, p1, p2, p3, p4);
						break;
				}
			
				MS5614T_SetCode(MS5614T_DAC_A, IDACData[1], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T_SetCode(MS5614T_DAC_C, IDACData[2], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T2_SetCode(MS5614T_DAC_B, IDACData[0], MS5614T_SPEED_FAST, MS5614T_NORMAL);
						
				sampleVoltage();
				
				adc1[i] = uADCOriginvalues[0];
				adc2[i] = uADCOriginvalues[1];
				adc3[i] = uADCOriginvalues[2];
				adc4[i] = uADCOriginvalues[3];
				
				delay_us(wave_time);
		}
}

void write_ms5614t_manual(){
		uint8_t Head = 0xFF;
		uint8_t flag = 0x21;
		u16 WriteData = 0;
		u8 i;
	
//		MS5614T2_SetCode(MS5614T_DAC_A, GAIN, MS5614T_SPEED_FAST, MS5614T_NORMAL);
//		MS5614T2_SetCode(MS5614T_DAC_C, SOA, MS5614T_SPEED_FAST, MS5614T_NORMAL);
	
		if (ReceEndFlag != 1)
			return ;
	
		ReceEndFlag = 0;
		
//		USART_DMA_Send(aRxBuffer, USART_RX_SIZE);
		
		if (aRxBuffer[0] == Head && aRxBuffer[1] == Head)
		{
				if(aRxBuffer[2] == 0x00){
						for(i = 0; i < 5; i++)
						{
								WriteData = ((aRxBuffer[3 + 2 * i] << 8) + aRxBuffer[4 + 2 * i]);
								switch(i){
									case 0:MS5614T2_SetCode(MS5614T_DAC_A, GAIN, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 1:MS5614T2_SetCode(MS5614T_DAC_C, SOA, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 2:MS5614T2_SetCode(MS5614T_DAC_B, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 3:MS5614T_SetCode(MS5614T_DAC_A, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 4:MS5614T_SetCode(MS5614T_DAC_C, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
								}			
						}
						USART_DMA_Send(&flag, 1);
						ClearRxBuff();
				}
				else if(aRxBuffer[2] == 0x01){
						modify_table_loop();
				}
		}
		if ((aRxBuffer[0] != Head) && (aRxBuffer[0] != 0x00))
		{
				ClearRxBuff();
		}
}

void modify_table_loop(){
		// 0x01 change wave_time
		if(aRxBuffer[3] == 0x01){
				wave_time = (aRxBuffer[7]<<8)+aRxBuffer[8];
//				uint8_t waveArray[2] = {(wave_time>>8)&0xFF, wave_time&0xFF};
//				USART_DMA_Send(waveArray, 2);
		}
		// 0x02 switch workState
		else if(aRxBuffer[3] == 0x02){
				workState = aRxBuffer[8];
				ClearRxBuff();
				lastGet = 0;
		}
		ClearRxBuff();
		ReceEndFlag = 0;
}

void ClearRxBuff(){
		for (u8 i = 0; i < USART_RX_SIZE; i++)
	  {
				aRxBuffer[i] = 0;
		}
}

void sampleVoltage(){
		uint16_t adcData = 0;
		for(u8 adc_idx=0;adc_idx<4;adc_idx++){
				adcData = ADC_Write_Loop() & 0x0FFF;
				uADCOriginvalues[adc_idx] = adcData;
				txBuffer[txCount++] = (adcData >> 8) & 0xFF;
				txBuffer[txCount++] = adcData & 0xFF;
		}
}

void sendTxBuffer(int dac_size, int p1, int p2, int p3, int p4){
		int floating_size = 4+8*dac_size+3+4+4*(p1+p2+p3+p4);
	
		txBuffer[txCount++] = 0xFF;
		txBuffer[txCount++] = 0xFF;
		
		if(txCount!=floating_size) return;
		USART_DMA_Send(txBuffer, floating_size);
}
