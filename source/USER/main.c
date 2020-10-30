#include "main.h"

int main(void)
{

    //链表测试：通过
    // List_NodeTypeDef* List_HeadPointer=NULL;
    // if(List_CreatHead(&List_HeadPointer))
    // {
    //     if(List_AddNode(List_HeadPointer,11))
    //     {
    //         List_AddNode(List_HeadPointer,33);
    //         uint8_t tmp;
    //         tmp=List_Length(List_HeadPointer);
    //         tmp=List_DeleteList(&List_HeadPointer);
    //     }
    // }

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

    RGBLED_Init();
    TIM6_Init();

    IIC_Init();
    USART1_Init();//上位机串口

    PPM_Init();

    MPU6050_Init();
    SPL06_Init();
    AK8975_Init();

    Motor_Init();
    Steer_Init();

    PID_ParamInit();

    //校准代码区
    CAL_Senser();
    //获取遥控器数值
    ANO_DT_SendSenserTypeDef ANO_DT_SendSenserStruct;//发送到上位机的传感器数据结构体
    ANO_DT_SendRCDataTypeDef ANO_DT_SendRCDataStruct;//发送到上位机的遥控数据
    ANO_DT_SendStatusTypeDef ANO_DT_SendStatusStruct;//无人机当前姿态，这里我只是随便塞两个数进去看看
    ANO_DT_SendSenser2TypeDef ANO_DT_SendSenser2Struct;//发送气压计，测高，温度到上位机

    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_CalDataTypeDef MPU6050_CalDataStruct;

    AK8975_RawDataTypeDef AK8975_RawDataStruct;
    AK8975_CalDataTypeDef AK8975_CalDataStruct;

    SPL06_RawDataTypeDef SPL06_RawDataStruct;
    SPL06_FloatDataTypeDef SPL06_FloatDataStruct;

    ATT_AngleDataTypeDef ATT_AngleDataStruct;
    
    // float_t testx,testy,testz,bx,bz;
    // float_t x0,y0,z0,rx,ry,rz;
    // float_t mx,my,mz,rmx,rmy,rmz;

    // AT24C02_SequentialRead(0x04,4,(uint8_t*)&testx);
    // AT24C02_SequentialRead(0x08,4,(uint8_t*)&testy);
    // AT24C02_SequentialRead(0x0C,4,(uint8_t*)&testz);
    // AT24C02_SequentialRead(0x40,4,(uint8_t*)&bx);
    // AT24C02_SequentialRead(0x44,4,(uint8_t*)&bz);

    // AT24C02_SequentialRead(0x10,4,(uint8_t*)&rx);
    // AT24C02_SequentialRead(0x18,4,(uint8_t*)&ry);
    // AT24C02_SequentialRead(0x20,4,(uint8_t*)&rz);
    // AT24C02_SequentialRead(0x14,4,(uint8_t*)&x0);
    // AT24C02_SequentialRead(0x1C,4,(uint8_t*)&y0);
    // AT24C02_SequentialRead(0x24,4,(uint8_t*)&z0);

    // AT24C02_SequentialRead(0x28,4,(uint8_t*)&rmx);
    // AT24C02_SequentialRead(0x30,4,(uint8_t*)&rmy);
    // AT24C02_SequentialRead(0x38,4,(uint8_t*)&rmz);
    // AT24C02_SequentialRead(0x2C,4,(uint8_t*)&mx);
    // AT24C02_SequentialRead(0x34,4,(uint8_t*)&my);
    // AT24C02_SequentialRead(0x3C,4,(uint8_t*)&mz);

    float_t alt=0;

    while (1)
    {
        if(CalFlag==1)
        {
            MPU6050_RawData2CalData(&MPU6050_RawDataStruct,&MPU6050_CalDataStruct);
            AK8975_RawData2CalData(&AK8975_RawDataStruct,&AK8975_CalDataStruct);

            //姿态解算
            // AHRS_EKF(&MPU6050_FloatDataStruct,&ATT_AngleDataStruct);
            // ATT_RawData(&MPU6050_FloatDataStruct,
            //             &AK8975_FloatDataStruct,
            //             &ATT_AngleDataStruct);

            AHRS_MahonyUpdate(&MPU6050_CalDataStruct,
                              &AK8975_CalDataStruct,
                              &ATT_AngleDataStruct);
            // 暴力矫正
            // ATT_AngleDataStruct.ATT_AnglePhi-=4.3;
            // ATT_AngleDataStruct.ATT_AngleTheta+=3.8;
            CalFlag=0;
        }
        // if(LEDFlag==1)
        // {
        //     RGBLED_StateSet(RGBLED_White,RGBLED_1sMode);
        // }

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
        // 发送遥控器参数到上位机
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        ANO_DT_SendRCData(USART1,&ANO_DT_SendRCDataStruct);

        ANO_DT_SendStatusStruct.ANO_DT_Roll=ATT_AngleDataStruct.ATT_AnglePhi*100;
        ANO_DT_SendStatusStruct.ANO_DT_Pitch=ATT_AngleDataStruct.ATT_AngleTheta*100;
        ANO_DT_SendStatusStruct.ANO_DT_Yaw=ATT_AngleDataStruct.ATT_AnglePsi*100;
        ANO_DT_SendStatus(USART1,&ANO_DT_SendStatusStruct);

        SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);
        SPL06_RawData2Altitude(&SPL06_RawDataStruct,&alt);
        ANO_DT_SendSenser2Struct.ANO_DT_ALT_BAR=alt*100;
        ANO_DT_SendSenser2Struct.ANO_DT_SEN_TMP=10.0f*SPL06_FloatDataStruct.SPL06_FloatTemp;
        ANO_DT_SendSenser2(USART1,&ANO_DT_SendSenser2Struct);

        if(ANO_DT_SendRCDataStruct.ANO_DT_RCAUX4<1200)
        {
            Motor_SetSpeed();
            RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
        }
        else
        {
            FLY_DroneCtrl(&ANO_DT_SendRCDataStruct,&ATT_AngleDataStruct,&MPU6050_CalDataStruct);
            RGBLED_StateSet(RGBLED_Green,RGBLED_1sMode);
        }
    }
}