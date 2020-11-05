/**
 * @file    ANO_DT.c
 * @author  Althrone
 * @version 3.0
 * @date    
 * @brief   使用匿名飞控V6协议，2.0使用链表还是和GPS有冲突，这次换成数组
 **/

#include "ANO_DT.h"

#define S_ADDR  ANO_DT_ToKon
#define D_ADDR  ANO_DT_Computer

/**
 * @brief  结构体成员拆解
 * @param  memberlenth: 成员长度
 * @param  memberhead: 成员首地址
 * @param  databuf: 缓存区目前位置
 **/
void ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,uint8_t* databuf)
{
    memberhead+=memberlenth-1;
    for (uint8_t i = 0; i < memberlenth; i++)
    {
        *(++databuf)=*memberhead;
        memberhead--;
    }
}

/**
 * @brief  发送版本信息
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendVerStruct: 版本信息结构体
 **/
void ANO_DT_SendVer(USART_TypeDef* USARTx,ANO_DT_SendVerTypeDef* ANO_DT_SendVerStruct)
{
    uint8_t databuf[13];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer)+
                  sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Ver;      //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_HardwareType,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_HardwareVer,
                       databuf+5);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_SoftwareVer,
                       databuf+7);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer),
                       (uint8_t *)&ANO_DT_SendVerStruct->ANO_DT_BootloaderVer,
                       databuf+9);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[12]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送姿态等基本信息
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendStatusStruct: 姿态等信息的结构体
 **/
void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct)
{
    uint8_t databuf[18];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel)+
                  sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Status;   //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Roll,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Pitch,
                       databuf+6);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Yaw,
                       databuf+8);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Altitude,
                       databuf+12);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_FlyModel,
                       databuf+13);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed),
                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Armed,
                       databuf+14);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[17]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送传感器数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendSenserStruct: 传感器数据结构体
 **/
void ANO_DT_SendSenser(USART_TypeDef* USARTx,ANO_DT_SendSenserTypeDef* ANO_DT_SendSenserStruct)
{
    uint8_t databuf[24];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendSenserTypeDef);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Senser;   //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccX),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccX,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccY),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccY,
                       databuf+6);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_AccZ),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_AccZ,
                       databuf+8);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroX),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroX,
                       databuf+10);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroY),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroY,
                       databuf+12);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_GyroZ),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_GyroZ,
                       databuf+14);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagX),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagX,
                       databuf+16);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagY),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagY,
                       databuf+18);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenserStruct->ANO_DT_MagZ),
                       (uint8_t *)&ANO_DT_SendSenserStruct->ANO_DT_MagZ,
                       databuf+20);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[23]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送遥控数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendRCDataStruct: 遥控数据结构体
 **/
void ANO_DT_SendRCData(USART_TypeDef* USARTx,ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    uint8_t databuf[26];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendRCDataTypeDef);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_RCData;   //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCYaw),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCYaw,
                       databuf+6);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCRoll),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCRoll,
                       databuf+8);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCPitch),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCPitch,
                       databuf+10);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX1),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX1,
                       databuf+12);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX2),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX2,
                       databuf+14);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX3),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX3,
                       databuf+16);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX4),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX4,
                       databuf+18);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX5),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX5,
                       databuf+20);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendRCDataStruct->ANO_DT_RCAUX6),
                       (uint8_t *)&ANO_DT_SendRCDataStruct->ANO_DT_RCAUX6,
                       databuf+22);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[25]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送GPS数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendGPSDataStruct: GPS数据结构体
 **/
void ANO_DT_SendGPSData(USART_TypeDef* USARTx,ANO_DT_SendGPSDataTypeDef* ANO_DT_SendGPSDataStruct)
{
    uint8_t databuf[20];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Satellite)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Longitude)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Latitude)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle)+
                  sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_GPSData;  //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_PointStatus,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Satellite),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Satellite,
                       databuf+5);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Longitude),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Longitude,
                       databuf+6);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_Latitude),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_Latitude,
                       databuf+10);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_ReturnAngle,
                       databuf+14);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance),
                       (uint8_t *)&ANO_DT_SendGPSDataStruct->ANO_DT_ReturnDistance,
                       databuf+16);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[19]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送电池数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendPowerStruct: 电池数据结构体
 **/
void ANO_DT_SendPower(USART_TypeDef* USARTx,ANO_DT_SendPowerTypeDef* ANO_DT_SendPowerStruct)
{
    uint8_t databuf[10];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendPowerTypeDef);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Power;    //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendPowerStruct->ANO_DT_Voltage),
                       (uint8_t *)&ANO_DT_SendPowerStruct->ANO_DT_Voltage,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendPowerStruct->ANO_DT_Current),
                       (uint8_t *)&ANO_DT_SendPowerStruct->ANO_DT_Current,
                       databuf+6);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[9]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送电机PWM数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendMotoStruct: 电机PWM数据结构体
 **/
void ANO_DT_SendMoto(USART_TypeDef* USARTx,ANO_DT_SendMotoTypeDef* ANO_DT_SendMotoStruct)
{
    uint8_t databuf[22];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendMotoTypeDef);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Moto;       //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto1),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto1,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto2),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto2,
                       databuf+6);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto3),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto3,
                       databuf+8);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto4),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto4,
                       databuf+10);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto5),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto5,
                       databuf+12);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto6),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto6,
                       databuf+14);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto7),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto7,
                       databuf+16);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendMotoStruct->ANO_DT_Moto8),
                       (uint8_t *)&ANO_DT_SendMotoStruct->ANO_DT_Moto8,
                       databuf+18);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[21]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  发送2号传感器数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendSenser2Struct: 2号传感器数据结构体
 **/
void ANO_DT_SendSenser2(USART_TypeDef* USARTx,ANO_DT_SendSenser2TypeDef* ANO_DT_SendSenser2Struct)
{
    uint8_t databuf[16];
    uint8_t sum=0;
    //计算数据长度
    uint8_t lenth=sizeof(ANO_DT_SendSenser2Struct->ANO_DT_ALT_BAR)+
                  sizeof(ANO_DT_SendSenser2Struct->ANO_DT_ALT_ADD)+
                  sizeof(ANO_DT_SendSenser2Struct->ANO_DT_SEN_TMP);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;          //在定义里面改
    databuf[2]=D_ADDR;          //在定义里面改
    databuf[3]=ANO_DT_Senser2;  //功能字
    databuf[4]=lenth;           //有效数据长度
    //数据填充
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenser2Struct->ANO_DT_ALT_BAR),
                       (uint8_t *)&ANO_DT_SendSenser2Struct->ANO_DT_ALT_BAR,
                       databuf+4);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenser2Struct->ANO_DT_ALT_ADD),
                       (uint8_t *)&ANO_DT_SendSenser2Struct->ANO_DT_ALT_ADD,
                       databuf+8);
    ANO_DT_SplitMember(sizeof(ANO_DT_SendSenser2Struct->ANO_DT_SEN_TMP),
                       (uint8_t *)&ANO_DT_SendSenser2Struct->ANO_DT_SEN_TMP,
                       databuf+12);
    //和校验
    for (uint8_t i = 0; i < lenth+6-1; i++)
    {
        sum=sum+databuf[i];
    }
    databuf[15]=sum;
    //发送数据
    for (uint8_t i = 0; i < lenth+6; i++)
    {
        USART_SendData(USARTx,databuf[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}