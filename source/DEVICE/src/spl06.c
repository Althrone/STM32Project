#include "spl06.h"

SPL06_PRMTypeDef SPL06_PRMStruct;

/**
 * @brief  初始化SPL06
 **/
void SPL06_Init(void)
{
    uint8_t temp[18];
    //重置所有寄存器
    IIC_WriteByteToSlave(SPL06_AD0_HIGH,SPL06_RESET,SPL06_RESET_SOFT_RST);
    SysTick_DelayMs(40);
    IIC_ReadMultByteFromSlave(SPL06_AD0_HIGH,SPL06_COEF,18,temp);//获取系数
    //系数预处理
    SPL06_PRMStruct.c0=((int16_t)temp[0]<<4)+(temp[1]>>4);
    if((SPL06_PRMStruct.c0&0x0800)==0x0800)//负数
        SPL06_PRMStruct.c0=SPL06_PRMStruct.c0|0xF000;
    SPL06_PRMStruct.c1=((int16_t)(temp[1]&0x0F)<<8)+temp[2];
    if((SPL06_PRMStruct.c1&0x0800)==0x0800)//负数
        SPL06_PRMStruct.c1=SPL06_PRMStruct.c1|0xF000;
    SPL06_PRMStruct.c00=((int32_t)temp[3]<<12)+((int32_t)temp[4]<<4)+(temp[5]>>4);
    if((SPL06_PRMStruct.c00&0x00080000)==0x00080000)
        SPL06_PRMStruct.c00=SPL06_PRMStruct.c00|0xFFF00000;
    SPL06_PRMStruct.c10=((int32_t)(temp[5]&0x0F)<<16)+((int32_t)temp[6]<<8)+temp[7];
    if((SPL06_PRMStruct.c10&0x00080000)==0x00080000)
        SPL06_PRMStruct.c10=SPL06_PRMStruct.c10|0xFFF00000;
    SPL06_PRMStruct.c01=((int16_t)temp[8]<<8)+temp[9];
    SPL06_PRMStruct.c11=((int16_t)temp[10]<<8)+temp[11];
    SPL06_PRMStruct.c20=((int16_t)temp[12]<<8)+temp[13];
    SPL06_PRMStruct.c21=((int16_t)temp[14]<<8)+temp[15];
    SPL06_PRMStruct.c30=((int16_t)temp[16]<<8)+temp[17];
    //设置为运动模式
    IIC_WriteByteToSlave(SPL06_AD0_HIGH,SPL06_PRS_CFG,SPL06_PRS_CFG_PM_RATE_4|
                                                      SPL06_PRS_CFG_PM_PRC_64);//0x26
    IIC_WriteByteToSlave(SPL06_AD0_HIGH,SPL06_TMP_CFG,SPL06_TMP_CFG_TMP_EX|
                                                      SPL06_TMP_CFG_TMP_RATE_4|
                                                      SPL06_TMP_CFG_TMP_PRC_1);//0xA0
    IIC_WriteByteToSlave(SPL06_AD0_HIGH,SPL06_CFG_REG,SPL06_CFG_REG_P_SHIFT);
    //对气压和温度进行连续测量
    IIC_WriteByteToSlave(SPL06_AD0_HIGH,SPL06_MEAS_CFG,SPL06_MEAS_CFG_MEAS_CTRL_BGPRSTMP);
}

/**
 * @brief  读取SPL06的气压，温度
 * @param  SPL06_RawDataStruct: 读取到的数据提取到这个结构体
 **/
void SPL06_AllRawDataRead(SPL06_RawDataTypeDef* SPL06_RawDataStruct)
{
    //暂存数据
    uint8_t temp[6];
    //抽取数据
    IIC_ReadMultByteFromSlave(SPL06_AD0_HIGH,SPL06_PSR_B2,6,temp);
    SPL06_RawDataStruct->SPL06_RawPres=((int32_t)temp[0]<<16)+((int32_t)temp[1]<<8)+temp[2];
    if((SPL06_RawDataStruct->SPL06_RawPres&0x00800000)==0x00800000)
        SPL06_RawDataStruct->SPL06_RawPres=SPL06_RawDataStruct->SPL06_RawPres|0xFF000000;
    SPL06_RawDataStruct->SPL06_RawTemp=((int32_t)temp[3]<<16)+((int32_t)temp[4]<<8)+temp[5];
    if((SPL06_RawDataStruct->SPL06_RawTemp&0x00800000)==0x00800000)
        SPL06_RawDataStruct->SPL06_RawTemp=SPL06_RawDataStruct->SPL06_RawTemp|0xFF000000;
}

/**
 * @brief  将数据转换成真值
 * @param  SPL06_RawDataStruct: 原始数据结构体
 * @param  SPL06_FloatDataStruct: 真值结构体
 **/
void SPL06_RawData2FloatData(SPL06_RawDataTypeDef* SPL06_RawDataStruct,
                             SPL06_FloatDataTypeDef* SPL06_FloatDataStruct)
{
    float_t Praw_sc,Traw_sc;
    uint32_t kP,kT;
    kP=SPL06_64Times;
    kT=SPL06_Single;
    Praw_sc=SPL06_RawDataStruct->SPL06_RawPres/kP;
    Traw_sc=SPL06_RawDataStruct->SPL06_RawTemp/kT;
    //
    SPL06_FloatDataStruct->SPL06_FloatPres=
                       SPL06_PRMStruct.c00+
                       Praw_sc*(SPL06_PRMStruct.c10+Praw_sc*(SPL06_PRMStruct.c20+Praw_sc*SPL06_PRMStruct.c30))+
                       Traw_sc*SPL06_PRMStruct.c10+
                       Traw_sc*Praw_sc*(SPL06_PRMStruct.c11+Praw_sc*SPL06_PRMStruct.c21);
    SPL06_FloatDataStruct->SPL06_FloatTemp=SPL06_PRMStruct.c0*0.5+SPL06_PRMStruct.c1*Traw_sc;
    //
}

void SPL06_IDRead(uint8_t* data)
{
    IIC_ReadByteFromSlave(SPL06_AD0_HIGH,SPL06_ID,data);
}