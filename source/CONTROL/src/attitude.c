#include "attitude.h"

arm_matrix_instance_f32 MixerMatrix;//混控矩阵

AHRS_EKFParamTypeDef AHRS_EKFParamStruct;

void ATT_RawData(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                 AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
                 ATT_AngleDataTypeDef* ATT_AngleDataStruct)
{
    float_t xita,phi,psi;
    float_t nomo;//用于单位化
    float_t a_x,a_y,a_z;
    float_t Bx,By;

    nomo=Fast_InvSqrt(MPU6050_FloatDataStruct->MPU6050_FloatAccelX*MPU6050_FloatDataStruct->MPU6050_FloatAccelX+
                      MPU6050_FloatDataStruct->MPU6050_FloatAccelY*MPU6050_FloatDataStruct->MPU6050_FloatAccelY+
                      MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*MPU6050_FloatDataStruct->MPU6050_FloatAccelZ);

    a_x=MPU6050_FloatDataStruct->MPU6050_FloatAccelX*nomo;
    a_y=MPU6050_FloatDataStruct->MPU6050_FloatAccelY*nomo;
    a_z=MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*nomo;

    xita=asin(a_x*nomo);
    phi=atan2(-a_y,-a_z);
    
    Bx=AK8975_FloatDataStruct->AK8975_FloatMagX*cos(xita)+
       AK8975_FloatDataStruct->AK8975_FloatMagY*sin(xita)*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(xita)*cos(phi);
    By=AK8975_FloatDataStruct->AK8975_FloatMagY*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(phi);
    
    xita*=53.7;
    phi*=53.7;

    // if((By==0)&&(Bx>0))
    // psi=0;
    // if(By<0)
    // psi=90+atan(Bx/By)*53.7;
    // if((By==0)&&(Bx<0))
    // psi=180;
    // if(By>0)
    // psi=270+atan(Bx/By)*53.7;


    ATT_AngleDataStruct->ATT_AngleTheta=xita;
    ATT_AngleDataStruct->ATT_AnglePhi=phi;
    // ATT_AngleDataStruct->ATT_AnglePsi=psi;
    ATT_AngleDataStruct->ATT_AnglePsi=0;
}

/**
 * @brief   欧拉角转四元数
 * @param   ATT_AngleDataStruct: 欧拉角
 * @param   ATT_QuatDataStruct: 四元数
 **/
void ATT_Angle2Quat(ATT_AngleDataTypeDef* ATT_AngleDataStruct,ATT_QuatDataTypeDef* ATT_QuatDataStruct)
{
    float_t HalfPhi,HalfTheta,HalfPsi;
    float_t SinHalfPhi,CosHalfPhi,SinHalfTheta,CosHalfTheta,SinHalfPsi,CosHalfPsi;

    HalfPhi=ATT_AngleDataStruct->ATT_AnglePhi/2*0.01745;
    HalfTheta=ATT_AngleDataStruct->ATT_AngleTheta/2*0.01745;
    HalfPsi=ATT_AngleDataStruct->ATT_AnglePsi/2*0.01745;

    //输入和输出都是弧度制
    SinHalfPhi=sin(HalfPhi);
    CosHalfPhi=cos(HalfPhi);
    SinHalfTheta=sin(HalfTheta);
    CosHalfTheta=cos(HalfTheta);
    SinHalfPsi=sin(HalfPsi);
    CosHalfPsi=cos(HalfPsi);

    ATT_QuatDataStruct->ATT_Quat0=CosHalfPhi*CosHalfTheta*CosHalfPsi+SinHalfPhi*SinHalfTheta*SinHalfPsi;
    ATT_QuatDataStruct->ATT_Quat1=SinHalfPhi*CosHalfTheta*CosHalfPsi-CosHalfPhi*SinHalfTheta*SinHalfPsi;
    ATT_QuatDataStruct->ATT_Quat2=CosHalfPhi*SinHalfTheta*CosHalfPsi+SinHalfPhi*CosHalfTheta*SinHalfPsi;
    ATT_QuatDataStruct->ATT_Quat3=CosHalfPhi*CosHalfTheta*SinHalfPsi-SinHalfPhi*SinHalfTheta*CosHalfPsi;
}

/**
 * @brief   四元数转欧拉角
 * @param   ATT_QuatDataStruct: 四元数
 * @param   ATT_AngleDataStruct: 欧拉角
 **/
