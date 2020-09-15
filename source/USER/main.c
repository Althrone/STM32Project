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

    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;//发送到上位机的传感器数据结构体
    ANO_DT_SendRCDataTypeDef ANO_DT_SendRCDataStruct;//发送到上位机的遥控数据
    ANO_DT_SendStatusTypeDef ANO_DT_SendStatusStruct;//无人机当前姿态，这里我只是随便塞两个数进去看看

    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;

    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    AK8975_RawDataTypeDef AK8975_RawDataStruct;
    AK8975_FloatDataTypeDef AK8975_FloatDataStruct;

    ANO_DT_SendRCDataStruct.ANO_DT_RCThrottle=0;
    ANO_DT_SendRCDataStruct.ANO_DT_RCYaw=1;
    ANO_DT_SendRCDataStruct.ANO_DT_RCRoll=2;
    ANO_DT_SendRCDataStruct.ANO_DT_RCPitch=3;
    ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1=4;

    while (1)
    {
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        AK8975_AllRawDataRead(&AK8975_RawDataStruct);
        SPL06_AllRawDataRead(&SPL06_RawDataStruct);

        PPM_GetRCData(&ANO_DT_SendRCDataStruct);

        ATT_RawData(&MPU6050_FloatDataStruct,&ANO_DT_SendStatusStruct);

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

        ANO_DT_SendRCData(USART1,&ANO_DT_SendRCDataStruct);
        ANO_DT_SendStatus(USART1,&ANO_DT_SendStatusStruct);

        MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
        AK8975_RawData2FloatData(&AK8975_RawDataStruct,&AK8975_FloatDataStruct);
        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);
    }
}