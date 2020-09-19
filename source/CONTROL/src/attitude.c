#include "attitude.h"

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
    phi=atan2(a_y,a_z);
    
    Bx=AK8975_FloatDataStruct->AK8975_FloatMagX*cos(xita)+
       AK8975_FloatDataStruct->AK8975_FloatMagY*sin(xita)*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(xita)*cos(phi);
    By=AK8975_FloatDataStruct->AK8975_FloatMagY*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(phi);
    
    xita*=53.7;
    phi*=53.7;

    if((By==0)&&(Bx>0))
    psi=0;
    if(By<0)
    psi=90+atan(Bx/By)*53.7;
    if((By==0)&&(Bx<0))
    psi=180;
    if(By>0)
    psi=270+atan(Bx/By)*53.7;


    ATT_AngleDataStruct->ATT_AngleTheta=xita;
    ATT_AngleDataStruct->ATT_AnglePhi=phi;
    ATT_AngleDataStruct->ATT_AnglePsi=psi;
    // ATT_AngleDataStruct->ATT_AnglePsi=0;
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
    float_t c11,c12,c13,c23,c33;
    float_t q0,q1,q2,q3;

    q0=ATT_QuatDataStruct->ATT_Quat0;
    q1=ATT_QuatDataStruct->ATT_Quat1;
    q2=ATT_QuatDataStruct->ATT_Quat2;
    q3=ATT_QuatDataStruct->ATT_Quat3;

    c11=q0*q0+q1*q1-q2*q2-q3*q3;
    c12=2*(q1*q2-q0*q3);
    c13=2*(q1*q3+q0*q2);
    c23=2*(q2*q3-q0*q1);
    c33=q0*q0-q1*q1-q2*q2+q3*q3;

    ATT_AngleDataStruct->ATT_AnglePhi=atan2(c23,c33)*53.7;
    ATT_AngleDataStruct->ATT_AngleTheta=asin(-c13)*53.7;
    ATT_AngleDataStruct->ATT_AnglePsi=atan2(c12,c11)*53.7;
}