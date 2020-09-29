#include "ahrs.h"

#define DT 0.02f//20ms，进去除1000
// float_t AHRS_B0x,AHRS_B0z;//地磁矢量

/**
 * @brief   初始化X状态矩阵
 * @param   X: 状态矩阵，四元数，4X1
 **/
void AHRS_InitX(arm_matrix_instance_f32* X)
{
    static float32_t XParam[4]=
    {
        1,
        0,
        0,
        0
    };
    arm_mat_init_f32(X,4,1,XParam);
}

/**
 * @brief   通过陀螺仪获取A'状态转移矩阵
 * @param   MPU6050_FloatDataStruct: 6050浮点数据
 * @param   A: 状态转移矩阵，4X4
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
 * @brief   获取Z观测矩阵
 * @param   MPU6050_FloatDataStruct: 获取加速度计数据，记得单位化
 * @param   Z: 观测矩阵
 **/
void AHRS_GetZ(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
               arm_matrix_instance_f32* Z)
{
    float_t norm=Fast_InvSqrt(MPU6050_FloatDataStruct->MPU6050_FloatAccelX*MPU6050_FloatDataStruct->MPU6050_FloatAccelX+
                              MPU6050_FloatDataStruct->MPU6050_FloatAccelY*MPU6050_FloatDataStruct->MPU6050_FloatAccelY+
                              MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*MPU6050_FloatDataStruct->MPU6050_FloatAccelZ);
    float_t Zparam[3]=
    {
        MPU6050_FloatDataStruct->MPU6050_FloatAccelX*norm,
        MPU6050_FloatDataStruct->MPU6050_FloatAccelY*norm,
        MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*norm,
    };
    arm_mat_init_f32(Z,3,1,Zparam);
}

/**
 * @brief   获取C'观测矩阵
 * @param   X: 四元数
 * @param   C: 观测矩阵，3X4
 **/
void AHRS_GetC(arm_matrix_instance_f32* X,
               arm_matrix_instance_f32* C)
{
    float_t dq0,dq1,dq2,dq3;
    float_t _dq0,_dq1,_dq2,_dq3;
    dq0=2*X->pData[0];
    dq1=2*X->pData[1];
    dq2=2*X->pData[2];
    dq3=2*X->pData[3];
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
    //地磁矢量有点问题，准确来说是磁罗盘有点问题，先不用磁罗盘
    // C[12]=dq0*AHRS_B0x+_dq2*AHRS_B0z; C[13]=dq1*AHRS_B0x+dq3*AHRS_B0z;
    // C[14]=_dq2*AHRS_B0x+_dq0*AHRS_B0z;C[15]=_dq3*AHRS_B0x+dq1*AHRS_B0z;
    // C[16]=_dq3*AHRS_B0x+dq1*AHRS_B0z; C[17]=dq2*AHRS_B0x+dq0*AHRS_B0z;
    // C[18]=dq1*AHRS_B0x+dq3*AHRS_B0z;  C[19]=_dq0*AHRS_B0x+dq2*AHRS_B0z;
    // C[20]=dq2*AHRS_B0x+dq0*AHRS_B0z;  C[21]=dq3*AHRS_B0x+_dq1*AHRS_B0z;
    // C[22]=dq0*AHRS_B0x+_dq2*AHRS_B0z; C[23]=dq1*AHRS_B0x+dq3*AHRS_B0z;
    arm_mat_init_f32(C,3,4,Cparam);
}

// /**
//  * @brief   从AT24C02获取参数，设为全局变量
//  **/
// void AHRS_GetGeomagneticVector()
// {
//     AT24C02_SequentialRead(0x10,4,(uint8_t*)&AHRS_B0x);
//     AT24C02_SequentialRead(0x14,4,(uint8_t*)&AHRS_B0z);
// }
/**
 * @brief   初始化P状态矩阵协方差矩阵
 **/
void AHRS_InitP(arm_matrix_instance_f32* P)
{
    float32_t Pparam[16]=
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    arm_mat_init_f32(P,4,4,Pparam);
}

/**
 * @brief   初始化过程噪声方差矩阵
 * @param   Q: 过程噪声方差矩阵，需要灵魂调参，4X4
 **/
