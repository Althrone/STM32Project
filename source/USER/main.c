#include "main.h"



int main(void)
{
    //
    // I2C1_Init();
    //
    RGBLED_Init();
    USART1_Init();
    TIM6_Init();
    
    Motor_Init();

    Motor_SetSpeed();


    while (1)
    {
        // if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET)
        // {
        //     USART_SendData(USART1,0xAA);
        // }
    }
}