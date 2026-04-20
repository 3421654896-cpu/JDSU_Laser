/* AI/ms5614t.c */
#include "main.h"

#define DAC_DELAY 3

uint16_t frame = 0;
uint16_t adcData = 0;
uint32_t wave_time = 10;

uint16_t IDACData[5] = {0};
uint16_t uADCOriginvalues[4] = {0};

void delay_us(__IO uint32_t us){
		uint32_t i;
		SysTick_Config(SystemCoreClock/1000000);
		for(i=0;i<us;i++){
			while(!((SysTick->CTRL)&(1<<16)));
		}
		SysTick->CTRL &=(~SysTick_CTRL_ENABLE_Msk);
}

void delay_ms(__IO uint32_t ms){
	delay_us(ms*1000);
}

void delay_s(__IO uint32_t s){
	delay_us(s*1000000);
}

inline void short_delay(volatile uint32_t n)
{
    while (n--) __NOP();
}

/* SPI2 ?? 16bit */
static void SPI2_Send16(uint16_t data)
{
    HAL_SPI_Transmit(&hspi2, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}

/* SPI3 ?? 16bit */
static void SPI3_Send16(uint16_t data)
{
    HAL_SPI_Transmit(&hspi3, (uint8_t*)&data, 1, HAL_MAX_DELAY);
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

void MS5614T_SetCode(MS5614T_Channel_t ch, uint16_t code, MS5614T_Speed_t spd, MS5614T_Power_t pwr)
{
    frame = MakeFrame(ch, code, spd, pwr);

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

void MS5614T2_SetCode(MS5614T_Channel_t ch, uint16_t code, MS5614T_Speed_t spd, MS5614T_Power_t pwr)
{
		frame = MakeFrame(ch, code, spd, pwr);

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

void write_ms5614t_table(void){
		if(!dma_transfer_complete) return;
	
		int i;
	  int j;
		uint8_t Head = 0xFF;
	
		memset(txBuffer, 0, PACK_SIZE*sizeof(uint8_t));
		txBuffer[0] = 0xEE;
		txBuffer[1] = 0xEE;
		txCount = 2;
	
		for (i = 0; i < Number;) 
		{
				if(workState == MANUAL_STATE) break;
				if(ReceEndFlag==1 && aRxBuffer[0] == Head && aRxBuffer[1] == Head) modify_table_loop();
			  // 提前把一个波长的通道数据取出来
			  for(j = 0; j < 5; j++)
			  {
					  IDACData[j] = Wave_DAC[i][j];
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
					
//						txBuffer[2] = ((i-1) >> 8) & 0xFF;
//						txBuffer[3] = (i-1) & 0xFF;
						FillPeaks(p1,p2,p3,p4);
					
//						uint8_t i_high = (i>>8) & 0xFF;
//						uint8_t i_low = i & 0xFF;
//						HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&i_high, 1);
//						HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&i_low, 1);
						sendTxBuffer(i-1, p1, p2, p3, p4);
						break;
				}
			
				MS5614T2_SetCode(MS5614T_DAC_A, IDACData[0], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T2_SetCode(MS5614T_DAC_C, IDACData[1], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T2_SetCode(MS5614T_DAC_B, IDACData[2], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T_SetCode(MS5614T_DAC_A, IDACData[3], MS5614T_SPEED_FAST, MS5614T_NORMAL);
				MS5614T_SetCode(MS5614T_DAC_C, IDACData[4], MS5614T_SPEED_FAST, MS5614T_NORMAL);
						
				sampleVoltage();
				
				adc1[i] = uADCOriginvalues[0];
				adc2[i] = uADCOriginvalues[1];
				adc3[i] = uADCOriginvalues[2];
				adc4[i] = uADCOriginvalues[3];
				
				delay_us(1);
		}
}

void write_ms5614t_manual(void){
		uint8_t Head = 0xFF;
		uint8_t flag = 0x21;
		uint16_t WriteData = 0;
		uint8_t i;
	
//		HAL_UART_Receive_DMA(&huart1, aRxBuffer, USART_RX_SIZE);
	
//		MS5614T2_SetCode(MS5614T_DAC_A, GAIN, MS5614T_SPEED_FAST, MS5614T_NORMAL);
//		MS5614T2_SetCode(MS5614T_DAC_C, SOA, MS5614T_SPEED_FAST, MS5614T_NORMAL);
//		USART_Queue_Send(&ReceEndFlag, 1);
	
		if(ReceEndFlag==0) return;
	
//		USART_Queue_Send(&flag, 1);
		
		ReceEndFlag = 0;
		
		if (aRxBuffer[0] == Head && aRxBuffer[1] == Head)
		{
				if(aRxBuffer[2] == 0x00){
						for(i = 0; i < 5; i++)
						{
								WriteData = ((aRxBuffer[3 + 2 * i] << 8) + aRxBuffer[4 + 2 * i]);
								switch(i){
									case 0:MS5614T2_SetCode(MS5614T_DAC_A, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 1:MS5614T2_SetCode(MS5614T_DAC_C, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 2:MS5614T2_SetCode(MS5614T_DAC_B, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 3:MS5614T_SetCode(MS5614T_DAC_A, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
									case 4:MS5614T_SetCode(MS5614T_DAC_C, WriteData, MS5614T_SPEED_FAST, MS5614T_NORMAL);break;
								}			
						}
						USART_Queue_Send(&flag, 1);
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

void modify_table_loop(void){
		// 0x01 change wave_time
		if(aRxBuffer[3] == 0x01){
				wave_time = (aRxBuffer[7]<<24)+(aRxBuffer[8]<<16)+(aRxBuffer[9]<<8)+aRxBuffer[10];
//				uint8_t waveArray[2] = {(wave_time>>8)&0xFF, wave_time&0xFF};
//				USART_Queue_Send(waveArray, 2);
		}
		// 0x02 switch workState
		else if(aRxBuffer[3] == 0x02){
				workState = aRxBuffer[8];
//				ClearRxBuff();
				lastGet = 0;
		}
		ClearRxBuff();
		ReceEndFlag = 0;
}

void ClearRxBuff(void){
		for (uint8_t i = 0; i < USART_RX_SIZE; i++)
	  {
				aRxBuffer[i] = 0;
		}
}

void sampleVoltage(void){
		delay_us(wave_time);
		for(uint8_t adc_idx=0;adc_idx<4;adc_idx++){
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
	
//		txBuffer[2] = 0x01;
//		txBuffer[3] = 0x02;
		
		if(txCount!=floating_size) return;
		
		dma_transfer_complete = 0;
		HAL_UART_Transmit_DMA(&huart1, txBuffer, floating_size);
}
