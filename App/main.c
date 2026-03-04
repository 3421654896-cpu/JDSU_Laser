/* App/main.c */
#include "main.h"

uint8_t test = 0x21;
uint8_t getData = 0;
uint8_t workState = TABLE_STATE;
uint16_t txCount = 0;
uint8_t txBuffer[PACK_SIZE] = {0};

int main(void)
{
//    const uint16_t code_0p5V = 410;  // 0.5V @ Vref=2.5V
//    const uint16_t code_1V   = 819;  // 1.0V @ Vref=2.5V

//    /* ?1?:??? 1V,???????????? */
//    MS5614T_SetCode(MS5614T_DAC_A, 900, MS5614T_SPEED_SLOW, MS5614T_NORMAL);
//    MS5614T_SetCode(MS5614T_DAC_B, 1064, MS5614T_SPEED_SLOW, MS5614T_NORMAL);//<1064
//    MS5614T_SetCode(MS5614T_DAC_C, 800, MS5614T_SPEED_SLOW, MS5614T_NORMAL);
//    MS5614T_SetCode(MS5614T_DAC_D, 0, MS5614T_SPEED_SLOW, MS5614T_NORMAL);
		//Clock_Init();
	
		NVIC_Config();
		USART_Config();
	
		MS5614T_Init();    // ?1?(SPI3)
    MS5614T2_Init();   // ?2?(SPI2)
		ADC_SPI_Init();
		BUTTON_Config();
		LED_Config();
	
    while (1)
    {                                                                                                                           
        /* ?2?:A/C 0.5V */
//        uint16_t getAdc = ADC_Write_Read();
//				adcData[0] = 0x01;
//				adcData[1] = 0x22;
//				adcData[0] = getAdc >> 8 & 0xFF;
//				adcData[1] = getAdc & 0xFF;
//				USART_DMA_Send(&test, 1);
//   			delay_us(10);
//				USART_DMA_Send(&workState, 1);
				if(workState == MANUAL_STATE)
				{
					LED_TABLE_LOW();
					LED_MANUAL_HIGH();
					write_ms5614t_manual();
				}
				else
				{
					LED_MANUAL_LOW();
					LED_TABLE_HIGH();
					write_ms5614t_table();
				}
//				getAdc = ADC_Write_Read();
//				adcData[0] = getAdc >> 8 & 0xFF;
//				adcData[1] = getAdc & 0xFF;
				//USART_DMA_Send(adcData, 2);
//				USART_SendData(USART1, getData);
				
				delay_us(5);
    }
}
