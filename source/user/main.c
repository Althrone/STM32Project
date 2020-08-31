#include "main.h"



int main(void)
{
    //
    I2C1_Init();
    //
    RGBLED_Init();
    // USART1_Init();

    while (1)
    {
            RGBLED_White();

            RGBLED_OFF();
    }
}