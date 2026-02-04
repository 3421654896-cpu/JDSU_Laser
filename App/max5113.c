#include "main.h"

atloc(0x08006000)
const u16 Write_Reg[21][2] = {{0x00, 0x8000}, 
                        {0x01, 0x0800}, 
                        {0x02, 0x0800}, 
                        {0x03, 0x0000}, 
                        {0x04, 0x0800}, 
                        {0x05, 0x0800}, 
                        {0x06, 0x0800}, 
                        {0x07, 0x0800}, 
                        {0x08, 0x0000}, 
                        {0x09, 0x0000}, 
		                    {0x0F, 0x0000}, 
                        {0x11, 0x0000}, // IDAC1 FS1
                        {0x12, 0x0000}, // IDAC2 FS2
                        {0x13, 0x0000}, // IDAC3 FS3
                        {0x14, 0x0000}, // IDAC4 Rear
                        {0x15, 0xC718}, // IDAC5 GAIN  MAX=180mA(0x3FFF),140mA=0x31C6左移两位后为0xC718
                        {0x16, 0x4CC8}, // IDAC6 SOA   MAX=300mA(0x3FFF),90mA=0x1332左移两位后为0x4CC8
                        {0x17, 0x0E38}, // IDAC7 Phase MAX=90mA(0x3FFF),5mA=0x038E左移两位后为0x0E38
                        {0x18, 0x0000}, // IDAC8 FS4
                        {0x19, 0x0000}, // IDAC9 FS5
                        {0x1F, 0x0000}};

atloc(0x08006100)												
const u8 IDACAddr[5] = {0x15, 0x16, 0x17, 0x18, 0x19};

u32 Wave_Time = 50;

//void Test_Write_Max5113(void)
//{
//		int i;
//	  int j;
//		u8 addr;
//		u16 data;	
//	
//		for (i = 0; i < 3000; i++)
//		{
//				for(j = 13; j < 20; j++)
//			  {
//						addr = Write_Reg[j][0];
//						data = Write_Reg[j][1];
//						//MySPI_WriteData(addr, data);
//					
//						MySPI_CLR_L();
//						MySPI_CLR_H();
//						delay2(1);         
//						MySPI_CLR_L();	
//						
//						sFLASH_CS_LOW();
//						delay2(1);

//					  SPI2_ReadWriteByte(addr);
//					  SPI2_ReadWriteByte((data >> 8) & 0xFF);
//					  SPI2_ReadWriteByte((data >> 0) & 0xFF);
//					
//						delay2(70);     // 延时18us，DAC生效时间15us
//						sFLASH_CS_HIGH();
//						//delay2(30); 
//				}
//		}
//}

