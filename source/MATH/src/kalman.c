#include "kalman.h"

/**
 * @brief   初始化的时候调用，计算数据平均值，用于计算噪声方差
 * @param   MPU6050_FloatDataStruct: MPU6050浮点数据
 * @param   AK8975_FloatDataStruct: AK8975浮点数据
 * @param   Kalman_MPU6050MeanDataStruct: MPU6050数据平均值
 * @param   Kalman_AK8975MeanDataStruct: AK8975数据平均值
 **/
void Kalman_GetMean(Kalman_MPU6050MeanDataTypeDef* Kalman_MPU6050MeanDataStruct,
                    Kalman_AK8975MeanDataTypeDef* Kalman_AK8975MeanDataStruct)
{
    MPU6050_RawDataTypeDef MPU6050_RawDataStruct;
    MPU6050_FloatDataTypeDef MPU6050_FloatDataStruct;
    //计算1000次，算平均值
    MPU6050_AllRawDataRead(&MPU6050_RawDataStruct);
    MPU6050_RawData2FloatData(&MPU6050_RawDataStruct,&MPU6050_FloatDataStruct);
}

/**
 * @brief   初始化的时候调用，计算噪声方差
 * @param   Kalman_MPU6050MeanDataStruct: MPU6050数据平均值
 * @param   Kalman_AK8975MeanDataStruct: AK8975数据平均值
 * @param   Kalman_MPU6050VarDataStruct: MPU6050噪声方差
 * @param   Kalman_AK8975VarDataStruct: AK8975噪声方差
 **/
void Kalman_GetVar(Kalman_MPU6050MeanDataTypeDef* Kalman_MPU6050MeanDataStruct,
                   Kalman_AK8975MeanDataTypeDef* Kalman_AK8975MeanDataStruct,
                   Kalman_MPU6050VarDataTypeDef* Kalman_MPU6050VarDataStruct,
                   Kalman_AK8975VarDataTypeDef* Kalman_AK8975VarDataStruct)
{
    //计算1000次，算方差
}