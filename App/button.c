/**
  ******************************************************************************
  * @file    button.c
  * @brief   ??????
  ******************************************************************************
  */

#include "button.h"

static uint8_t led_state = 0;  // LED??:0-??,1-??

/**
  * @brief  ????
  */
static void Delay_ms(uint32_t ms)
{
    uint32_t i;
    for(i = 0; i < ms; i++) {
        uint32_t temp = 12000;
        while(temp--);
    }
}

/**
  * @brief  ???LED GPIO???
  */
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ??GPIO?? */
    RCC_AHB1PeriphClockCmd(BUTTON_GPIO_CLK | LED_GPIO_CLK, ENABLE);
    
    /* ???????????,?? */
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // ??,?????
    GPIO_Init(BUTTON_GPIO, &GPIO_InitStructure);
    
    /* ??LED??????? */
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
    
    /* LED??????? */
    LED_OFF();
    led_state = 0;
}

/**
  * @brief  ??????
  * @retval 0-??, 1-??
  */
uint8_t Button_GetState(void)
{
    return GPIO_ReadInputDataBit(BUTTON_GPIO, BUTTON_PIN);
}

/**
  * @brief  ????(????????)
  */
void Button_Scan(void)
{
    static uint8_t button_last_state = BUTTON_RELEASED;
    uint8_t button_current_state;
    
    /* ???????? */
    button_current_state = Button_GetState();
    
    /* ??????(?????) */
    if(button_current_state == BUTTON_PRESSED && button_last_state == BUTTON_RELEASED)
    {
        /* ???? */
        Delay_ms(BUTTON_DEBOUNCE_TIME);
        
        /* ???????? */
        if(Button_GetState() == BUTTON_PRESSED)
        {
            /* ??LED?? */
            led_state = !led_state;
            
            if(led_state) {
                LED_ON();   // PC0?????
            } else {
                LED_OFF();  // PC0?????
            }
        }
    }
    
    /* ?????? */
    button_last_state = button_current_state;
}
