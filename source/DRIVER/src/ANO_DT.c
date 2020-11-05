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
 * @brief  发送传感器数据
 * @param  USARTx: 串口号
 * @param  ANO_DT_SendSenserStruct: 传感器数据结构体
 **/
void ANO_DT_SendSenser(USART_TypeDef* USARTx,ANO_DT_SendSenserTypeDef* ANO_DT_SendSenserStruct)
{
    uint8_t databuf[24];
    uint8_t sum=0;
    uint8_t lenth=sizeof(ANO_DT_SendSenserTypeDef);
    //填写其他帧
    databuf[0]=0xAA;            //帧头固定
    databuf[1]=S_ADDR;        //在定义里面改
    databuf[2]=D_ADDR;        //在定义里面改
    databuf[3]=ANO_DT_Senser; //功能字
    databuf[4]=lenth;         //有效数据长度
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