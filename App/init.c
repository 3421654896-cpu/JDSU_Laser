#include "main.h"

#define DEM_CR       (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL    (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t *)0xE0001004)

uint8_t tx_buffer[512] = {0};

/**
  * @brief  Sets System clock frequency to 180MHz and configure HCLK, PCLK2 
  *         and PCLK1 prescalers. 
  * @param  None
  * @retval None
  */
void SetSysClockTo180(void)
{
	ErrorStatus HSEStartUpStatus;
	
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  
  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(ENABLE);
    
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_5);
    
    /* PLL configuration */
		// HSE = 25MHz,25/5*72/2=180
    RCC_PLLConfig(RCC_PLLSource_HSE, 25, 360, 2, 7);
		
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
    User can add here some code to deal with this error */    
    
    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

void Clock_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef RCC_ClockFreq;
	
	SetSysClockTo180();
	
	/* This function fills the RCC_ClockFreq structure with the current
  frequencies of different on chip clocks (for debug purpose) **************/
  RCC_GetClocksFreq(&RCC_ClockFreq);
	
  /* Enable Clock Security System(CSS): this will generate an NMI exception
  when HSE clock fails *****************************************************/
  RCC_ClockSecuritySystemCmd(ENABLE);	
	
  /* Enable and configure RCC global IRQ channel, will be used to manage HSE ready 
  and PLL ready interrupts. 
  These interrupts are enabled in stm32f4xx_it.c file **********************/
//  NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
	RCC_PCLK1Config(RCC_HCLK_Div4);
	
	RCC_PCLK2Config(RCC_HCLK_Div2);


  /* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
  
//  /* Enable the GPIOA peripheral */ 
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  
//  /* Configure MCO1 pin(PA8) in alternate function */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//  /* HSE clock selected to output on MCO1 pin(PA8)*/
//  RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
void USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//设置GPIOA结构体变量
	USART_InitTypeDef USART_InitStructure;//设置串口结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;//设置中断优先级NVIC结构体变量
	DMA_InitTypeDef DMA_InitStructure;//USART_DMA
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能串口1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIOA使能
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//PA9引脚映射为串口1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//PA10引脚映射为串口1

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//初始化引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//设置模式为复用功能
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIOA初始化
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//初始化引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//设置模式为复用功能
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIOA初始化
	
	USART_InitStructure.USART_BaudRate=2000000;//设置波特率
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//设置串口模式使能Tx/Rx
	USART_InitStructure.USART_Parity=USART_Parity_No;//设置奇偶校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//设置停止位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件流控制
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//设置8位字长(设置9位字长通常最后一位是奇偶校验位)
	USART_Init(USART1,&USART_InitStructure);//串口初始化
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1,ENABLE);//使能串口1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启相关中断   USART_IT_RXNE使能非空
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IQR通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//子优先级1
	NVIC_Init(&NVIC_InitStructure);//初始化NVIC中断优先级
	
	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 8;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(USART_DMA_STREAM, &DMA_InitStructure);
  DMA_Cmd(USART_DMA_STREAM, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(USART_DMA_STREAM, DMA_IT_TC, ENABLE);
}

void BUTTON_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void LED_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	LED_MANUAL_LOW();
	LED_TABLE_LOW();
}

void USART_DMA_Send(uint8_t *data, uint16_t length){
//	while(!dma_transfer_complete && DMA_GetCmdStatus(DMA2_Stream7));
//	
//	dma_transfer_complete = 0;
	
	while(DMA_GetCmdStatus(DMA2_Stream7));
	
	memcpy(tx_buffer, data, length);
	
	DMA_SetCurrDataCounter(DMA2_Stream7, length);
	
	DMA_ClearFlag(DMA2_Stream7, 
								DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 |
								DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
	
	DMA_Cmd(DMA2_Stream7, ENABLE);
}

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






