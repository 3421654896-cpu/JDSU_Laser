/**
  *********************************  STM32F2xx  ********************************
  * @文件名     ： bsp.h
  * @作者       ： strongerHuang
  * @库版本     ： V1.1.2
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年11月01日
  * @摘要       ： BSP板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f2xx.h"


/* 宏定义 --------------------------------------------------------------------*/
#define PORT_LED                  GPIOD                    //端口
#define PIN_LED                   GPIO_Pin_2               //引脚

/* LED亮、灭、变化 */
#define LED_ON                    GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_OFF                   GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_TOGGLE                GPIO_ToggleBits(PORT_LED, PIN_LED)

/* 函数申明 ------------------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Basic_Configuration(void);
void BSP_Initializes(void);


#endif /* _BSP_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
