#include "attitude.h"

void ATT_RawData(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                 AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
                 ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct)
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

    xita=asin(a_x*nomo)*53.7;
    phi=atan(a_y/a_z)*53.7;
    
    Bx=AK8975_FloatDataStruct->AK8975_FloatMagX*cos(xita)+
       AK8975_FloatDataStruct->AK8975_FloatMagY*sin(xita)*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(xita)*cos(phi);
    By=AK8975_FloatDataStruct->AK8975_FloatMagY*cos(phi)+
       AK8975_FloatDataStruct->AK8975_FloatMagZ*sin(phi);

    if((By==0)&&(Bx>0))
    psi=0;
    if(By<0)
    psi=(3.14f/2+atan(Bx/By))*53.7;
    if((By==0)&&(Bx<0))
    psi=180;
    if(By>0)
    psi=(3*3.14f/2+atan(Bx/By))*53.7;


    ANO_DT_SendStatusStruct->ANO_DT_Pitch=(int16_t)xita*100;
    ANO_DT_SendStatusStruct->ANO_DT_Roll=(int16_t)phi*100;
    ANO_DT_SendStatusStruct->ANO_DT_Yaw=(int16_t)psi*100;
    // ANO_DT_SendStatusStruct->ANO_DT_Yaw=0;
}