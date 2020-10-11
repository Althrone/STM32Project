#include "fly.h"

//角度环，遥控器的俯仰和横滚摇杆给的是角度值
PID_InfoTypeDef PID_RollAngleInfo;
PID_InfoTypeDef PID_PitchAngleInfo;
//角速度环，遥控器的偏航杆给的是角速度值
PID_InfoTypeDef PID_RollRateInfo;
PID_InfoTypeDef PID_PitchRateInfo;
PID_InfoTypeDef PID_YawRateInfo;
//对应的pid参数
extern PID_ParamTypeDef PID_RollAngleParam;
extern PID_ParamTypeDef PID_PitchAngleParam;

extern PID_ParamTypeDef PID_RollRateParam;
extern PID_ParamTypeDef PID_PitchRateParam;
extern PID_ParamTypeDef PID_YawRateParam;

/**
 * @brief   无人机控制，分配动力到各轴
 * @param   ANO_DT_SendRCDataStruct: 遥控器送到的数值，也就是飞机的目标姿态
 * @param   ATT_AngleDataStruct: 传感器解算的无人机当前姿态
 **/
void FLY_DroneCtrl(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct,
                   ATT_AngleDataTypeDef* ATT_AngleDataStruct,
                   MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct)
{
    //俯仰，横滚转化成角度值
    //偏航转化成角速度值
    float_t RollTargetAngle,PitchTargetAngle;
    RollTargetAngle=(float_t)(ANO_DT_SendRCDataStruct->ANO_DT_RCRoll-1500)/500.f*30;//量程+-30°
    PitchTargetAngle=(float_t)(ANO_DT_SendRCDataStruct->ANO_DT_RCPitch-1500)/500.f*30;//量程+-30°
    float_t YawTargeRate;
    YawTargeRate=(float_t)(ANO_DT_SendRCDataStruct->ANO_DT_RCYaw-1500)/500.f*30;//量程+-30°/s
    //角度环
    PID_IncCtrl(&PID_RollAngleInfo,
                &PID_RollAngleParam,
                RollTargetAngle,
                ATT_AngleDataStruct->ATT_AnglePhi);
    PID_IncCtrl(&PID_PitchAngleInfo,
                &PID_PitchAngleParam,
                PitchTargetAngle,
                ATT_AngleDataStruct->ATT_AngleTheta);
    //角速度环
    PID_IncCtrl(&PID_RollRateInfo,
                &PID_RollRateParam,
                PID_RollAngleInfo.Output,
                MPU6050_FloatDataStruct->MPU6050_FloatGyroX);
    PID_IncCtrl(&PID_PitchRateInfo,
                &PID_PitchRateParam,
                PID_PitchAngleInfo.Output,
                MPU6050_FloatDataStruct->MPU6050_FloatGyroY);
    PID_IncCtrl(&PID_YawRateInfo,
                &PID_YawRateParam,
                YawTargeRate,
                MPU6050_FloatDataStruct->MPU6050_FloatGyroZ);
    //混控矩阵
    float32_t MixerData[16]=
    {
       -0.70710678f,   -0.70710678f,    1.0f,   1.0f,
        0.70710678f,    0.70710678f,    1.0f,   1.0f,
        0.70710678f,   -0.70710678f,    1.0f,  -1.0f,
       -0.70710678f,    0.70710678f,    1.0f,  -1.0f
    };
    arm_matrix_instance_f32 MixerMatrix;
    arm_mat_init_f32(&MixerMatrix,4,4,MixerData);
    //遥控参数矩阵
    float32_t RCData[4]=
    {
        PID_RollRateInfo.Output,
        PID_PitchRateInfo.Output,
        ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle,
        PID_YawRateInfo.Output,
    };
    arm_matrix_instance_f32 RCMatrix;
    arm_mat_init_f32(&RCMatrix,4,1,RCData);
    //电机输出矩阵
    float32_t MotoData[4]={0};
    arm_matrix_instance_f32 MotoMatrix;
    arm_mat_init_f32(&MotoMatrix,4,1,MotoData);
    arm_mat_mult_f32(&MixerMatrix,&RCMatrix,&MotoMatrix);

    TIM_SetCompare1(TIM3,(uint32_t)MotoData[0]*2);
    TIM_SetCompare2(TIM3,(uint32_t)MotoData[1]*2);
    TIM_SetCompare3(TIM3,(uint32_t)MotoData[2]*2);
    TIM_SetCompare4(TIM3,(uint32_t)MotoData[3]*2);

    ANO_DT_SendMotoTypeDef ANO_DT_SendMotoStruct;
    ANO_DT_SendMotoStruct.ANO_DT_Moto1=(uint32_t)MotoData[0]*2;
    ANO_DT_SendMotoStruct.ANO_DT_Moto2=(uint32_t)MotoData[0]*2;
    ANO_DT_SendMotoStruct.ANO_DT_Moto3=(uint32_t)MotoData[0]*2;
    ANO_DT_SendMotoStruct.ANO_DT_Moto4=(uint32_t)MotoData[0]*2;
    ANO_DT_SendMoto(USART1,&ANO_DT_SendMotoStruct);
}