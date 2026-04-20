#include "main.h"

uint16_t adcSPI = 0;
uint8_t adcCount = 0;
uint8_t adcStable = 0;
uint16_t adcQueue[10] = {0};

void Reset_ADC_Queue(void){
		for(uint8_t i=0;i<10;i++){
				adcQueue[i] = 0;
		}
		adcCount = 0;
		adcStable = 0;
}

static void ADC_Select_Chs(){
		uint16_t Chs = (1<<0) | (1<<1) | (1<<2) | (1<<3);
		ADC_SPI_Cmd(Chs);
}

void ADC_LOOP_SPI_Init(void){
		volatile uint8_t tmp;
		__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
		(void)tmp;
		
		ADC_SPI_Cmd(0x8000);
		ADC_Select_Chs();
		ADC_Loop_Start();
}

void ADC_MANUAL_SPI_Init(void){
		volatile uint8_t tmp;
		__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
		(void)tmp;
	
		ADC_SPI_Cmd(0x8000);
}

uint16_t ADC_SPI_Cmd(uint16_t cmdF){
		ADC_CS_HIGH();
		short_delay(10);
		ADC_CS_LOW();
	
		HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&cmdF, (uint8_t*)&adcSPI, 1, HAL_MAX_DELAY);
	
		return adcSPI;
}

uint16_t ADC_Write_Read(uint8_t ch){
		uint16_t frame = (0x1 << 12) | (ch << 7);
		Reset_ADC_Queue();
	
		while(adcStable<5){
				adcQueue[adcCount++] = ADC_SPI_Cmd(frame);
				if(adcCount==10){
						uint16_t adcSum = 0;
						for(uint8_t i=1;i<10;i++){
								adcSum+=adcQueue[i];
						}
						return adcSum/9;
				}
				if(adcCount>2 && abs((int)adcQueue[adcCount-1]-(int)adcQueue[adcCount-2])<410) adcStable++;
				else adcStable=0;
		}
		return adcQueue[adcCount-1];
}

void ADC_Loop_Start(void){
		ADC_SPI_Cmd(ADC_RESET_FRAME);
		ADC_SPI_Cmd(ADC_ENTER_FRAME);
}

uint16_t ADC_Write_Loop(void){
		return ADC_SPI_Cmd(ADC_LOOP_FRAME);
}
