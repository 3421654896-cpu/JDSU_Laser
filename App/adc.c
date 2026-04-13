#include "main.h"

//static void SPI1_Send16(uint16_t data)
//{
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//    SPI_I2S_SendData(SPI1, data);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
//}

static void ADC_Select_Chs(){
		uint16_t Chs = (1<<0) | (1<<1) | (1<<2) | (1<<3);
		ADC_SPI_Cmd(Chs);
}

void ADC_SPI_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);//SCK
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);//MISO
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);//MOSI
	
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Pin = ADC_CS_PIN;
		GPIO_Init(ADC_CS_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
		GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);
		
		SPI_I2S_DeInit(SPI1);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
		
		SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
		
		SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		
		SPI_Init(SPI1, &SPI_InitStructure);
		SPI_Cmd(SPI1, ENABLE);
		
//		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
//		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE,  ENABLE);

//		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

//		NVIC_Init(&NVIC_InitStructure);
		
		ADC_CS_LOW();
		volatile uint8_t tmp;
		tmp = SPI_I2S_ReceiveData(SPI1);
		tmp = SPI1->SR;
		(void)tmp;
		
		ADC_SPI_Cmd(0x8000);
		ADC_Select_Chs();
		ADC_Loop_Start();
}

uint16_t ADC_SPI_Cmd(uint16_t cmdF){
		ADC_CS_HIGH();
		short_delay(10);
		ADC_CS_LOW();
	
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
		SPI_I2S_SendData(SPI1, cmdF);

		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		return SPI_I2S_ReceiveData(SPI1);
}

// 单通道读写需要注意2帧后才能获取到当前帧命令需要的数据
uint16_t ADC_Write_Read(uint8_t ch){
		uint16_t frame = (0x1 << 12) | (ch << 7);
	
		return ADC_SPI_Cmd(frame);
}

// ADC要2帧之后才能响应当前帧的命令
void ADC_Loop_Start(){
		ADC_SPI_Cmd(ADC_RESET_FRAME);
		ADC_SPI_Cmd(ADC_ENTER_FRAME);
}

uint16_t ADC_Write_Loop(){
		return ADC_SPI_Cmd(ADC_LOOP_FRAME);
}