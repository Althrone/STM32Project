#include "at24c02.h"

/**
 * @brief   单字节写入
 * @param   addr: 写入的地址
 * @param   data: 写入的数据
 **/
void AT24C02_ByteWrite(uint8_t addr,uint8_t data)
{
    IIC_WriteByteToSlave(AT24C02_A2_LOW_A1_LOW_A0_LOW,addr,data);
}

/**
 * @brief   整页写入，1K/2K芯片8字节为一页，4K/8K/16K芯片16字节为一页，页写入只支持从页头开始写，不然会出错
 * @param   addr: 写入的地址
 * @param   data: 写入的数据的指针
 **/
void AT24C02_PageWrite(uint8_t addr,uint8_t* data)
{
    IIC_WriteMultByteToSlave(AT24C02_A2_LOW_A1_LOW_A0_LOW,addr,8,data);
}

/**
 * @brief   读当前地址数据，仅限1字节
 * @param   buf: 写入的数据的指针
 **/
void AT24C02_CurrentAddressRead(uint8_t* buf)
{
    IIC_Start();
    IIC_SendByte(AT24C02_A2_LOW_A1_LOW_A0_LOW<<1|1); //进入接收模式
    IIC_WaitAck();
    *buf=IIC_ReadByte(0);
    IIC_Stop(); //产生一个停止条件
}

/**
 * @brief   随机读取
 **/
void AT24C02_RandomRead(uint8_t* buf)
{}

/**
 * @brief   顺序读取n个字节，最多8个字节，不允许跨页读取
 * @param   addr: 读取的首个寄存器的地址
 * @param   length: 顺序读取的长度
 * @param   data: 读取到的数据缓存的地方
 **/
void AT24C02_SequentialRead(uint8_t addr,uint8_t length,uint8_t* data)
{
    IIC_ReadMultByteFromSlave(AT24C02_A2_LOW_A1_LOW_A0_LOW,addr,length,data);
    // I2C_Master(I2C1,I2C_Direction_Receiver,
    //            AT24C02_A2_LOW_A1_LOW_A0_LOW,I2C_AcknowledgedAddress_7bit,
    //            addr,data,length);
}