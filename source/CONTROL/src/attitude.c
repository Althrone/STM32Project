#include "attitude.h"

void ATT_RawData(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                 ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct)
{
    float_t xita,phi;
    float_t nomo;//用于单位化
    float_t a_x,a_y,a_z;

    nomo=FastInvSqrt(MPU6050_FloatDataStruct->MPU6050_FloatAccelX*MPU6050_FloatDataStruct->MPU6050_FloatAccelX+
                     MPU6050_FloatDataStruct->MPU6050_FloatAccelY*MPU6050_FloatDataStruct->MPU6050_FloatAccelY+
                     MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*MPU6050_FloatDataStruct->MPU6050_FloatAccelZ);

    a_x=MPU6050_FloatDataStruct->MPU6050_FloatAccelX/nomo;
    a_y=MPU6050_FloatDataStruct->MPU6050_FloatAccelY/nomo;
    a_z=MPU6050_FloatDataStruct->MPU6050_FloatAccelZ/nomo;

    xita=asin(a_x/9.8);
    phi=atan(a_y/a_z);

    // xita=(a_x/9.8);
    // phi=(a_y/a_z);

    ANO_DT_SendStatusStruct->ANO_DT_Pitch=(int16_t)xita*100;
    ANO_DT_SendStatusStruct->ANO_DT_Roll=(int16_t)phi*100;

}