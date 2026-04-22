#include "main.h"

uint16_t temperature = 0;

void DQ_IN(void){
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = TEMP_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(TEMP_PORT, &GPIO_InitStruct);
}

void DQ_OUT(void){
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = TEMP_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(TEMP_PORT, &GPIO_InitStruct);
}

uint8_t M1820Z_Reset(void){
		uint8_t response;

    DQ_OUT();
    HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_RESET);
    delay_us(600); // 480us+

    HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_SET);
    DQ_IN();

    delay_us(600);

    response = HAL_GPIO_ReadPin(TEMP_PORT, TEMP_PIN);

    delay_us(600);

    return response;
}

void M1820Z_WriteBit(uint8_t bit){
		DQ_OUT();

		HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_RESET);
		
		if(bit)
				HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_SET);

		HAL_Delay(1);

		HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_SET);
}

uint8_t M1820Z_ReadBit(void){
		uint8_t bit = 0;

		DQ_OUT();
		HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, GPIO_PIN_RESET);

		DQ_IN();

		if(HAL_GPIO_ReadPin(TEMP_PORT, TEMP_PIN))
				bit = 1;

//		HAL_Delay(1);

		return bit;
}

void M1820Z_WriteByte(uint8_t data){
		//Byte from low to high
		for(int i=0;i<8;i++)
		{
				M1820Z_WriteBit(data & 0x01);
				data >>= 1;
		}
}

uint8_t M1820Z_ReadByte(void){
		uint8_t data = 0;

		for(int i=0;i<8;i++)
		{
				data >>= 1;
				if(M1820Z_ReadBit())
						data |= 0x80;
		}
		return data;
}

float M1820Z_GetTmp(void){
		uint8_t temp_l, temp_h;
    short temp;

    M1820Z_Reset();
    M1820Z_WriteByte(0xCC);
    M1820Z_WriteByte(0x44);

    HAL_Delay(11);

    M1820Z_Reset();
    M1820Z_WriteByte(0xCC);
    M1820Z_WriteByte(0xBE);

    temp_l = M1820Z_ReadByte();
    temp_h = M1820Z_ReadByte();

    temp = (temp_h << 8) | temp_l;
	
		float tmpResult = 40.f+(float)temp/256.f;

    return tmpResult;
}
