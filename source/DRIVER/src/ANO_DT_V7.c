// /**
//  *******************************************************************************
//  * @file    source\DRIVER\src\ANO_DT_V7.c
//  * @file    source\DRIVER\inc\ANO_DT_V7.h
//  * @brief   匿名V7上位机
//  * Date         Author          Notes
//  * 14/12/2020   Althrone        替换原有的V6协议，兼容V6函数
//  *******************************************************************************
//  **/

// #include "ANO_DT_V7.h"

// /**
//  * @brief   将两字节，四字节的数据裂开
//  * @param   Endianness: 字节顺序，暂时不考虑
//  *              This parameter can be one of the following values:
//  *                  @arg Little_Endian: 小端模式
//  *                  @arg Big_Endian: 大端模式
//  * @param   databuf: 缓存区目前位置
//  * @param   memberhead: 成员首地址
//  * @param   memberlenth: 成员长度
//  **/
// void ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,uint8_t* databuf)
// {
//     memberhead+=memberlenth-1;
//     for (uint8_t i = 0; i < memberlenth; i++)
//     {
//         *(++databuf)=*memberhead;
//         memberhead--;
//     }
// }

// /**
//  * @brief   所有需要发送传感器数据
//  * @param   USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
//  *          UART peripheral.
//  * @param   MPU6050_CalDataStruct: 6050校正数据，可以为NULL
//  * @param   AK8975_CalDataStruct: 磁力计数据，可以为NULL
//  * @param   SPL06_FloatDataStruct: 气压计数据，可以为NULL
//  * @param   GPS_DataStruct: GPS数据，可以为NULL
//  **/
// void ANO_DT_SendSenser(USART_TypeDef* USARTx,
//                        MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
//                        AK8975_CalDataTypeDef* AK8975_CalDataStruct,
//                        SPL06_FloatDataTypeDef* SPL06_FloatDataStruct,
//                        GPS_DataTypeDef* GPS_DataStruct)
// {
//     //为NULL就不发送
//     if(MPU6050_CalDataStruct!=NULL)
//     {}
//     if((AK8975_CalDataStruct!=NULL)||(SPL06_FloatDataStruct!=NULL))
//     {}
//     if(GPS_DataStruct!=NULL)
//     {
//         //
//     }
// }

// /**
//  * @param   USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
//  *          UART peripheral.
//  * @param   MPU6050_CalDataStruct: 6050校正数据，可以为NULL
//  * @param   Shock_Sta: 震动状态，暂时为NULL
//  **/
// void ANO_DT_SendSenser1(USART_TypeDef* USARTx,
//                         MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
//                         uint8_t* Shock_Sta)
// {
//     //加速度计满量程+-4g，浮点数放大1000倍
//     //陀螺仪不放大
    
//     uint8_t databuf[19];
//     uint8_t tmp=0;//当前数组元素位置
//     databuf[tmp++]=0xAA;                //帧头固定
//     databuf[tmp++]=ANO_DT_GeneralCall;  //广播模式
//     databuf[tmp++]=ANO_DT_Sensor1;      //
//     databuf[tmp++]=13;                  //有效数据位长度
//     ANO_DT_SplitMember(sizeof(MPU6050_CalDataStruct->MPU6050_CalAccelX),
//                        &(MPU6050_CalDataStruct->MPU6050_CalAccelX),
//                        &tmp);
//     for (uint8_t i = 0; i < 19; i++)
//     {
//         USART_SendData(USARTx,databuf[i]);
//         while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
//     }
// }