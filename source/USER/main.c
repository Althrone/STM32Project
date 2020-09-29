#include "main.h"

int main(void)
{
    //测试dsp功能
    // float_t sinx,cosx,add;
    // sinx=arm_sin_f32(0.5f);
    // cosx=arm_cos_f32(0.5f);
    // add=sinx*sinx+cosx*cosx;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

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
    AK8975_FloatDataTypeDef AK8975_FloatDataStruct;

    ATT_AngleDataTypeDef ATT_AngleDataStruct;

    while (1)
    {
        if(CalFlag==1)
        {
            
            CalFlag=0;
        }
        //发送传感器原始数据，机体坐标系
        MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
        MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
        ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_RawDataStruct.MPU6050_RawAccelX;
        ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_RawDataStruct.MPU6050_RawAccelY;
        ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_RawDataStruct.MPU6050_RawAccelZ;
        ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_RawDataStruct.MPU6050_RawGyroX;
        ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_RawDataStruct.MPU6050_RawGyroY;
        ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_RawDataStruct.MPU6050_RawGyroZ;
        ANO_DT_SendSenser(USART1,&ANO_DT_SendSenserStruct);
        //发送遥控器参数到上位机
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        ANO_DT_SendRCData(USART1,&ANO_DT_SendRCDataStruct);

        //姿态解算
        // ATT_RawData(&MPU6050_FloatDataStruct,&AK8975_FloatDataStruct,&ATT_AngleDataStruct);
        // ATT_Calculation(&MPU6050_FloatDataStruct,&ATT_AngleDataStruct,&AHRS_EKFParamStruct);
        AHRS_EKF(&MPU6050_FloatDataStruct,&ATT_AngleDataStruct);
        ANO_DT_SendStatusStruct.ANO_DT_Roll=ATT_AngleDataStruct.ATT_AnglePhi*100;
        ANO_DT_SendStatusStruct.ANO_DT_Pitch=ATT_AngleDataStruct.ATT_AngleTheta*100;
        ANO_DT_SendStatusStruct.ANO_DT_Yaw=ATT_AngleDataStruct.ATT_AnglePsi*100;
        ANO_DT_SendStatus(USART1,&ANO_DT_SendStatusStruct);
    }
}