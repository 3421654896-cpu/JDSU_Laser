#include "main.h"

uint16_t adcSPI = 0;

static void ADC_Select_Chs(){
		uint16_t Chs = (1<<0) | (1<<1) | (1<<2) | (1<<3);
		ADC_SPI_Cmd(Chs);
}

void ADC_SPI_Init(void){
		volatile uint8_t tmp;
		HAL_SPI_Receive(&hspi1, (uint8_t*)&tmp, 1, HAL_MAX_DELAY);
		(void)tmp;
		
		ADC_SPI_Cmd(0x8000);
		ADC_Select_Chs();
		ADC_Loop_Start();
}

uint16_t ADC_SPI_Cmd(uint16_t cmdF){
		ADC_CS_HIGH();
		short_delay(10);
		ADC_CS_LOW();
	
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&cmdF, (uint8_t*)&adcData, 1, HAL_MAX_DELAY);
	
		return adcData;//todo:compare the queue data
}

uint16_t ADC_Write_Read(uint8_t ch){
		uint16_t frame = (0x1 << 12) | (ch << 7);
	
		return ADC_SPI_Cmd(frame);
}

void ADC_Loop_Start(void){
		ADC_SPI_Cmd(ADC_RESET_FRAME);
		ADC_SPI_Cmd(ADC_ENTER_FRAME);
}

uint16_t ADC_Write_Loop(void){
		return ADC_SPI_Cmd(ADC_LOOP_FRAME);
}