void sampleVoltage(){
		//获取时间
//		uint32_t now_us, elapsed_us;
//		now_us = Get_Micros();
//		elapsed_us = now_us-start_us;
//		USART_SendData(USART1, (elapsed_us>>24)&0xFF);
//		USART_SendData(USART1, 0x2F);
//		USART_SendData(USART1, (elapsed_us>>16)&0xFF);
//		USART_SendData(USART1, 0x2F);
//		USART_SendData(USART1, (elapsed_us>>8)&0xFF);
//		USART_SendData(USART1, 0x2F);
//		USART_SendData(USART1, (elapsed_us>>0)&0xFF);
//		USART_SendData(USART1, 0x2F);
		
		//采集
//		uint16_t _Data0 = uSyncConvertedvalues[0];
//		USART_SendData(USART1, (_Data0>>8)&0xFF);
//		delay_us(1);
//		USART_SendData(USART1, (_Data0>>0)&0xFF);
//		delay_us(1);
		
//		uint16_t _Data1 = uSyncConvertedvalues[1];
//		USART_SendData(USART1, (_Data1>>8)&0xFF);
//		delay_us(1);
//		USART_SendData(USART1, (_Data1>>0)&0xFF);
//		delay_us(1);
		
//		uint16_t _Data2 = uSyncConvertedvalues[2];
//		USART_SendData(USART1, (_Data2>>8)&0xFF);
//		delay_us(1);
//		USART_SendData(USART1, (_Data2>>0)&0xFF);
//		delay_us(1);
		
//		uint16_t _Data3 = uSyncConvertedvalues[3];
//		USART_SendData(USART1, (_Data3>>8)&0xFF);
//		delay_us(1);
//		USART_SendData(USART1, (_Data3>>0)&0xFF);
//		delay_us(1);

//		tx_buffer[0] = (_Data0>>8)&0xFF;
//		tx_buffer[1] = (_Data0>>0)&0xFF;
//		
//		tx_buffer[2] = (_Data1>>8)&0xFF;
//		tx_buffer[3] = (_Data1>>0)&0xFF;
//		
//		tx_buffer[4] = (_Data2>>8)&0xFF;
//		tx_buffer[5] = (_Data2>>0)&0xFF;
//		
//		tx_buffer[6] = (_Data3>>8)&0xFF;
//		tx_buffer[7] = (_Data3>>0)&0xFF;
//		
//		tx_buffer[0] = 0xFF;
//		tx_buffer[1] = 0x0F;
//		
//		tx_buffer[2] = 0xFF;
//		tx_buffer[3] = 0x0F;
//		
//		tx_buffer[4] = 0xFF; 
//		tx_buffer[5] = 0x0F;
//		
//		tx_buffer[6] = 0xFF;
//		tx_buffer[7] = 0x0F;

//			uSyncConvertedvalues[0] = 0xFF;
//			uSyncConvertedvalues[1] = 0x0F;
//			
//			uSyncConvertedvalues[2] = 0xFF;
//			uSyncConvertedvalues[3] = 0x0F;
//			
//			uSyncConvertedvalues[4] = 0xFF; 
//			uSyncConvertedvalues[5] = 0x0F;
//			
//			uSyncConvertedvalues[6] = 0xFF;
//			uSyncConvertedvalues[7] = 0x0F;
		
//	  while(!dma_transfer_complete);
			USART_DMA_Send(uSyncConvertedvalues, 8);
//			if(item==2999) Find_Peak();
//			item = (item+1)%3000;
}
		
void Init_Max5113(void)
{
		int i;
		u8 addr;
	  u16 data;	
	
		for(i = 0; i < 21; i++)
		{
				addr = ((Write_Reg[i][0] << 1) | 0x00);
				data = Write_Reg[i][1];		
			
				MySPI_CLR_L();
				MySPI_CLR_H();
				delay_us(1);         
				MySPI_CLR_L();	
			
				sFLASH_CS_LOW();
				delay_us(1);

				SPI2_ReadWriteByte(addr);
				SPI2_ReadWriteByte((data >> 8) & 0xFF);
				SPI2_ReadWriteByte((data >> 0) & 0xFF);
			
				delay_us(18);     // 延时18us，DAC生效时间15us
				sFLASH_CS_HIGH();
				//delay2(30); 
		}
		
}			
		
