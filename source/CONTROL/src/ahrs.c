#include "ahrs.h"

#define DT 20.0f//20ms，进去除1000
float_t AHRS_B0x,AHRS_B0z;

/**
 * @brief   通过陀螺仪获取A'状态转移矩阵
 * @param   MPU6050_FloatDataStruct: 6050浮点数据
 * @param   A: 状态转移矩阵
 **/
void AHRS_GetA(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
               arm_matrix_instance_f32* A)
{
    float_t wx,wy,wz;
    float_t _wx,_wy,_wz;
    wx=MPU6050_FloatDataStruct->MPU6050_FloatGyroX/2*DT;
    wy=MPU6050_FloatDataStruct->MPU6050_FloatGyroY/2*DT;
    wz=MPU6050_FloatDataStruct->MPU6050_FloatGyroZ/2*DT;
    _wx=-wx;
    _wy=-wy;
    _wz=-wz;
    float_t Aparam[16]=
    {
        1,     _wx,    _wy,    _wz,
        wx,     1,      wz,    _wy,
        wy,    _wz,     1,      wx,
        wz,     wy,    _wx,     1
    };
    arm_mat_init_f32(A,4,4,Aparam);
}

/**
 * @brief   获取C'观测矩阵
 * @param   AK8975_FloatDataStruct: 磁罗盘数据
 * @param   ATT_QuatDataStruct: 四元数
 * @param   C: 观测矩阵
 **/
void AHRS_GetC(AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
               ATT_QuatDataTypeDef* ATT_QuatDataStruct,
               arm_matrix_instance_f32* C)
{
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
    float_t Cparam[12]=
    {
        dq2,   _dq3,    dq0,   _dq1,
       _dq1,   _dq0,   _dq3,   _dq2,
       _dq0,    dq1,    dq2,   _dq3
    };
    // C[12]=dq0*AHRS_B0x+_dq2*AHRS_B0z; C[13]=dq1*AHRS_B0x+dq3*AHRS_B0z;
    // C[14]=_dq2*AHRS_B0x+_dq0*AHRS_B0z;C[15]=_dq3*AHRS_B0x+dq1*AHRS_B0z;
    // C[16]=_dq3*AHRS_B0x+dq1*AHRS_B0z; C[17]=dq2*AHRS_B0x+dq0*AHRS_B0z;
    // C[18]=dq1*AHRS_B0x+dq3*AHRS_B0z;  C[19]=_dq0*AHRS_B0x+dq2*AHRS_B0z;
    // C[20]=dq2*AHRS_B0x+dq0*AHRS_B0z;  C[21]=dq3*AHRS_B0x+_dq1*AHRS_B0z;
    // C[22]=dq0*AHRS_B0x+_dq2*AHRS_B0z; C[23]=dq1*AHRS_B0x+dq3*AHRS_B0z;
    arm_mat_init_f32(C,3,4,Cparam);
}

/**
 * @brief   从AT24C02获取参数，设为全局变量
 **/
void AHRS_GetGeomagneticVector()
{
    AT24C02_SequentialRead(0x10,4,(uint8_t*)&AHRS_B0x);
    AT24C02_SequentialRead(0x14,4,(uint8_t*)&AHRS_B0z);
}


/**
 * @brief   扩展卡尔曼计算
 * @param   AHRS_EKFParamStruct: 机体四元数
 **/
void AHRS_EKF(AHRS_EKFParamTypeDef* AHRS_EKFParamStruct)
{
    arm_matrix_instance_f32 P1,P2,H1,H2,H2_,At,Ct;
    //X(k)=A*X(k-1)
    arm_mat_mult_f32(&AHRS_EKFParamStruct->A,&AHRS_EKFParamStruct->X,
                     &AHRS_EKFParamStruct->X);
    //A*P(k-1)*At+Q
    arm_mat_mult_f32(&AHRS_EKFParamStruct->A,&AHRS_EKFParamStruct->P,
                     &P1);
    arm_mat_trans_f32(&AHRS_EKFParamStruct->A,&At);
    arm_mat_mult_f32(&P1,&At,
                     &AHRS_EKFParamStruct->P);
    arm_mat_add_f32(&AHRS_EKFParamStruct->P,&AHRS_EKFParamStruct->Q,
                    &AHRS_EKFParamStruct->P);
    //H(k)
    arm_mat_trans_f32(&AHRS_EKFParamStruct->C,&Ct);
    arm_mat_mult_f32(&AHRS_EKFParamStruct->P,&Ct,
                     &H1);
    arm_mat_mult_f32(&AHRS_EKFParamStruct->C,&AHRS_EKFParamStruct->P,
                     &P2);
    arm_mat_mult_f32(&P2,&Ct,&H2);
    arm_mat_add_f32(&H2,&AHRS_EKFParamStruct->R,&H2);
    arm_mat_inverse_f32(&H2,&H2_);
    arm_mat_mult_f32(&H1,&H2_,&AHRS_EKFParamStruct->H);
    //X(k)校正
    arm_matrix_instance_f32 X1,Z1;
    arm_mat_mult_f32(&AHRS_EKFParamStruct->C,&AHRS_EKFParamStruct->X,&Z1);
    arm_mat_sub_f32(&AHRS_EKFParamStruct->Z,&Z1,&Z1);
    arm_mat_mult_f32(&AHRS_EKFParamStruct->H,&Z1,&X1);
    arm_mat_add_f32(&AHRS_EKFParamStruct->X,&X1,&AHRS_EKFParamStruct->X);
    //P(k)校正
    arm_matrix_instance_f32 I,I1,I2;//单位阵
    arm_mat_mult_f32(&AHRS_EKFParamStruct->H,&AHRS_EKFParamStruct->C,&I1);
    arm_mat_sub_f32(&I,&I1,&I2);
}