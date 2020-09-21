#include "main.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

    // I2C1_Init();//硬件iic
    RGBLED_Init();
    TIM6_Init();

    IIC_Init();
    USART1_Init();
    
    PPM_Init();

    MPU6050_Init();
    SPL06_Init();
    AK8975_Init();

    Motor_Init();
    Steer_Init();

    //校准代码区
    //获取遥控器数值


    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;//发送到上位机的传感器数据结构体
    ANO_DT_SendRCDataTypeDef ANO_DT_SendRCDataStruct;//发送到上位机的遥控数据
    ANO_DT_SendStatusTypeDef ANO_DT_SendStatusStruct;//无人机当前姿态，这里我只是随便塞两个数进去看看

    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;

    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    AK8975_RawDataTypeDef AK8975_RawDataStruct;
    AK8975_FloatDataTypeDef AK8975_FloatDataStruct;

    ATT_AngleDataTypeDef ATT_AngleDataStruct;
    ATT_QuatDataTypeDef ATT_QuatDataStruct;

    Motor_PWMTypeDef Motor_PWMStruct;

    MPU6050_CalParamTypeDef MPU6050_CalParamStruct;
    MPU6050_GyroCal(&MPU6050_CalParamStruct);

    // float_t a=0.2333f;
    float_t b=0;
    // AT24C02_PageWrite(0x00,(uint8_t*)&a);
    // AT24C02_SequentialRead(0x00,4,(uint8_t*)&b);
    // AT24C02_SequentialRead(0x04,4,(uint8_t*)&b);
    AT24C02_SequentialRead(0x00,4,(uint8_t*)&b);
    AT24C02_SequentialRead(0x08,4,(uint8_t*)&b);
    AT24C02_SequentialRead(0x0F,4,(uint8_t*)&b);

    while (1)
    {
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        AK8975_AllRawDataRead(&AK8975_RawDataStruct);
        SPL06_AllRawDataRead(&SPL06_RawDataStruct);

        MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
        AK8975_RawData2FloatData(&AK8975_RawDataStruct,&AK8975_FloatDataStruct);
        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);

        PPM_GetRCData(&ANO_DT_SendRCDataStruct);

        Steer_Test(&ANO_DT_SendRCDataStruct);

        Motor_PWMStruct.PWM1=ANO_DT_SendRCDataStruct.ANO_DT_RCThrottle;
        Motor_PWMStruct.PWM2=ANO_DT_SendRCDataStruct.ANO_DT_RCThrottle;
        Motor_PWMStruct.PWM3=ANO_DT_SendRCDataStruct.ANO_DT_RCThrottle;
        Motor_PWMStruct.PWM4=ANO_DT_SendRCDataStruct.ANO_DT_RCThrottle;
        Motor_PWM(&Motor_PWMStruct);

        ATT_RawData(&MPU6050_FloatDataStruct,&AK8975_FloatDataStruct,&ATT_AngleDataStruct);
        ANO_DT_SendStatusStruct.ANO_DT_Pitch=(int16_t)(ATT_AngleDataStruct.ATT_AngleTheta*100);
        ANO_DT_SendStatusStruct.ANO_DT_Roll=(int16_t)(ATT_AngleDataStruct.ATT_AnglePhi*100);
        ANO_DT_SendStatusStruct.ANO_DT_Yaw=(int16_t)(ATT_AngleDataStruct.ATT_AnglePsi*100);

        // ATT_Angle2Quat(&ATT_AngleDataStruct,&ATT_QuatDataStruct);
        // ATT_Quat2Angle(&ATT_QuatDataStruct,&ATT_AngleDataStruct);


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

        
    }
}