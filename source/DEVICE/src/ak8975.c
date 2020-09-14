#include "ak8975.h"

AK8975_PRMTypeDef AK8975_PRMStruct;

/**
 * @brief  磁罗盘初始化
 **/
void AK8975_Init(void)
{
    uint8_t temp[3];
    //获取磁罗盘校正值
    IIC_WriteByteToSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_CNTL,AK8975_CNTL_MODE_FUSEACCESS);
    IIC_ReadMultByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_ASAX,3,temp);
    AK8975_PRMStruct.ASAX=temp[0];
    AK8975_PRMStruct.ASAY=temp[1];
    AK8975_PRMStruct.ASAZ=temp[2];
    //获取完之后回到关闭模式
    IIC_WriteByteToSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_CNTL,AK8975_CNTL_MODE_POWERDOWN);

}

/**
 * @brief  读取磁力计的原始数据
 * @param  AK8975_RawDataStruct： 读取到的数据提取到这个结构体
 **/
void AK8975_AllRawDataRead(AK8975_RawDataTypeDef* AK8975_RawDataStruct)
{
    //暂存数据
    uint8_t temp[6];
    //这个sb传感器每一次获取完数据就关闭，每次都要开
    IIC_WriteByteToSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_CNTL,AK8975_CNTL_MODE_MEAS);
    //等待数据处理好
    uint8_t flag=0;
    while(flag!=AK8975_ST1_DRDY)
    {
        IIC_ReadByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_ST1,&flag);
    }
    //抽取数据
    IIC_ReadMultByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_HXL,6,temp);
    //转化成有符号整型
    AK8975_RawDataStruct->AK8975_RawMagX=((int16_t)temp[1]<<8)+temp[0];
    AK8975_RawDataStruct->AK8975_RawMagY=((int16_t)temp[3]<<8)+temp[2];
    AK8975_RawDataStruct->AK8975_RawMagZ=((int16_t)temp[5]<<8)+temp[4];
}

/**
 * @brief  将数据转换成真值
 * @param  AK8975_RawDataStruct: 原始数据结构体
 * @param  AK8975_FloatDataStruct: 真值结构体
 **/
void AK8975_RawData2FloatData(AK8975_RawDataTypeDef* AK8975_RawDataStruct,
                              AK8975_FloatDataTypeDef* AK8975_FloatDataStruct)
{
    AK8975_FloatDataStruct->AK8975_FloatMagX=(float_t)AK8975_RawDataStruct->AK8975_RawMagX*
                                             ((AK8975_PRMStruct.ASAX-128)*0.5/128+1);
    AK8975_FloatDataStruct->AK8975_FloatMagY=(float_t)AK8975_RawDataStruct->AK8975_RawMagY*
                                             ((AK8975_PRMStruct.ASAY-128)*0.5/128+1);
    AK8975_FloatDataStruct->AK8975_FloatMagZ=(float_t)AK8975_RawDataStruct->AK8975_RawMagZ*
                                             ((AK8975_PRMStruct.ASAZ-128)*0.5/128+1);
    
}

void AK8975_IDRead(uint8_t* data)
{
    IIC_ReadByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_WIA,data);
}