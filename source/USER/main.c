#include "main.h"


int main(void)
{
    // I2C1_Init();//硬件iic
    RGBLED_Init();
    TIM6_Init();

    IIC_Init();
    USART1_Init();
    // Motor_Init();
    PPM_Init();

    MPU6050_Init();
    SPL06_Init();
    AK8975_Init();

    // Motor_SetSpeed();

    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;

    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;

    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    AK8975_RawDataTypeDef AK8975_RawDataStruct;
    AK8975_FloatDataTypeDef AK8975_FloatDataStruct;

    while (1)
    {
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        AK8975_AllRawDataRead(&AK8975_RawDataStruct);
        ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_RawDataStruct.MPU6050_RawAccelX;
        ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_RawDataStruct.MPU6050_RawAccelY;
        ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_RawDataStruct.MPU6050_RawAccelZ;
        ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_RawDataStruct.MPU6050_RawGyroX;
        ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_RawDataStruct.MPU6050_RawGyroY;
        ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_RawDataStruct.MPU6050_RawGyroZ;
        ANO_DT_SendSenserStruct.ANO_DT_MagX=AK8975_RawDataStruct.AK8975_RawMagX;
        ANO_DT_SendSenserStruct.ANO_DT_MagY=AK8975_RawDataStruct.AK8975_RawMagY;
        ANO_DT_SendSenserStruct.ANO_DT_MagZ=AK8975_RawDataStruct.AK8975_RawMagZ;
        ANO_DT_SendSenser(USART1,&ANO_DT_SendSenserStruct);

        SPL06_AllRawDataRead(&SPL06_RawDataStruct);

        MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);
        AK8975_RawData2FloatData(&AK8975_RawDataStruct,&AK8975_FloatDataStruct);
    }
}