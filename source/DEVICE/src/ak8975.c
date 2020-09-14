#include "ak8975.h"

/**
 * @brief  磁罗盘初始化
 **/
void AK8975_Init(void)
{
    //重制磁罗盘所有寄存器
    //启动数据采集
    IIC_WriteByteToSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_CNTL,AK8975_CNTL_MODE_MEAS);
}

/**
 * @brief  读取磁力计的原始数据
 * @param  AK8975_RawDataStruct： 读取到的数据提取到这个结构体
 **/
void AK8975_AllRawDataRead(AK8975_RawDataTypeDef* AK8975_RawDataStruct)
{
    //暂存数据
    uint8_t temp[6];
    //抽取数据
    IIC_ReadMultByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_HXL,6,temp);
    //转化成有符号整型
    AK8975_RawDataStruct->AK8975_RawMagX=((int16_t)temp[1]<<8)+temp[0];
    AK8975_RawDataStruct->AK8975_RawMagX=((int16_t)temp[3]<<8)+temp[2];
    AK8975_RawDataStruct->AK8975_RawMagX=((int16_t)temp[5]<<8)+temp[4];
}

void AK8975_IDRead(uint8_t* data)
{
    IIC_ReadByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_WIA,data);
}