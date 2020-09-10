#include "mpu6050.h"



/**
 * @brief  初始化6050
 **/
void MPU6050_Init(void)
{
    //重置6050所有寄存器
    IIC_WriteByteToSlave();
    //设置量程
    //设置采样率
    //使能6050
}

void MPU6050_RawAccelRead(int16_t* a)
{

}

void MPU6050_RawAllDataRead()
{

}