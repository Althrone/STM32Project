#include "ak8975.h"

AK8975_PRMTypeDef AK8975_PRMStruct;//灵敏度调整值，详见磁力计的芯片手册
AK8975_CalParamTypeDef AK8975_CalParamStruct;//真值修正参数

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
    AK8975_RawDataStruct->AK8975_RawMagX=-(((int16_t)temp[3]<<8)|temp[2]);
    AK8975_RawDataStruct->AK8975_RawMagY=((int16_t)temp[1]<<8)|temp[0];
    AK8975_RawDataStruct->AK8975_RawMagZ=((int16_t)temp[5]<<8)|temp[4];
}

/**
 * @brief  将数据转换成真值
 * @param  AK8975_RawDataStruct: 原始数据结构体
 * @param  AK8975_FloatDataStruct: 真值结构体
 **/
void AK8975_RawData2FloatData(AK8975_RawDataTypeDef* AK8975_RawDataStruct,
                              AK8975_FloatDataTypeDef* AK8975_FloatDataStruct)
{
    AK8975_AllRawDataRead(AK8975_RawDataStruct);
    AK8975_FloatDataStruct->AK8975_FloatMagX=(float_t)AK8975_RawDataStruct->AK8975_RawMagX*0.3f;
    AK8975_FloatDataStruct->AK8975_FloatMagY=(float_t)AK8975_RawDataStruct->AK8975_RawMagY*0.3f;
    AK8975_FloatDataStruct->AK8975_FloatMagZ=(float_t)AK8975_RawDataStruct->AK8975_RawMagZ*0.3f;

    AK8975_FloatDataStruct->AK8975_FloatMagX*=(AK8975_PRMStruct.ASAX-128)*0.5f/128+1;
    AK8975_FloatDataStruct->AK8975_FloatMagY*=(AK8975_PRMStruct.ASAY-128)*0.5f/128+1;
    AK8975_FloatDataStruct->AK8975_FloatMagZ*=(AK8975_PRMStruct.ASAZ-128)*0.5f/128+1;
}

/**
 * @brief   原始数据变修正后数据
 * @param   AK8975_RawDataStruct: 原始数据结构体
 * @param   AK8975_CalDataStruct: 修正后数据
 **/
void AK8975_RawData2CalData(AK8975_RawDataTypeDef* AK8975_RawDataStruct,
                            AK8975_CalDataTypeDef* AK8975_CalDataStruct)
{
    AK8975_FloatDataTypeDef AK8975_FloatDataStruct;
    AK8975_RawData2FloatData(AK8975_RawDataStruct,&AK8975_FloatDataStruct);
    //先不写校正，直接上
    AK8975_CalDataStruct->AK8975_CalMagX=AK8975_FloatDataStruct.AK8975_FloatMagX;
    AK8975_CalDataStruct->AK8975_CalMagY=AK8975_FloatDataStruct.AK8975_FloatMagY;
    AK8975_CalDataStruct->AK8975_CalMagZ=AK8975_FloatDataStruct.AK8975_FloatMagZ;
}

/**
 * @brief   获取地磁矢量，写入AT24C02
 **/
void AK8975_GetGeomagneticVector(void)
{
    float_t B0x=0;//地磁矢量
    float_t B0z=0;
    float_t By=0;//计算地磁矢量的中间值
    AK8975_RawDataTypeDef AK8975_RawDataStruct;
    AK8975_CalDataTypeDef AK8975_CalDataStruct;
    //递归1000次，计算各轴平均值
    for (uint16_t i = 0; i < 1000; i++)
    {
        RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
        AK8975_RawData2CalData(&AK8975_RawDataStruct,&AK8975_CalDataStruct);
        B0x=Recursion_Mean(B0x,AK8975_CalDataStruct.AK8975_CalMagX,i+1);
        By=Recursion_Mean(By,AK8975_CalDataStruct.AK8975_CalMagY,i+1);
        B0z=Recursion_Mean(B0z,AK8975_CalDataStruct.AK8975_CalMagZ,i+1);
    }
    B0x=1/Fast_InvSqrt(B0x*B0x+By*By);
    //写入AT24C02
    uint64_t write_tmp;//at24c02页写入中间量
    write_tmp=(uint64_t)*(uint32_t*)&B0z<<32|(uint64_t)*(uint32_t*)&B0x;//大端小端写入问题，keil要反过来
    AT24C02_PageWrite(0x40,(uint8_t*)&write_tmp);
    RGBLED_StateSet(RGBLED_Green,RGBLED_1sMode);//矫正完成，显示绿色
}

void AK8975_IDRead(uint8_t* data)
{
    IIC_ReadByteFromSlave(AK8975_CAD1_LOW_CAD0_LOW,AK8975_WIA,data);
}