void Test_Write_Max5113_2(void)
{
		int i;
	  int j;
		u8 addr;
	  u8 dataH,dataL;
		u16 IDACData[5];	
	  u16 data;	
		
		uint8_t front[2] = {0xEE,0xEE};
		uint8_t back[2] = {0xFF,0xFF};
		USART_DMA_Send(front, 2);
		for (i = 0; i < Number;)
		{
				if ((Read_Or_Write == 0x55) || (Open_Or_Close == 0x55))
				{
						break;
				}
				
//				if(Continue_Read_Flash==0x21) continue;
			  // 提前把一个波长的通道数据取出来
			  for(j = 0; j < 5; j++)
			  {
					  IDACData[j] = ((u16*)&Wave_DAC[i])[j];
				}
				i++;
			
				// 波长数据0x00就跳过
				if ((IDACData[0] == 0x00) && (IDACData[1] == 0x00) && (IDACData[2] == 0x00) && (IDACData[3] == 0x00) && (IDACData[4] == 0x00)){
						uint8_t p1 = Find_Peaks(adc1, peaks1, i);
						uint8_t p2 = Find_Peaks(adc2, peaks2, i);
						uint8_t p3 = Find_Peaks(adc3, peaks3, i);
						uint8_t p4 = Find_Peaks(adc4, peaks4, i);
				
						SendPeaks(p1,p2,p3,p4);
						
						break;
				}
				
				// 波长数据0xFF就跳过
//				if ((IDACData[0] == 0xFFFF) && (IDACData[1] == 0xFFFF) && (IDACData[2] == 0xFFFF) && (IDACData[3] == 0xFFFF) && (IDACData[4] == 0xFFFF))
//					continue;
				
				for(j = 0; j < 5; j++)
			  {
						addr = ((IDACAddr[j] << 1) | 0x00);
					  data = (IDACData[j] << 2);			         // 数据寄存器低两位是无效的
            dataH = ((data >> 8) & 0xFF);		
					  dataL = ((data >> 0) & 0xFF);	
					
						MySPI_CLR_L();
						MySPI_CLR_H();
						delay_us(1);         
						MySPI_CLR_L();	
					
						sFLASH_CS_LOW();
						delay_us(1);

					  SPI2_ReadWriteByte(addr);
					  SPI2_ReadWriteByte(dataH);
					  SPI2_ReadWriteByte(dataL);
					
						//delay2(25);     // 70 延时18us，DAC生效时间15us   //镀金杜邦线16达到2MHZ
					  delay_us(18);       // 70 延时18us，DAC生效时间15us   //跳线128达到600KHZ
						sFLASH_CS_HIGH();
						//delay2(7000);    //2ms
						
				}
				
				sampleVoltage();
				
				adc1[i] = uADCConvertedvalues[0];
				adc2[i] = uADCConvertedvalues[1];
				adc3[i] = uADCConvertedvalues[2];
				adc4[i] = uADCConvertedvalues[3];
				
				delay_us(Wave_Time);
				
		}
		USART_DMA_Send(back, 2);
}	



// 写数据到Flash中
void Write_Flash_Data(u16 addr, u8 *data)
{
	  u8 i;
		u32 uwAddress = 0;
	
	  // 起始地址加偏移地址
		uwAddress = (Para_Addr + addr);
	
	  // 循环把数据写入到FLASH 
		for (i = 0; i < 10; i++)
	  {
				FLASH_ProgramByte(uwAddress + i, data[i]);
		}
}



/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  
#if defined (USE_STM324xG_EVAL)
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

#else /* USE_STM324x7I_EVAL or USE_STM324x9I_EVAL */  
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_Sector_23;  
  }
#endif /* USE_STM324x7I_EVAL or USE_STM324x9I_EVAL */
  return sector;
}

// 擦除数据
void Erase_Data(void)
{
	uint32_t i;
	uint32_t data[8];
	uint32_t addr = 0x0801C350;
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;
//	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;	
	
  /* Unlock the Flash *********************************************************/
  /* Enable the flash control register access */
  FLASH_Unlock();	
	
	// 先把0x0801C350地址数据取出来
	for (i = 0; i < 8; i++)
	{
			data[i] = *(__IO uint32_t*)(addr + 4 * i);
	}
	
  /* Erase the user Flash area ************************************************/
  /* area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 	
	
  /* Get the number of the start and end sectors */
  uwStartSector = GetSector(FLASH_USER_START_ADDR);
  uwEndSector = GetSector(FLASH_USER_END_ADDR);	
	
	/* Strat the erase operation */
  uwSectorCounter = uwStartSector;
  while (uwSectorCounter <= uwEndSector) 
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /* Error occurred while sector erase. 
         User can add here some code to deal with this error  */
      while (1)
      {
      }
    }
    /* jump to the next sector */
    if (uwSectorCounter == FLASH_Sector_11)
    {
      uwSectorCounter += 40;
    } 
    else 
    {
      uwSectorCounter += 8;
    }
  }
	
	// 擦除完后再把0x0801C350地址数据写回去
	for (i = 0; i < 8; i++)
	{
		  FLASH_ProgramWord((addr + 4 * i), data[i]);
	}
}


void Write_Flash_Data_OK(void)
{
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  FLASH_Lock(); 	
}


