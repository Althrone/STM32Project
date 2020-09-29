#include "ahrs.h"

#define DT 0.02f//20ms，进去除1000

//以下就是卡尔曼滤波非临时矩阵的存储区域

float32_t Xdata[4]={1,0,0,0};
float32_t Pdata[16]={1,0,0,0,
                     0,1,0,0,
                     0,0,1,0,
                     0,0,0,1};
float32_t Qdata[16]={0};
float32_t Rdata[9]={0};

void AHRS_InitX(arm_matrix_instance_f32* X)
{
    arm_mat_init_f32(X,4,1,Xdata);
}

void AHRS_InitP(arm_matrix_instance_f32* P)
{
    arm_mat_init_f32(P,4,4,Pdata);
}

void AHRS_InitQ(arm_matrix_instance_f32* Q)
{
    Qdata[0]=0.0001;
    Qdata[5]=0.0001;
    Qdata[10]=0.0001;
    Qdata[15]=0.0001;
    arm_mat_init_f32(Q,4,4,Qdata);
}

void AHRS_InitR(arm_matrix_instance_f32* R)
{
    Rdata[0]=0.0001;
    Rdata[4]=0.0001;
    Rdata[8]=0.0001;
    arm_mat_init_f32(R,3,3,Rdata);
}

void AHRS_EKF(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
              ATT_AngleDataTypeDef* ATT_AngleDataStruct)
{
    arm_matrix_instance_f32 X,P,Q,R;
    AHRS_InitX(&X);
    AHRS_InitX(&P);
    AHRS_InitX(&Q);
    AHRS_InitX(&R);

    //获取A矩阵
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
    arm_matrix_instance_f32 A;
    arm_mat_init_f32(&A,4,4,Aparam);

    //获取Z矩阵
    float_t norm=Fast_InvSqrt(MPU6050_FloatDataStruct->MPU6050_FloatAccelX*MPU6050_FloatDataStruct->MPU6050_FloatAccelX+
                              MPU6050_FloatDataStruct->MPU6050_FloatAccelY*MPU6050_FloatDataStruct->MPU6050_FloatAccelY+
                              MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*MPU6050_FloatDataStruct->MPU6050_FloatAccelZ);
    float_t Zparam[3]=
    {
        MPU6050_FloatDataStruct->MPU6050_FloatAccelX*norm,
        MPU6050_FloatDataStruct->MPU6050_FloatAccelY*norm,
        MPU6050_FloatDataStruct->MPU6050_FloatAccelZ*norm,
    };
    arm_matrix_instance_f32 Z;
    arm_mat_init_f32(&Z,3,1,Zparam);

    //获取C矩阵
    float_t dq0,dq1,dq2,dq3;
    float_t _dq0,_dq1,_dq2,_dq3;
    dq0=2*Xdata[0];
    dq1=2*Xdata[1];
    dq2=2*Xdata[2];
    dq3=2*Xdata[3];
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
    arm_matrix_instance_f32 C;
    arm_mat_init_f32(&C,3,4,Cparam);

    //初始化H矩阵
    float_t Hdata[12]={0};
    arm_matrix_instance_f32 H;
    arm_mat_init_f32(&H,3,4,Hdata);

    //X(k|k-1)=A*X(k-1|k-1)，状态估计
    arm_mat_mult_f32(&A,&X,&X);

    //P(k|k-1)=A*P(k-1|k-1)*AT+Q，状态协方差估计
    float_t Ptmp1data[16]={0};
    float_t ATdata[16]={0};
    arm_matrix_instance_f32 Ptmp1,AT;
    arm_mat_init_f32(&Ptmp1,4,4,Ptmp1data);
    arm_mat_init_f32(&AT,4,4,ATdata);

    arm_mat_mult_f32(&A,&P,&Ptmp1);
    arm_mat_trans_f32(&A,&AT);
    arm_mat_mult_f32(&Ptmp1,&AT,&P);
    arm_mat_mult_f32(&P,&Q,&P);

    //H(k)=(P(k|k-1)*CT)/(C*P(k|k-1)*CT+R)
    float_t CTdata[12]={0};
    float_t Htmp1data[12]={0};
    float_t Htmp2data[9]={0};
    float_t Ptmp2data[12]={0};
    float_t Htmp2Idata[9]={0};
    arm_matrix_instance_f32 CT,Htmp1,Htmp2,Ptmp2,Htmp2I;
    arm_mat_init_f32(&CT,4,3,CTdata);
    arm_mat_init_f32(&Htmp1,4,3,Htmp1data);
    arm_mat_init_f32(&Htmp2,4,3,Htmp2data);
    arm_mat_init_f32(&Ptmp2,3,4,Ptmp2data);
    arm_mat_init_f32(&Htmp2I,3,3,Htmp2Idata);

    arm_mat_trans_f32(&C,&CT);
    arm_mat_mult_f32(&P,&CT,&Htmp1);
    arm_mat_mult_f32(&C,&P,&Ptmp2);
    arm_mat_mult_f32(&Ptmp2,&CT,&Htmp2);
    arm_mat_add_f32(&Htmp2,&R,&Htmp2);
    arm_mat_inverse_f32(&Htmp2,&Htmp2I);
    arm_mat_mult_f32(&Htmp1,&Htmp2I,&H);

    //X(k|k)=X(k|k-1)+H(k)*[Z(k)-C(k)*X(k|k-1)]，状态校正
    float_t Ztmp1data[3]={0};
    float_t Xtmp1data[4]={0};
    arm_matrix_instance_f32 Xtmp1,Ztmp1;
    arm_mat_init_f32(&Ztmp1,3,1,Ztmp1data);
    arm_mat_init_f32(&Xtmp1,4,1,Xtmp1data);

    arm_mat_mult_f32(&C,&X,&Ztmp1);
    arm_mat_sub_f32(&Z,&Ztmp1,&Ztmp1);
    arm_mat_mult_f32(&H,&Ztmp1,&Xtmp1);
    arm_mat_add_f32(&X,&Xtmp1,&X);

    //P(k|k)=[I-H(k)*C(k)]*P(k|k-1)，状态协方差校正
    float_t Idata[16]={1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1};
    float_t Itmp1data[16]={0};
    arm_matrix_instance_f32 I,Itmp1;
    arm_mat_init_f32(&I,4,4,Idata);
    arm_mat_init_f32(&Itmp1,4,4,Itmp1data);

    arm_mat_mult_f32(&H,&C,&Itmp1);
    arm_mat_sub_f32(&I,&Itmp1,&Itmp1);
    arm_mat_mult_f32(&Itmp1,&P,&P);

    //四元数转角度
    ATT_QuatDataTypeDef ATT_QuatDataStruct;
    ATT_QuatDataStruct.ATT_Quat0=Xdata[0];
    ATT_QuatDataStruct.ATT_Quat1=Xdata[1];
    ATT_QuatDataStruct.ATT_Quat2=Xdata[2];
    ATT_QuatDataStruct.ATT_Quat3=Xdata[3];
    ATT_Quat2Angle(&ATT_QuatDataStruct,ATT_AngleDataStruct);
}