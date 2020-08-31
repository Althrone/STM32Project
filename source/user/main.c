#include "main.h"



int main(void)
{
    //
    I2C1_Init();
    //
    RGBLED_Init();

    GPIO_SetBits(GPIOE,GPIO_Pin_0);
    GPIO_SetBits(GPIOE,GPIO_Pin_1);
}