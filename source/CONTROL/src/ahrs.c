#include "ahrs.h"

#define DT 20.0f//20ms，进去除1000

/**
 * @brief   通过陀螺仪获取A'状态转移矩阵
 * @param   MPU6050_FloatDataStruct: 6050浮点数据
 * @param   A: 状态转移矩阵
 **/
void AHRS_GetA(float_t* A ,MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct)
{
    float_t wx,wy,wz;
    float_t _wx,_wy,_wz;
    wx=MPU6050_FloatDataStruct->MPU6050_FloatGyroX/2*DT;
    wy=MPU6050_FloatDataStruct->MPU6050_FloatGyroY/2*DT;
    wz=MPU6050_FloatDataStruct->MPU6050_FloatGyroZ/2*DT;
    _wx=-wx;
    _wy=-wy;
    _wz=-wz;

    A[0]=1;     A[1]=_wx;   A[2]=_wy;   A[3]=_wz;
    A[4]=wx;    A[5]=1;     A[6]=wz;    A[7]=_wy;
    A[8]=wy;    A[9]=_wz;   A[10]=1;    A[11]=wx;
    A[12]=wz;   A[13]=wy;   A[14]=_wx;  A[15]=1;
}

/**
 * @brief   获取C'观测矩阵
 * @param   
 * @param   C: 观测矩阵
 **/
void AHRS_GetC(float_t* C,AK8975_FloatDataTypeDef* AK8975_FloatDataStruct)
{
    float_t B0x,B0y,B0z;
    B0x=AK8975_FloatDataStruct->AK8975_FloatMagX;
    B0y=AK8975_FloatDataStruct->AK8975_FloatMagY;
    B0z=AK8975_FloatDataStruct->AK8975_FloatMagZ;
}