void AHRS_InitQ(arm_matrix_instance_f32* Q)
{
    float32_t Qparam[16]=
    {
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    };
    Qparam[0]=0.0001;
    Qparam[5]=0.0001;
    Qparam[10]=0.0001;
    Qparam[15]=0.0001;
    arm_mat_init_f32(Q,4,4,Qparam);
}

void AHRS_InitH(arm_matrix_instance_f32* H)
{
    float32_t Hparam[12]={0};
    arm_mat_init_f32(H,4,3,Hparam);
}

/**
 * @brief   获取测量噪声方差
 * @param   R: 噪声方差矩阵，3X3
 **/
void AHRS_InitR(arm_matrix_instance_f32* R)
{
    float32_t Rparam[9]=
    {
        0.0001,0,0,
        0,0.0001,0,
        0,0,0.0001
    };
    // AT24C02_SequentialRead(0x28,4,(uint8_t*)&Rparam[0]);
    // AT24C02_SequentialRead(0x2C,4,(uint8_t*)&Rparam[4]);
    // AT24C02_SequentialRead(0x30,4,(uint8_t*)&Rparam[8]);
    arm_mat_init_f32(R,3,3,Rparam);
}

/**
 * @brief   初始化一个单位阵，用于状态矩阵协方差矩阵校正
 * @param   I: 单位阵
 **/
void AHRS_InitI(arm_matrix_instance_f32* I)
{
    float32_t Iparam[16]=
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    arm_mat_init_f32(I,4,4,Iparam);
}

/**
 * @brief   扩展卡尔曼计算
 * @param   AHRS_EKFParamStruct: 机体四元数
 **/
void AHRS_EKF(AHRS_EKFParamTypeDef* AHRS_EKFParamStruct)
{
    int16_t status;
    //X(k)=A*X(k-1)，X(k)估计
    arm_mat_mult_f32(&AHRS_EKFParamStruct->A,&AHRS_EKFParamStruct->X,
                     &AHRS_EKFParamStruct->X);
    //P(k)=A*P(k-1)*At+Q，P(k)估计
    arm_matrix_instance_f32 P1,At;
    float32_t Init16[16]={0};
    arm_mat_init_f32(&P1,4,4,Init16);
    arm_mat_init_f32(&At,4,4,Init16);

    arm_mat_mult_f32(&AHRS_EKFParamStruct->A,&AHRS_EKFParamStruct->P,
                     &P1);
    arm_mat_trans_f32(&AHRS_EKFParamStruct->A,&At);
    arm_mat_mult_f32(&P1,&At,
                     &AHRS_EKFParamStruct->P);
    arm_mat_add_f32(&AHRS_EKFParamStruct->P,&AHRS_EKFParamStruct->Q,
                    &AHRS_EKFParamStruct->P);
    //H(k)
    arm_matrix_instance_f32 Ct,P2,H1,H2,H2_;
    float32_t Init9[9]={0};
    float32_t Init12[12]={0};
    arm_mat_init_f32(&Ct,4,3,Init12);
    arm_mat_init_f32(&H1,4,3,Init12);
    arm_mat_init_f32(&P2,3,4,Init12);
    arm_mat_init_f32(&H2,3,3,Init9);
    arm_mat_init_f32(&H2_,3,3,Init9);

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
    float32_t Init3[3]={0};
    float32_t Init4[4]={0};
    arm_mat_init_f32(&Z1,3,1,Init3);
    arm_mat_init_f32(&X1,4,1,Init4);

    arm_mat_mult_f32(&AHRS_EKFParamStruct->C,&AHRS_EKFParamStruct->X,&Z1);
    arm_mat_sub_f32(&AHRS_EKFParamStruct->Z,&Z1,&Z1);
    arm_mat_mult_f32(&AHRS_EKFParamStruct->H,&Z1,&X1);
    arm_mat_add_f32(&AHRS_EKFParamStruct->X,&X1,&AHRS_EKFParamStruct->X);
    //P(k)校正
    arm_matrix_instance_f32 I1,I2;
    arm_mat_init_f32(&I1,4,4,Init16);
    arm_mat_init_f32(&I2,4,4,Init16);

    arm_mat_mult_f32(&AHRS_EKFParamStruct->H,&AHRS_EKFParamStruct->C,&I1);
    arm_mat_sub_f32(&AHRS_EKFParamStruct->I,&I1,&I2);
    arm_mat_mult_f32(&I2,&AHRS_EKFParamStruct->P,
                     &AHRS_EKFParamStruct->P);
}