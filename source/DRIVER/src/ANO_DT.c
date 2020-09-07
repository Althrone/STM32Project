/**
 * @file    ANO_DT.c
 * @author  Althrone
 * @version 
 * @date    
 * @brief   使用匿名飞控V6协议
 **/

#define S_ADDR  ANO_OtherHDW
#define D_ADDR  ANO_Computer

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
 * @param  USARTx: 硬件种类
 * @param  ANO_DT_SendVerStruct: 硬件版本
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
    *(databuf)=0xAA;      //帧头固定
    *(++databuf)=S_ADDR;    //在定义里面改
    *(++databuf)=D_ADDR;    //在定义里面改
    *(++databuf)=0x00;      //功能字
    *(++databuf)=lenth;     //有效数据长度
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
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    }
    //释放内存
    free(databuf);
}
