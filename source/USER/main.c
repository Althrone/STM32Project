#include "main.h"



int main(void)
{
    // ANO_DT_SendVerTypeDef ANO_DT_SendVerStruct;
    // ANO_DT_SendVerStruct.ANO_DT_HardwareType=0x11;
    // ANO_DT_SendVerStruct.ANO_DT_HardwareVer=0x2233;
    // ANO_DT_SendVerStruct.ANO_DT_SoftwareVer=0x4455;
    // ANO_DT_SendVerStruct.ANO_DT_BootloaderVer=0x6677;
    //
    // I2C1_Init();
    IIC_Init();
    //
    RGBLED_Init();
    USART1_Init();
    TIM6_Init();
    
    Motor_Init();

    MPU6050_Init();
    SPL06_Init();

    Motor_SetSpeed();

    // ANO_DT_SendVer(USART1,&ANO_DT_SendVerStruct);

    // USART_SendData(USART1,0xAA);
    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;
    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    while (1)
    {
        SPL06_AllRawDataRead(&SPL06_RawDataStruct);
        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_RawDataStruct.MPU6050_RawAccelX;
        ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_RawDataStruct.MPU6050_RawAccelY;
        ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_RawDataStruct.MPU6050_RawAccelZ;
        ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_RawDataStruct.MPU6050_RawGyroX;
        ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_RawDataStruct.MPU6050_RawGyroY;
        ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_RawDataStruct.MPU6050_RawGyroZ;
        ANO_DT_SendSenser(USART1,&ANO_DT_SendSenserStruct);
        // if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET)
        // {
        //     USART_SendData(USART1,0xAA);
        // }
    }
}