void ATT_Quat2Angle(ATT_QuatDataTypeDef* ATT_QuatDataStruct,ATT_AngleDataTypeDef* ATT_AngleDataStruct)
{
    // float_t c11,c12,c13,c23,c33;
    float_t q0,q1,q2,q3;

    q0=ATT_QuatDataStruct->ATT_Quat0;
    q1=ATT_QuatDataStruct->ATT_Quat1;
    q2=ATT_QuatDataStruct->ATT_Quat2;
    q3=ATT_QuatDataStruct->ATT_Quat3;

    // c11=q0*q0+q1*q1-q2*q2-q3*q3;
    // c12=2*(q1*q2-q0*q3);
    // c13=2*(q1*q3+q0*q2);
    // c23=2*(q2*q3-q0*q1);
    // c33=q0*q0-q1*q1-q2*q2+q3*q3;

    ATT_AngleDataStruct->ATT_AnglePhi=atan2(2*(q0*q1+q2*q3),1-2*(q1*q1+q2*q2))*53.7;
    ATT_AngleDataStruct->ATT_AngleTheta=asin(2*(q0*q2-q1*q3))*53.7;
    ATT_AngleDataStruct->ATT_AnglePsi=atan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3))*53.7;
}

/**
 * @brief   数据校正
 **/
void ATT_SensorCal(void)
{
    //读AT24C02的6050陀螺仪标志位
    uint32_t flag;
    AT24C02_SequentialRead(0x00,4,(uint8_t*)&flag);
    if(flag!=0xAAAAAA00)//传感器未校准，或者校准数据出错
    {
        //死循环红灯快闪，
    }
}

void Control(void)
{
    //
}

/**
 * @brief   选择对应的混控矩阵
 **/
void ATT_MixerMatrixInit(void)
{
    /*
    X模式，RPTY是摇杆通道顺序
    ┌         ┐┌   ┐ ┌    ┐
    │ - - + + ││ R │ │ M1 │
    │ + + + + ││ P │=│ M2 │
    │ + - + - ││ T │ │ M3 │
    │ - + + - ││ Y │ │ M4 │
    └         ┘└   ┘ └    ┘
    */
    float32_t param[16]=
    {
       -0.70710678f,   -0.70710678f,    1.0f,   1.0f,
        0.70710678f,    0.70710678f,    1.0f,   1.0f,
        0.70710678f,   -0.70710678f,    1.0f,  -1.0f,
       -0.70710678f,    0.70710678f,    1.0f,  -1.0f
    };

    arm_mat_init_f32(&MixerMatrix,4,4,param);
}

/**
 * @brief   飞机初始姿态四元数初始化
 **/
void ATT_Init(void)
{
    //初始化飞机姿态
    AHRS_InitX(&AHRS_EKFParamStruct.X);
    //初始化状态矩阵协方差矩阵
    AHRS_InitP(&AHRS_EKFParamStruct.P);
    //初始化过程噪声协方差，灵魂调参
    AHRS_InitQ(&AHRS_EKFParamStruct.Q);
    //初始化观测噪声方差矩阵，这个矩阵一直都是不变的，非常重要
    AHRS_InitR(&AHRS_EKFParamStruct.R);
}

/**
 * @brief   姿态解算
 * @param   MPU6050_FloatDataStruct: 6050数据
 * @param   ATT_AngleDataStruct: 解算出来的姿态数据
 **/
void ATT_Calculation(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                     ATT_AngleDataTypeDef* ATT_AngleDataStruct)
{
    ATT_QuatDataTypeDef ATT_QuatDataStruct;
    AHRS_GetA(MPU6050_FloatDataStruct,&AHRS_EKFParamStruct.A);
    AHRS_GetC(&AHRS_EKFParamStruct.X,&AHRS_EKFParamStruct.C);
    AHRS_GetZ(MPU6050_FloatDataStruct,&AHRS_EKFParamStruct.Z);
    AHRS_EKF(&AHRS_EKFParamStruct);
    ATT_QuatDataStruct.ATT_Quat0=AHRS_EKFParamStruct.X.pData[0];
    ATT_QuatDataStruct.ATT_Quat1=AHRS_EKFParamStruct.X.pData[1];
    ATT_QuatDataStruct.ATT_Quat2=AHRS_EKFParamStruct.X.pData[2];
    ATT_QuatDataStruct.ATT_Quat3=AHRS_EKFParamStruct.X.pData[3];
    ATT_Quat2Angle(&ATT_QuatDataStruct,ATT_AngleDataStruct);
}