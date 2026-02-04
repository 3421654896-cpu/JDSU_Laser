#include "bsp.h"


void RCC_Configuration(void)
{
  /* 使能AHB1时钟 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | 
                         RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | 
                         RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH, ENABLE);

#if 0
  /* 使能AHB2时钟 */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* 使能APB1时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  /* 使能APB2时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
}

void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = PIN_LED;                             //LED引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                      //输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                 //高速输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推完输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                   //无上下拉(浮空)
  GPIO_Init(PORT_LED, &GPIO_InitStructure);

#if 0
  /* MCO时钟配置 */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);

  RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2);
#endif
}

void BSP_Initializes(void)
{
  RCC_Configuration();                                               //时钟配置
  GPIO_Basic_Configuration();                                        //IO配置
}


/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
