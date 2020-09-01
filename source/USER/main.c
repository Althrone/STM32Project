#include "main.h"



int main(void)
{
    //
    // I2C1_Init();
    //
    RGBLED_Init();
    // USART1_Init();
    TIM6_Init();
    
    Motor_Init();

    Motor_SetSpeed();

    while (1)
    {

    }
}