//读出所有寄存器数据
void Read_Max5113(void)
{
		int i;
		u8 data1, data2;
		u8 addr;
		u16 data;
		
		for (i = 0; i < 21; i++)
		{
				addr = Write_Reg[i][0];
			  data = MySPI_ReadData(addr);
			
			  //串口显示数据
				data1 = ((data >> 8) & 0xFF);
				data2 = ((data >> 0) & 0xFF);
				USART_SendData(USART1, data1);//将res在发送给串口1
				delay(100);
				USART_SendData(USART1, data2);//将res在发送给串口1
				delay(100);		
		}		
		
		addr = 0x0E;
		data = MySPI_ReadData(addr);
		
		//串口显示数据
		data1 = ((data >> 8) & 0xFF);
		data2 = ((data >> 0) & 0xFF);
		USART_SendData(USART1, data1);//将res在发送给串口1
		delay(100);
		USART_SendData(USART1, data2);//将res在发送给串口1
		delay(100);		
}

// 清空串口接收数组
void ClearRxBuff(void)
{
	  u8 i;
	
	  for (i = 0; i < 7; i++)
	  {
				aRxBuffer[i] = 0;
		}
}

// 解析串口收到的数据
void AnalysisUartData(void)
{
	  u8 i;
	  u8 WriteDataAddr = 0x15;
		u8 addr1,addr2;
	  u8 data1,data2;
		u8 data[10];
	  u16 WriteData;
		u8 Head = 0xFE;
//	  u8 End = 0xFE;
	  u16 indexAddr = 0; 
    u32 itime;	

	  // 没用接收完数据不处理
	  if (ReceEndFlag != 1)
			return ;
	
		// 清除标志
		ReceEndFlag = 0;
		
		if (aRxBuffer[0] == Head)
		{
//			  if (aRxBuffer[1] == 0xAA)   // 轮询模式
//				{
//						Read_Or_Write = 0xAA;
//				}
//				else if (aRxBuffer[1] == 0x55)   // 手动调试模式
//				{
//						Read_Or_Write = 0x55;
//				}		
			  // 读数据
				if (aRxBuffer[1] == 0x01)
				{
					  // 取地址
						addr1 = ((aRxBuffer[4] << 1) | 0x01);   // 读数据  

					  ////////////////////////////////
						MySPI_CLR_L();
						MySPI_CLR_H();
						delay2(1);         
						MySPI_CLR_L();	
					
						sFLASH_CS_LOW();
						delay2(1);
					  ///////////////////////////////
					
					  // 读取寄存器数据
						SPI2_ReadWriteByte(addr1);
						data1 = SPI2_ReadWriteByte(sFLASH_DUMMY_BYTE);
						data2 = SPI2_ReadWriteByte(sFLASH_DUMMY_BYTE);
					
						//串口发送数据
						USART_SendData(USART1, data1);
					  delay(10);
						USART_SendData(USART1, data2);	
					  delay(10);
					 
					  ////////////////////////////
					  //delay2(25);     // 70 延时18us，DAC生效时间15us   //镀金杜邦线16达到2MHZ
						delay2(70);       // 70 延时18us，DAC生效时间15us   //跳线128达到600KHZ
						sFLASH_CS_HIGH();
						//delay2(30); 
					  ////////////////////////////

					  // 清空接收数组
						ClearRxBuff();
				}
				// 写寄存器数据
				else if (aRxBuffer[1] == 0x20)
				{
						for(i = 0; i < 5; i++)
						{
								WriteDataAddr = ((IDACAddr[i] << 1) | 0x00);  // 地址
							  WriteData = ((aRxBuffer[4 + 2 * i] << 8) + aRxBuffer[5 + 2 * i]);    // 寄存器数据
								WriteData <<= 2;                                                     // 寄存器数据
								data1 = ((WriteData >> 8) & 0xFF);
								data2 = ((WriteData >> 0) & 0xFF);
			
								////////////////////////////////
								MySPI_CLR_L();
								MySPI_CLR_H();
								delay2(1);         
								MySPI_CLR_L();	
							
								sFLASH_CS_LOW();
								delay2(1);
								///////////////////////////////					
							
								// 写寄存器数据
								SPI2_ReadWriteByte(WriteDataAddr);
								SPI2_ReadWriteByte(data1);
								SPI2_ReadWriteByte(data2);
								

								////////////////////////////
								//delay2(25);     // 70 延时18us，DAC生效时间15us   //镀金杜邦线16达到2MHZ
								delay2(70);       // 70 延时18us，DAC生效时间15us   //跳线128达到600KHZ
								sFLASH_CS_HIGH();
								//delay2(30); 
								////////////////////////////
						}	
						//USART_SendData(USART1, 0x21);
						//delay(100);
						
						sampleVoltage();
					
					  // 清空接收数组
						ClearRxBuff();				
				}
				// 写FLASH数据
				else if (aRxBuffer[1] == 0x02)
				{
						addr1 = aRxBuffer[2];       // indexAddr M
					  addr2 = aRxBuffer[3];       // indexAddr M
					  data[0] = aRxBuffer[4];     // Chan1 M
					  data[1] = aRxBuffer[5];     // Chan1 L
						data[2] = aRxBuffer[6];     // Chan2 M
					  data[3] = aRxBuffer[7];     // Chan2 L
						data[4] = aRxBuffer[8];     // Chan3 M
					  data[5] = aRxBuffer[9];     // Chan3 L
						data[6] = aRxBuffer[10];    // Chan4 M
					  data[7] = aRxBuffer[11];    // Chan4 L
						data[8] = aRxBuffer[12];    // Chan5 M
					  data[9] = aRxBuffer[13];    // Chan5 L

					  // 得到偏移地址
					  indexAddr = ((addr1 << 8) | addr2);
					
					  // 写Flash数据
						Write_Flash_Data(indexAddr, data);
					
					  // 清空接收数组
						ClearRxBuff();
				}
				// 结束写FLASH数据
				else if (aRxBuffer[1] == 0x04)				
				{ 
					  // 开启FLASH锁
						Write_Flash_Data_OK();		
				}
				// 开始写FLASH数据
				else if (aRxBuffer[1] == 0x08)				
				{
				    // 擦除数据
					  Erase_Data();
				}		
				// 修改轮询模式下波长间隔时间
				else if (aRxBuffer[1] == 0x40)				
				{
						itime = (aRxBuffer[4] << 24);
					  itime += (aRxBuffer[5] << 16);
						itime += (aRxBuffer[6] << 8);
					  itime += (aRxBuffer[7] << 0);
					
						Wave_Time = itime;
				}	
			  // 读TEC温度数据
				if (aRxBuffer[1] == 0x1B)
				{
            // 读TEC温度
            WriteData = Monitor_Thermister(uhADCxConvertedValue);
            data1 = ((WriteData >> 8) & 0xFF);
         		data2 = ((WriteData >> 0) & 0xFF);
          
						//串口发送数据
						USART_SendData(USART1, data1);
					  delay(10);
						USART_SendData(USART1, data2);	
					  delay(10);

					  // 清空接收数组
						ClearRxBuff();
				}				
		}
		
		if ((aRxBuffer[0] != Head) && (aRxBuffer[0] != 0x00))
		{
			  // 清空接收数组
				ClearRxBuff();
		}
		
//		if ((aRxBuffer[1] != 0x01) || (aRxBuffer[1] != 0x02) || (aRxBuffer[1] != 0x04) || (aRxBuffer[1] != 0x08))
//		{
//			  // 清空接收数组
//				ClearRxBuff();
//		}
}



