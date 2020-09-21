#include "mpu6050.h"

/**
 * @brief  初始化6050
 * @note  这个传感器也是sb，初始化之后MPU6050_PWR_MGMT_1
 * 的SLEEP位是1，然后这个状态下你修改其他寄存器是没鸟用的
 **/
void MPU6050_Init(void)
{
    //重置6050所有寄存器
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_PWR_MGMT_1,MPU6050_PWR_MGMT_1_DEVICE_RESET);
    SysTick_DelayMs(10);
    //配置时钟
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_PWR_MGMT_1,MPU6050_PWR_MGMT_1_CLKSEL_GYRO_X_PLL);//设置量程等寄存器要先唤醒了
    //设置量程
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_GYRO_CONFIG,MPU6050_GYRO_CONFIG_FS_SEL_2000);
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_ACCEL_CONFIG,MPU6050_ACCEL_CONFIG_AFS_SEL_4G);
    //设置低通滤波
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_CONFIG,MPU6050_CONFIG_DLPF_CFG_BW_20);//Gyroscope Output Rate=1kHz
    //设置采样率
    IIC_WriteByteToSlave(MPU6050_AD0_LOW,MPU6050_SMPLRT_DIV,MPU6050_SMPLRT_DIV_(0));//采样频率1kHz
}

/**
 * @brief   陀螺仪零漂校正
 **/
void MPU6050_GyroCal(MPU6050_CalParamTypeDef* MPU6050_CalParamStruct)
{
    //静止状态取值1000次，算平均然后塞到存储芯片里面，开机读取里面的数值，如果是0就进入校准状态
    //暂存数据
    uint8_t temp[6];
    for (uint16_t i = 0; i < 1000; i++)
    {
        IIC_ReadMultByteFromSlave(MPU6050_AD0_LOW,MPU6050_GYRO_XOUT_H,6,temp);
        //与前一数据对比，正负10之内都能接受
        MPU6050_CalParamStruct->MPU6050_OffsetGyroX+=(((int16_t)temp[0]<<8)+temp[1])/32767.0f;
        MPU6050_CalParamStruct->MPU6050_OffsetGyroY+=(((int16_t)temp[2]<<8)+temp[3])/32767.0f;
        MPU6050_CalParamStruct->MPU6050_OffsetGyroZ+=(((int16_t)temp[4]<<8)+temp[5])/32767.0f;

    }
    MPU6050_CalParamStruct->MPU6050_OffsetGyroX/=1000;
    MPU6050_CalParamStruct->MPU6050_OffsetGyroY/=1000;
    MPU6050_CalParamStruct->MPU6050_OffsetGyroZ/=1000;
    //浮点数写入存储器，一个占四字节，从0x00开始
    AT24C02_PageWrite(0x00,(uint8_t*)&(MPU6050_CalParamStruct->MPU6050_OffsetGyroX));
    SysTick_DelayMs(2);
    AT24C02_PageWrite(0x08,(uint8_t*)&(MPU6050_CalParamStruct->MPU6050_OffsetGyroY));
    SysTick_DelayMs(2);
    AT24C02_PageWrite(0x0F,(uint8_t*)&(MPU6050_CalParamStruct->MPU6050_OffsetGyroZ));
}

/**
 * @brief   加速度计六面矫正
 **/
void MPU6050_AccelCal()
{
    //
}

/**
 * @brief  读取6050的加速度，温度，角速度数据
 * @param  MPU6050_RawDataStruct: 读取到的数据提取到这个结构体
 **/
void MPU6050_AllRawDataRead(MPU6050_RawDataTypeDef* MPU6050_RawDataStruct)
{
    //暂存数据
    uint8_t temp[14];
    //抽取数据
    IIC_ReadMultByteFromSlave(MPU6050_AD0_LOW,MPU6050_ACCEL_XOUT_H,14,temp);
    //转化成有符号整型
    MPU6050_RawDataStruct->MPU6050_RawAccelX=((int16_t)temp[0]<<8)+temp[1];
    MPU6050_RawDataStruct->MPU6050_RawAccelY=((int16_t)temp[2]<<8)+temp[3];
    MPU6050_RawDataStruct->MPU6050_RawAccelZ=((int16_t)temp[4]<<8)+temp[5];
    MPU6050_RawDataStruct->MPU6050_RawTemp=((int16_t)temp[6]<<8)+temp[7];
    MPU6050_RawDataStruct->MPU6050_RawGyroX=((int16_t)temp[8]<<8)+temp[9];
    MPU6050_RawDataStruct->MPU6050_RawGyroY=((int16_t)temp[10]<<8)+temp[11];
    MPU6050_RawDataStruct->MPU6050_RawGyroZ=((int16_t)temp[12]<<8)+temp[13];
}

/**
 * @brief  将数据转换成真值
 * @param  MPU6050_RawDataStruct: 原始数据结构体
 * @param  MPU6050_FloatDataStruct: 真值结构体
 **/
void MPU6050_RawData2FloatData(MPU6050_RawDataTypeDef* MPU6050_RawDataStruct,
                               MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct)
{
    int16_t Accel_Full_Scale=4;
    int16_t Gyro_Full_Scale=2000;
    MPU6050_FloatDataStruct->MPU6050_FloatAccelX=Accel_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawAccelX/32767.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatAccelY=Accel_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawAccelY/32767.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatAccelZ=Accel_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawAccelZ/32767.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatTemp=36.53f+MPU6050_RawDataStruct->MPU6050_RawTemp/340.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatGyroX=Gyro_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawGyroX/32767.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatGyroY=Gyro_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawGyroY/32767.0f;
    MPU6050_FloatDataStruct->MPU6050_FloatGyroZ=Gyro_Full_Scale*MPU6050_RawDataStruct->MPU6050_RawGyroZ/32767.0f;
}

/**
 * @brief   将数据转换成校准过后的值，这个值会用来解算欧拉角，然后送卡尔曼滤波器
 * @param  MPU6050_RawDataStruct: 原始数据结构体
 * @param  MPU6050_CalDataStruct: 校准后数据结构体
 **/
void MPU6050_RawData2CalData(MPU6050_RawDataTypeDef* MPU6050_RawDataStruct,
                             MPU6050_CalDataTypeDef* MPU6050_CalDataStruct)
{
    //需要变成浮点数，然后滤波，然后添加校准值
}

void MPU6050_IDRead(uint8_t* data)
{
    IIC_ReadByteFromSlave(MPU6050_AD0_LOW,MPU6050_WHO_AM_I,data);
}