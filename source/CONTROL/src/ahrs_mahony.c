/**
 *******************************************************************************
 * @file    source/CONTROL/src/ahrs_mahony.c
 * @brief   Madgwick's implementation of Mayhony's AHRS algorithm.
 *          See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
 * Date         Author          Notes
 * 29/09/2011   SOH Madgwick    Initial release
 * 02/10/2011   SOH Madgwick    Optimised for reduced CPU load
 * 07/10/2020   Althrone        汉化并适配自己的代码，未使用磁力计
 *******************************************************************************
 **/

#include "ahrs_mahony.h"


// Definitions
#define sampleFreq	100.0f			// sample frequency in Hz
#define twoKpDef	(2.0f * 1.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0005f)	// 2 * integral gain

// Variable definitions
volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki

/**
 * @brief   Mahony互补滤波算法计算无人机姿态
 * @param   MPU6050_FloatDataStruct: 提供加速度和角速度参数
 * @param   AK8975_FloatDataStruct: 校正偏航值
 * @param   ATT_AngleDataStruct: 输出无人机姿态角，角度制
 **/
void AHRS_MahonyUpdate(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                       AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
                       ATT_AngleDataTypeDef* ATT_AngleDataStruct)
{
    float_t norm;//归一化参数
    float_t q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    // float_t hx, hy;//计算地磁矢量的中间量
    // float_t bx, bz;//地磁矢量
    float_t halfvx, halfvy, halfvz;//加速度计估计的姿态向量
    // float_t halfwx, halfwy, halfwz;//磁罗盘估计的资态向量
    float_t halfex, halfey, halfez;//姿态估计误差
    // float_t qx, qy, qz;
    float_t ax, ay, az;//测量得到的加速度各轴分量
    float_t gx, gy, gz;//测量得到的角速度各轴分量,弧度制

    ax=MPU6050_FloatDataStruct->MPU6050_FloatAccelX;
    ay=MPU6050_FloatDataStruct->MPU6050_FloatAccelY;
    az=MPU6050_FloatDataStruct->MPU6050_FloatAccelZ;

    gx=MPU6050_FloatDataStruct->MPU6050_FloatGyroX*0.01745;
    gy=MPU6050_FloatDataStruct->MPU6050_FloatGyroY*0.01745;
    gz=MPU6050_FloatDataStruct->MPU6050_FloatGyroZ*0.01745;

    //只有所有值非零的时候才进行计算，防止归一化的时候值变成NaN
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
    {
        //归一化加速度计测量值
        norm=Fast_InvSqrt(ax * ax + ay * ay + az * az);
        ax *= norm;
        ay *= norm;
        az *= norm;

        //归一化磁力计测量值

        //辅助变量，避免重复计算浮点乘法
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        //通过四元数估计各轴重力分量
        halfvx = q1q3 - q0q2;
        halfvy = q0q1 + q2q3;
        halfvz = q0q0 - 0.5f + q3q3;//q1q1 + q2q2 = 0.5?

        //计算测量值与估计值的叉积求取误差
        halfex = (ay * halfvz - az * halfvy);
        halfey = (az * halfvx - ax * halfvz);
        halfez = (ax * halfvy - ay * halfvx);

        // Compute and apply integral feedback if enabled
        if(twoKi > 0.0f) {
            integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
            integralFBy += twoKi * halfey * (1.0f / sampleFreq);
            integralFBz += twoKi * halfez * (1.0f / sampleFreq);
            gx += integralFBx;	// apply integral feedback
            gy += integralFBy;
            gz += integralFBz;
        }
        else {
            integralFBx = 0.0f;	// prevent integral windup
            integralFBy = 0.0f;
            integralFBz = 0.0f;
        }

        // Apply proportional feedback
        gx += twoKp * halfex;
        gy += twoKp * halfey;
        gz += twoKp * halfez;

        //应用四元素微分公式，用校正后的角速度计算飞机的姿态角
        gx/=sampleFreq*2;//ωx*dt/2
        gy/=sampleFreq*2;//ωy*dt/2
        gz/=sampleFreq*2;//ωz*dt/2

        //更新飞机姿态，X(k)=A*X(k-1)
        arm_matrix_instance_f32 X,A;
        float_t Xdata[4]=
        {
            q0,
            q1,
            q2,
            q3
        };
        float_t Adata[16]=
        {
            1,     -gx,    -gy,    -gz,
            gx,     1,      gz,    -gy,
            gy,    -gz,     1,      gx,
            gz,     gy,    -gx,     1,
        };
        arm_mat_init_f32(&X,4,1,Xdata);
        arm_mat_init_f32(&A,4,4,Adata);
        arm_mat_mult_f32(&A,&X,&X);

        //四元数归一化
        norm=Fast_InvSqrt(Xdata[0]*Xdata[0]+
                          Xdata[1]*Xdata[1]+
                          Xdata[2]*Xdata[2]+
                          Xdata[3]*Xdata[3]);

        //更新全局变量
        q0=Xdata[0]*norm;
        q1=Xdata[1]*norm;
        q2=Xdata[2]*norm;
        q3=Xdata[3]*norm;

        //四元数转欧拉角
        ATT_QuatDataTypeDef ATT_QuatDataStruct;
        ATT_QuatDataStruct.ATT_Quat0=q0;
        ATT_QuatDataStruct.ATT_Quat1=q1;
        ATT_QuatDataStruct.ATT_Quat2=q2;
        ATT_QuatDataStruct.ATT_Quat3=q3;
        ATT_Quat2Angle(&ATT_QuatDataStruct,ATT_AngleDataStruct);
    }
}