void CloseOpwer(void)
{
	  u8 i;
	  u8 WriteDataAddr = 0x15;
	  u8 data1,data2;
	  u16 WriteData;
  
    for(i = 0; i < 5; i++)
    {
        WriteDataAddr = ((IDACAddr[i] << 1) | 0x00);  // 地址
        WriteData = 0x00;                             // 寄存器数据
        WriteData <<= 2;                              // 寄存器数据
        data1 = ((WriteData >> 8) & 0xFF);
        data2 = ((WriteData >> 0) & 0xFF);

        ////////////////////////////////
        MySPI_CLR_L();
        MySPI_CLR_H();
        delay2(1);         
        MySPI_CLR_L();	
      
        sFLASH_CS_LOW();
        delay2(1);
        ///////////////////////////////					
      
        // 写寄存器数据
        SPI2_ReadWriteByte(WriteDataAddr);
        SPI2_ReadWriteByte(data1);
        SPI2_ReadWriteByte(data2);

        ////////////////////////////
        //delay2(25);     // 70 延时18us，DAC生效时间15us   //镀金杜邦线16达到2MHZ
        delay2(70);       // 70 延时18us，DAC生效时间15us   //跳线128达到600KHZ
        sFLASH_CS_HIGH();
        //delay2(30); 
        ////////////////////////////
    }		
}
















