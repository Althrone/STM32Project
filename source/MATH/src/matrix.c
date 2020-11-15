#include "matrix.h"

/**
 * @brief   旋转矩阵，欧拉角模式
 * @param   Mat_RotEulerStruct: 旋转矩阵的欧拉角
 * @param   Mat_RotDataStruct: 需要旋转的数据
 **/
void Mat_RotEuler(Mat_RotEulerTypeDef* Mat_RotEulerStruct,
                  Mat_RotDataTypeDef* Mat_RotDataStruct)
{
    //预计算
    float_t sinPhi,sinTheta,sinPsi,
            cosPhi,cosTheta,cosPsi;
    sinPhi=arm_sin_f32(Mat_RotEulerStruct->Phi);
    cosPhi=arm_cos_f32(Mat_RotEulerStruct->Phi);
    sinTheta=arm_sin_f32(Mat_RotEulerStruct->Theta);
    cosTheta=arm_cos_f32(Mat_RotEulerStruct->Theta);
    sinPsi=arm_sin_f32(Mat_RotEulerStruct->Psi);
    cosPsi=arm_cos_f32(Mat_RotEulerStruct->Psi);

    //初始化旋转矩阵
    arm_matrix_instance_f32 rotMat;
    float_t rotMatData[9]=
    {
        cosTheta*cosPsi,                        cosTheta*sinPsi,                       -sinTheta,
        sinPhi*sinTheta*cosPsi-cosPhi*sinPsi,   sinPhi*sinTheta*sinPsi+cosPhi*cosPsi,   sinPhi*cosTheta,
        cosPhi*sinTheta*cosPsi+sinPhi*sinPsi,   cosPhi*sinTheta*sinPsi-sinPhi*cosPsi,   cosPhi*cosTheta
    };
    arm_mat_init_f32(&rotMat,3,3,rotMatData);

    //初始化输出矩阵
    arm_matrix_instance_f32 inputMat;
    float_t inputMatData[3]={0};
    arm_mat_init_f32(&inputMat,3,1,inputMatData);

    //初始化输入矩阵
    arm_matrix_instance_f32 outputMat;
    float_t outputMatData[3]=
    {
        Mat_RotDataStruct->Data1,
        Mat_RotDataStruct->Data2,
        Mat_RotDataStruct->Data3
    };
    arm_mat_init_f32(&outputMat,3,1,outputMatData);

    arm_mat_mult_f32(&rotMat,&inputMat,&outputMat);

    Mat_RotDataStruct->Data1=inputMatData[0];
    Mat_RotDataStruct->Data2=inputMatData[1];
    Mat_RotDataStruct->Data3=inputMatData[2];
}

/**
 * @brief   旋转矩阵，四元数模式
 * @param   Mat_RotQuatStruct: 旋转矩阵的四元数
 * @param   Mat_RotDataStruct: 需要旋转的数据
 **/
void Mat_RotQuat(Mat_RotQuatTypeDef* Mat_RotQuatStruct,
                 Mat_RotDataTypeDef* Mat_RotDataStruct)
{
    //预计算
    float_t q0,q1,q2,q3;
    q0=Mat_RotQuatStruct->Quat0;
    q1=Mat_RotQuatStruct->Quat1;
    q2=Mat_RotQuatStruct->Quat2;
    q3=Mat_RotQuatStruct->Quat3;

    float_t q0q0,q0q1,q0q2,q0q3,
            q1q1,q1q2,q1q3,
            q2q2,q2q3,
            q3q3;
    q0q0=q0*q0;q0q1=q0*q1;q0q2=q0*q2;q0q3=q0*q3;
    q1q1=q1*q1;q1q2=q1*q2;q1q3=q1*q3;
    q2q2=q2*q2;q2q3=q2*q3;
    q3q3=q3*q3;
    //初始化旋转矩阵
    arm_matrix_instance_f32 rotMat;
    float_t rotMatData[9]=
    {
        2*(q0q0+q1q1)-1,    2*(q1q2+q0q3),      2*(q1q3-q0q2),
        2*(q1q2-q0q3),      2*(q0q0+q2q2)-1,    2*(q2q3+q0q1),
        2*(q1q3+q0q2),      2*(q2q3-q0q1),      2*(q0q0+q3q3)-1
    };
    arm_mat_init_f32(&rotMat,3,3,rotMatData);

    //初始化输出矩阵
    arm_matrix_instance_f32 inputMat;
    float_t inputMatData[3]={0};
    arm_mat_init_f32(&inputMat,3,1,inputMatData);

    //初始化输入矩阵
    arm_matrix_instance_f32 outputMat;
    float_t outputMatData[3]=
    {
        Mat_RotDataStruct->Data1,
        Mat_RotDataStruct->Data2,
        Mat_RotDataStruct->Data3
    };
    arm_mat_init_f32(&outputMat,3,1,outputMatData);

    arm_mat_mult_f32(&rotMat,&inputMat,&outputMat);

    Mat_RotDataStruct->Data1=inputMatData[0];
    Mat_RotDataStruct->Data2=inputMatData[1];
    Mat_RotDataStruct->Data3=inputMatData[2];
}

/**
 * @brief   转置旋转矩阵，四元数模式
 * @param   Mat_RotQuatStruct: 旋转矩阵的四元数
 * @param   Mat_RotDataStruct: 需要旋转的数据
 **/
void Mat_TransRotQuat(Mat_RotQuatTypeDef* Mat_RotQuatStruct,
                      Mat_RotDataTypeDef* Mat_RotDataStruct)
{
    //预计算
    float_t q0,q1,q2,q3;
    q0=Mat_RotQuatStruct->Quat0;
    q1=Mat_RotQuatStruct->Quat1;
    q2=Mat_RotQuatStruct->Quat2;
    q3=Mat_RotQuatStruct->Quat3;

    float_t q0q0,q0q1,q0q2,q0q3,
            q1q1,q1q2,q1q3,
            q2q2,q2q3,
            q3q3;
    q0q0=q0*q0;q0q1=q0*q1;q0q2=q0*q2;q0q3=q0*q3;
    q1q1=q1*q1;q1q2=q1*q2;q1q3=q1*q3;
    q2q2=q2*q2;q2q3=q2*q3;
    q3q3=q3*q3;
    //初始化旋转矩阵
    arm_matrix_instance_f32 rotMat;
    float_t rotMatData[9]=
    {
        2*(q0q0+q1q1)-1,    2*(q1q2-q0q3),      2*(q1q3+q0q2),
        2*(q1q2+q0q3),      2*(q0q0+q2q2)-1,    2*(q2q3-q0q1),
        2*(q1q3-q0q2),      2*(q2q3+q0q1),      2*(q0q0+q3q3)-1
    };
    arm_mat_init_f32(&rotMat,3,3,rotMatData);

    //初始化输出矩阵
    arm_matrix_instance_f32 inputMat;
    float_t inputMatData[3]={0};
    arm_mat_init_f32(&inputMat,3,1,inputMatData);

    //初始化输入矩阵
    arm_matrix_instance_f32 outputMat;
    float_t outputMatData[3]=
    {
        Mat_RotDataStruct->Data1,
        Mat_RotDataStruct->Data2,
        Mat_RotDataStruct->Data3
    };
    arm_mat_init_f32(&outputMat,3,1,outputMatData);

    arm_mat_mult_f32(&rotMat,&inputMat,&outputMat);

    Mat_RotDataStruct->Data1=inputMatData[0];
    Mat_RotDataStruct->Data2=inputMatData[1];
    Mat_RotDataStruct->Data3=inputMatData[2];
}