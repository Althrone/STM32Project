#include "main.h"



int main(void)
{
    ANO_DT_SendVerTypeDef ANO_DT_SendVerStruct;
    ANO_DT_SendVerStruct.ANO_DT_HardwareType=0x11;
    ANO_DT_SendVerStruct.ANO_DT_HardwareVer=0x2233;
    ANO_DT_SendVerStruct.ANO_DT_SoftwareVer=0x4455;
    ANO_DT_SendVerStruct.ANO_DT_BootloaderVer=0x6677;
    //
    // I2C1_Init();
    //
    RGBLED_Init();
    USART1_Init();
    TIM6_Init();
    
    Motor_Init();

    MPU6050_Init();

    Motor_SetSpeed();

    ANO_DT_SendVer(USART1,&ANO_DT_SendVerStruct);

    // USART_SendData(USART1,0xAA);

    while (1)
    {
        
        // if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET)
        // {
        //     USART_SendData(USART1,0xAA);
        // }
    }
}