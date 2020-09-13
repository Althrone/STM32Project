#include "main.h"



int main(void)
{
    // I2C1_Init();//硬件iic
    RGBLED_Init();
    TIM6_Init();

    IIC_Init();
    USART1_Init();
    // Motor_Init();

    MPU6050_Init();
    SPL06_Init();

    // Motor_SetSpeed();

    // ANO_DT_SendVer(USART1,&ANO_DT_SendVerStruct);

    // USART_SendData(USART1,0xAA);
    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;
    ANO_DT_SendSenser2TypeDef ANO_DT_SendSenser2Struct;

    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;

    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    while (1)
    {
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
        ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_RawDataStruct.MPU6050_RawAccelX;
        ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_RawDataStruct.MPU6050_RawAccelY;
        ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_RawDataStruct.MPU6050_RawAccelZ;
        ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_RawDataStruct.MPU6050_RawGyroX;
        ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_RawDataStruct.MPU6050_RawGyroY;
        ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_RawDataStruct.MPU6050_RawGyroZ;
        ANO_DT_SendSenser(USART1,&ANO_DT_SendSenserStruct);

        SPL06_AllRawDataRead(&SPL06_RawDataStruct);
        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);
        // ANO_DT_SendSenser2Struct.ANO_DT_ALT_BAR=;
        // ANO_DT_SendSenser2Struct.ANO_DT_SEN_TMP=;
    }
}