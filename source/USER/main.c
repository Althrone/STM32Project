#include "main.h"

int main(void)
{
    uint8_t CLEAR_CFG[] ={0xB5,0x62,0x06,0x09,0x0D,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,};//88 B7
    uint8_t SAVE_CFG[]  ={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x17,};//31 BF
    uint8_t LOAD_CFG[]  ={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x17,};

    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    for(uint8_t i=2;i<sizeof(SAVE_CFG);i++)
    {
        CK_A = CK_A + CLEAR_CFG[i];
        CK_B = CK_B + CK_A;
        //CK_A和CK_B等于串口数据的最后两项的情况下就正确
    }


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

    RGBLED_Init();
    TIM6_Init();

    IIC_Init();
    // I2C1_Init();

    USART1_Init();//上位机串口

    PPM_Init();

    MPU6050_Init();
    SPL06_Init();
    AK8975_Init();
    GPS_Init();

    Motor_Init();
    Steer_Init();

    PID_ParamInit();
    ALT_3OrderCompParamInit();

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
    ATT_QuatDataTypeDef ATT_QuatDataStruct;
    
    float_t testx,testy,testz,bx,bz;
    float_t x0,y0,z0,rx,ry,rz;
    float_t mx,my,mz,rmx,rmy,rmz;

    AT24C02_SequentialRead(0x04,4,(uint8_t*)&testx);
    AT24C02_SequentialRead(0x08,4,(uint8_t*)&testy);
    AT24C02_SequentialRead(0x0C,4,(uint8_t*)&testz);
    AT24C02_SequentialRead(0x40,4,(uint8_t*)&bx);
    AT24C02_SequentialRead(0x44,4,(uint8_t*)&bz);

    AT24C02_SequentialRead(0x10,4,(uint8_t*)&rx);
    AT24C02_SequentialRead(0x18,4,(uint8_t*)&ry);
    AT24C02_SequentialRead(0x20,4,(uint8_t*)&rz);
    AT24C02_SequentialRead(0x14,4,(uint8_t*)&x0);
    AT24C02_SequentialRead(0x1C,4,(uint8_t*)&y0);
    AT24C02_SequentialRead(0x24,4,(uint8_t*)&z0);

    AT24C02_SequentialRead(0x28,4,(uint8_t*)&rmx);
    AT24C02_SequentialRead(0x30,4,(uint8_t*)&rmy);
    AT24C02_SequentialRead(0x38,4,(uint8_t*)&rmz);
    AT24C02_SequentialRead(0x2C,4,(uint8_t*)&mx);
    AT24C02_SequentialRead(0x34,4,(uint8_t*)&my);
    AT24C02_SequentialRead(0x3C,4,(uint8_t*)&mz);

    // MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;
    // AK8975_FloatDataTypeDef AK8975_FloatDataStruct;

    extern float_t accZ;

    float_t vol;

    extern Filter_CompInfoTypeDef ALT_AccZCompInfoStruct;

    // uint8_t databuf[8]={0,1,2,3,4,5,6,7};
    // uint8_t tmp=0;
    // uint8_t a;
    // a=databuf[++tmp];

    while (1)
    {
        if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)
            USART_ClearFlag(UART4,USART_FLAG_RXNE);
        if(CalFlag==1)
        {
            MPU6050_RawData2CalData(&MPU6050_RawDataStruct,&MPU6050_CalDataStruct);
            AK8975_RawData2CalData(&AK8975_RawDataStruct,&AK8975_CalDataStruct);
            SPL06_RawData2FloatData(&SPL06_RawDataStruct,&SPL06_FloatDataStruct);

            // MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
            // AK8975_RawData2FloatData(&AK8975_RawDataStruct,&AK8975_FloatDataStruct);

            AHRS_MahonyUpdate(&MPU6050_CalDataStruct,
                              &AK8975_CalDataStruct,
                              &ATT_QuatDataStruct);
            
            ATT_Quat2Angle(&ATT_QuatDataStruct,&ATT_AngleDataStruct);

            ALT_3OrderCompUpdate(&MPU6050_CalDataStruct,
                                 &SPL06_FloatDataStruct,
                                 &ATT_QuatDataStruct);
            
            vol+=accZ*0.01;//加速度计积分

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

            CalFlag=0;
        }

        /*********各原始数据**********/

        // ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_RawDataStruct.MPU6050_RawAccelX;
        // ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_RawDataStruct.MPU6050_RawAccelY;
        // ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_RawDataStruct.MPU6050_RawAccelZ;
        // ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_RawDataStruct.MPU6050_RawGyroX;
        // ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_RawDataStruct.MPU6050_RawGyroY;
        // ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_RawDataStruct.MPU6050_RawGyroZ;

        // ANO_DT_SendSenserStruct.ANO_DT_MagX=AK8975_RawDataStruct.AK8975_RawMagX;
        // ANO_DT_SendSenserStruct.ANO_DT_MagY=AK8975_RawDataStruct.AK8975_RawMagY;
        // ANO_DT_SendSenserStruct.ANO_DT_MagZ=AK8975_RawDataStruct.AK8975_RawMagZ;

        /*********加速度计原始数据与矫正数据对比**********/

        ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_CalDataStruct.MPU6050_CalAccelX*1000;
        ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_CalDataStruct.MPU6050_CalAccelY*1000;
        ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_CalDataStruct.MPU6050_CalAccelZ*1000;

        // ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_FloatDataStruct.MPU6050_FloatGyroX*1000;
        // ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_FloatDataStruct.MPU6050_FloatGyroY*1000;
        // ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_FloatDataStruct.MPU6050_FloatGyroZ*1000;

        /*********陀螺仪原始数据与矫正数据对比**********/

        // ANO_DT_SendSenserStruct.ANO_DT_AccX=MPU6050_FloatDataStruct.MPU6050_FloatGyroX;
        // ANO_DT_SendSenserStruct.ANO_DT_AccY=MPU6050_FloatDataStruct.MPU6050_FloatGyroY;
        // ANO_DT_SendSenserStruct.ANO_DT_AccZ=MPU6050_FloatDataStruct.MPU6050_FloatGyroZ;

        ANO_DT_SendSenserStruct.ANO_DT_GyroX=MPU6050_CalDataStruct.MPU6050_CalGyroX;
        ANO_DT_SendSenserStruct.ANO_DT_GyroY=MPU6050_CalDataStruct.MPU6050_CalGyroY;
        ANO_DT_SendSenserStruct.ANO_DT_GyroZ=MPU6050_CalDataStruct.MPU6050_CalGyroZ;

        /*********磁力计原始数据与矫正数据对比**********/

        // ANO_DT_SendSenserStruct.ANO_DT_AccX=AK8975_FloatDataStruct.AK8975_FloatMagX;
        // ANO_DT_SendSenserStruct.ANO_DT_AccY=AK8975_FloatDataStruct.AK8975_FloatMagY;
        // ANO_DT_SendSenserStruct.ANO_DT_AccZ=AK8975_FloatDataStruct.AK8975_FloatMagZ;

        ANO_DT_SendSenserStruct.ANO_DT_MagX=AK8975_CalDataStruct.AK8975_CalMagX;
        ANO_DT_SendSenserStruct.ANO_DT_MagY=AK8975_CalDataStruct.AK8975_CalMagY;
        ANO_DT_SendSenserStruct.ANO_DT_MagZ=AK8975_CalDataStruct.AK8975_CalMagZ;

        ANO_DT_SendSenser(USART1,&ANO_DT_SendSenserStruct);

        ANO_DT_SendSenser2Struct.ANO_DT_ALT_BAR=(SPL06_FloatDataStruct.SPL06_FloatAlt+38)*100;//海拔高度，有滤波
        // ANO_DT_SendSenser2Struct.ANO_DT_ALT_ADD=SPL06_FloatDataStruct.SPL06_FloatSpeed*100;//气压计微分速度
        ANO_DT_SendSenser2Struct.ANO_DT_SEN_TMP=SPL06_FloatDataStruct.SPL06_FloatTemp*10;

        // ANO_DT_SendSenser2Struct.ANO_DT_ALT_BAR=vol*100;//加速度计积分的速度
        // ANO_DT_SendSenser2Struct.ANO_DT_ALT_ADD=SPL06_FloatDataStruct.SPL06_FloatSpeed*100;//气压计微分的速度

        // ANO_DT_SendSenser2Struct.ANO_DT_ALT_BAR=SPL06_FloatDataStruct.SPL06_FloatSpeed*100;//气压计微分的速度
        ANO_DT_SendSenser2Struct.ANO_DT_ALT_ADD=ALT_AccZCompInfoStruct.Output*100;//互补滤波的速度

        ANO_DT_SendSenser2(USART1,&ANO_DT_SendSenser2Struct);

        // 发送遥控器参数到上位机
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        ANO_DT_SendRCData(USART1,&ANO_DT_SendRCDataStruct);

        ANO_DT_SendStatusStruct.ANO_DT_Roll=ATT_AngleDataStruct.ATT_AnglePhi*100;
        ANO_DT_SendStatusStruct.ANO_DT_Pitch=ATT_AngleDataStruct.ATT_AngleTheta*100;
        ANO_DT_SendStatusStruct.ANO_DT_Yaw=ATT_AngleDataStruct.ATT_AnglePsi*100;
        ANO_DT_SendStatus(USART1,&ANO_DT_SendStatusStruct);

        
    }
}