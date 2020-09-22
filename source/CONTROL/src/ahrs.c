#include "ahrs.h"

#define DT 20.0f//20ms，进去除1000



/**
 * @brief   通过陀螺仪获取A'状态转移矩阵
 * @param   MPU6050_FloatDataStruct: 6050浮点数据
 * @param   A: 状态转移矩阵
 **/
void AHRS_GetA(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
               float_t* A)
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
 * @param   AK8975_FloatDataStruct: 磁罗盘数据
 * @param   ATT_QuatDataStruct: 四元数
 * @param   C: 观测矩阵
 **/
void AHRS_GetC(AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
               ATT_QuatDataTypeDef* ATT_QuatDataStruct,
               float_t* C)
{
    float_t B0x,B0y,B0z;
    B0x=AK8975_FloatDataStruct->AK8975_FloatMagX;
    B0y=AK8975_FloatDataStruct->AK8975_FloatMagY;
    B0z=AK8975_FloatDataStruct->AK8975_FloatMagZ;
    float_t dq0,dq1,dq2,dq3;
    float_t _dq0,_dq1,_dq2,_dq3;
    dq0=2*ATT_QuatDataStruct->ATT_Quat0;
    dq1=2*ATT_QuatDataStruct->ATT_Quat1;
    dq2=2*ATT_QuatDataStruct->ATT_Quat2;
    dq3=2*ATT_QuatDataStruct->ATT_Quat3;
    _dq0=-dq0;
    _dq1=-dq1;
    _dq2=-dq2;
    _dq3=-dq3;

    C[0]=dq2;   C[1]=_dq3;  C[2]=dq0;   C[3]=_dq1;
    C[4]=_dq1;  C[5]=_dq0;  C[6]=_dq3;  C[7]=_dq2;
    C[8]=_dq0;  C[9]=dq1;   C[10]=dq2;  C[11]=_dq3;
    C[12]=dq0*B0x+_dq2*B0z;C[13]=dq0*B0x+_dq2*B0z;C[13]=dq0*B0x+_dq2*B0z;C[13]=dq0*B0x+_dq2*B0z;

}