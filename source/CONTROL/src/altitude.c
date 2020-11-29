#include "altitude.h"

extern GPS_DataTypeDef GPS_DataStruct;//引入GPS数据的全局变量

Filter_CompParamTypeDef ALT_AccCompParamStruct;//加速度环互补滤波参数
Filter_CompParamTypeDef ALT_SpeedCompParamStruct;//速度环互补滤波参数

Filter_CompInfoTypeDef ALT_AccZCompInfoStruct;//加速度环Z轴数据
Filter_CompInfoTypeDef ALT_SpeedZCompInfoStruct;//速度环Z轴数据

float_t accZ;

/**
 * @brief   无人机定高，三阶互补滤波，先试试互补个速度看看
 * @param   MPU6050_CalDataStruct: 校正后的加速度的值
 * @param   SPL06_FloatDataStruct: 气压计测得的海拔高度和速度
 * @param   GPS_DataStruct: GPS测得的海拔高度（全局变量）
 * @param   ATT_QuatDataStruct: 无人机当前姿态
 **/
void ALT_3OrderCompUpdate(MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
                          SPL06_FloatDataTypeDef* SPL06_FloatDataStruct,
                          ATT_QuatDataTypeDef* ATT_QuatDataStruct)
{
    //计算导航坐标系下Z轴方向的整合加速度
    Mat_RotQuatTypeDef Mat_RotQuatStruct;
    Mat_RotDataTypeDef Mat_RotDataStruct;

    Mat_RotQuatStruct.Quat0=ATT_QuatDataStruct->ATT_Quat0;
    Mat_RotQuatStruct.Quat1=ATT_QuatDataStruct->ATT_Quat1;
    Mat_RotQuatStruct.Quat2=ATT_QuatDataStruct->ATT_Quat2;
    Mat_RotQuatStruct.Quat3=ATT_QuatDataStruct->ATT_Quat3;

    Mat_RotDataStruct.Data1=MPU6050_CalDataStruct->MPU6050_CalAccelX;
    Mat_RotDataStruct.Data2=MPU6050_CalDataStruct->MPU6050_CalAccelY;
    Mat_RotDataStruct.Data3=MPU6050_CalDataStruct->MPU6050_CalAccelZ;

    Mat_TransRotQuat(&Mat_RotQuatStruct,&Mat_RotDataStruct);

    Mat_RotDataStruct.Data3-=1;//减去重力加速度

    accZ=Mat_RotDataStruct.Data3;
    
    //加速度环互补
    ALT_AccZCompInfoStruct.Error=SPL06_FloatDataStruct->SPL06_FloatSpeed-
                                 ALT_AccZCompInfoStruct.Output;
    ALT_AccZCompInfoStruct.Input=Mat_RotDataStruct.Data3;
    Filter_Comp(&ALT_AccCompParamStruct,&ALT_AccZCompInfoStruct);

    //速度环互补
    ALT_SpeedZCompInfoStruct.Error=SPL06_FloatDataStruct->SPL06_FloatAlt-
                                   ALT_SpeedZCompInfoStruct.Output;
    ALT_SpeedZCompInfoStruct.Input=ALT_AccZCompInfoStruct.Output;
    Filter_Comp(&ALT_SpeedCompParamStruct,&ALT_SpeedZCompInfoStruct);
}

/**
 * @brief   无人机定高三阶互补滤波C(s)参数初始化
 **/

void ALT_3OrderCompParamInit(void)
{
    ALT_AccCompParamStruct.Kp=1.f;
    ALT_AccCompParamStruct.Ki=0.0005f;

    ALT_SpeedCompParamStruct.Kp=1.f;
    ALT_SpeedCompParamStruct.Ki=0.0005f;
}