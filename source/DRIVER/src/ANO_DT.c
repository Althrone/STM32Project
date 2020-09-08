/**
 * @file    ANO_DT.c
 * @author  Althrone
 * @version 
 * @date    
 * @brief   使用匿名飞控V6协议
 **/

#define S_ADDR  ANO_DT_OtherHDW
#define D_ADDR  ANO_DT_Computer

#include "ANO_DT.h"

/**
 * @brief  结构体成员拆解
 * @param  memberlenth: 成员长度
 * @param  memberhead: 成员首地址
 * @param  databuf: 缓存区目前位置
 **/
uint8_t* ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,uint8_t* databuf)
{
    memberhead+=memberlenth-1;
    for (uint8_t i = 0; i < memberlenth; i++)
    {
        *(++databuf)=*memberhead;
        memberhead--;
    }
    return databuf;
}

/**
 * @brief  发送版本信息
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendVerStruct: 版本信息结构体
 **/
void ANO_DT_SendVer(USART_TypeDef* USARTx,ANO_DT_SendVerTypeDef* ANO_DT_SendVerStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;            //帧头固定
    *(++databuf)=S_ADDR;        //在定义里面改
    *(++databuf)=D_ADDR;        //在定义里面改
    *(++databuf)=ANO_DT_Ver;    //功能字
    *(++databuf)=lenth;         //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_HardwareType,
                       databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_HardwareVer,
                       databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_SoftwareVer,
                       databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_BootloaderVer,
                       databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送姿态等基本信息
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendStatusStruct: 姿态等信息的结构体
 **/
void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;            //帧头固定
    *(++databuf)=S_ADDR;        //在定义里面改
    *(++databuf)=D_ADDR;        //在定义里面改
    *(++databuf)=ANO_DT_Status; //功能字
    *(++databuf)=lenth;         //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Roll,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Pitch,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Yaw,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Altitude,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_FlyModel,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed),
                          (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Armed,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送传感器数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendSenserStruct: 传感器数据结构体
 **/
void ANO_DT_SendSenser(USART_TypeDef* USARTx,ANO_DT_SendSenserTypeDef* ANO_DT_SendSenserStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendSenserTypeDef);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;            //帧头固定
    *(++databuf)=S_ADDR;        //在定义里面改
    *(++databuf)=D_ADDR;        //在定义里面改
    *(++databuf)=ANO_DT_Senser; //功能字
    *(++databuf)=lenth;         //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccX),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccX,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccY),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccY,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccZ),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccZ,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroX),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroX,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroY),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroY,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroZ),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroZ,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagX),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagX,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagY),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagY,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagZ),
                          (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagZ,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送遥控数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendRCDataStruct: 遥控数据结构体
 **/
void ANO_DT_SendRCData(USART_TypeDef* USARTx,ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendRCDataTypeDef);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;            //帧头固定
    *(++databuf)=S_ADDR;        //在定义里面改
    *(++databuf)=D_ADDR;        //在定义里面改
    *(++databuf)=ANO_DT_RCData; //功能字
    *(++databuf)=lenth;         //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCYaw),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCYaw,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCRoll),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCRoll,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCPitch),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCPitch,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX1),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX1,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX2),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX2,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX3),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX3,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX4),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX4,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX5),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX5,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX6),
                          (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX6,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送GPS数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendGPSDataStruct: GPS数据结构体
 **/
void ANO_DT_SendRCData(USART_TypeDef* USARTx,ANO_DT_SendGPSDataTypeDef* ANO_DT_SendGPSDataStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Satellite)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Longitude)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Latitude)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;                //帧头固定
    *(++databuf)=S_ADDR;            //在定义里面改
    *(++databuf)=D_ADDR;            //在定义里面改
    *(++databuf)=ANO_DT_GPSData;    //功能字
    *(++databuf)=lenth;             //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Satellite),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Satellite,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Longitude),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Longitude,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Latitude),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Latitude,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance),
                          (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送电池数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendPowerStruct: 电池数据结构体
 **/
void ANO_DT_SendPower(USART_TypeDef* USARTx,ANO_DT_SendPowerTypeDef* ANO_DT_SendPowerStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendPowerTypeDef);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;                //帧头固定
    *(++databuf)=S_ADDR;            //在定义里面改
    *(++databuf)=D_ADDR;            //在定义里面改
    *(++databuf)=ANO_DT_Power;    //功能字
    *(++databuf)=lenth;             //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendPowerStruct->ANO_DT_Voltage),
                          (uint8_t *)&ANO_DT_SendPowerStruct->ANO_DT_Voltage,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendPowerStruct->ANO_DT_Current),
                          (uint8_t *)&ANO_DT_SendPowerStruct->ANO_DT_Current,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}

/**
 * @brief  发送电机PWM数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendMotoStruct: 电机PWM数据结构体
 **/
void ANO_DT_SendMoto(USART_TypeDef* USARTx,ANO_DT_SendMotoTypeDef* ANO_DT_SendMotoStruct)
{
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendMotoTypeDef);
    //分派内存空间
    uint8_t* databuf=(uint8_t*)malloc(lenth+6);
    //填写其他帧
    *(databuf)=0xAA;                //帧头固定
    *(++databuf)=S_ADDR;            //在定义里面改
    *(++databuf)=D_ADDR;            //在定义里面改
    *(++databuf)=ANO_DT_Moto;       //功能字
    *(++databuf)=lenth;             //有效数据长度
    //数据填充
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto1),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto1,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto2),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto2,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto3),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto3,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto4),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto4,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto5),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto5,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto6),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto6,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto7),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto7,
                          databuf);
    databuf=ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto8),
                          (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto8,
                          databuf);
    //指针回滚
    databuf=databuf-(lenth+6-2);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+*(databuf+i);
    }
    *(databuf+lenth+6-1)=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,*(databuf+i));
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}