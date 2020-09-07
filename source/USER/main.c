#include "main.h"



int main(void)
{
    ANO_DT_SendVerTypeDef ANO_DT_SendVerStruct;
    ANO_DT_SendVerStruct.ANO_DT_BootloaderVer=1;
    ANO_DT_SendVerStruct.ANO_DT_HardwareType=0xCC;
    //
    // I2C1_Init();
    //
    RGBLED_Init();
    USART1_Init();
    TIM6_Init();
    
    Motor_Init();

    Motor_SetSpeed();

    // ANO_DT_SendVer(USART1,&ANO_DT_SendVerStruct);

    USART_SendData(USART1,0xAA);

    while (1)
    {
        // if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET)
        // {
        //     USART_SendData(USART1,0xAA);
        // }
    }
}