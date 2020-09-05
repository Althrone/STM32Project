/**
 * @file    ANO_DT.c
 * @author  Althrone
 * @version 
 * @date    
 * @brief   使用匿名飞控V6协议
 **/

#include "ANO_DT.h"

//计算数据长度

/**
 * @brief  串口补零函数
 **/
void ANO_DT_ZeroPadding(void)
{

}

void ANO_DT_SendData()
{
    
}


/**
 * @brief  发送版本信息
 * @param  HardwareType: 硬件种类
 *          This parameter can be one of the following values:
 *            @arg NULL:  
 * @param  HardwareVer: 硬件版本
 *          This parameter can be one of the following values:
 *            @arg NULL:  
 * @param  SoftwareVer: 软件版本
 *          This parameter can be one of the following values:
 *            @arg NULL:  
 * @param  BootloaderVer: 引导加载版本
 *          This parameter can be one of the following values:
 *            @arg NULL:  
 **/
void ANO_DT_SendVer(USART_TypeDef* USARTx,ANO_DT_SendVerTypeDef* ANO_DT_SendVerStruct)
{
    uint8_t lenth=0;//真正显示的数据的长度
    //计算长度
    if(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer!=0)
        lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType);
    else if(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer!=0)
        lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType);
    else if(ANO_DT_SendVerStruct->ANO_DT_HardwareVer!=0)
        lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer)+
              sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType);
    else if(ANO_DT_SendVerStruct->ANO_DT_HardwareType!=0)
        lenth=sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType);
    //只要有一个有效参数，就计算然后串口发送数据
    if(lenth!=0)
    {
        uint8_t tmp=0;
        lenth=lenth+6;
        //开辟内存区域
        uint8_t* const databuf=(uint8_t*)malloc(sizeof(uint8_t)*lenth);
        //往databuf填写发送的信息
        *databuf=0xAA;
        *(databuf+1)=ANO_ToKon;
        *(databuf+2)=ANO_Computer;
        *(databuf+3)=0x00;
        *(databuf+4)=lenth;
        //真实数据和补零
        if(lenth-6-tmp>0)
        {
            *(databuf+5)=ANO_DT_SendVerStruct->ANO_DT_HardwareType;
            tmp=tmp+sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareType);
        }
        if(lenth-6-tmp>0)
        {
            *(databuf+6)=BYTE0(ANO_DT_SendVerStruct->ANO_DT_HardwareVer);
            *(databuf+7)=BYTE1(ANO_DT_SendVerStruct->ANO_DT_HardwareVer);
            tmp=tmp+sizeof(ANO_DT_SendVerStruct->ANO_DT_HardwareVer);
        }
        if(lenth-6-tmp>0)
        {
            *(databuf+8)=BYTE0(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer);
            *(databuf+9)=BYTE1(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer);
            tmp=tmp+sizeof(ANO_DT_SendVerStruct->ANO_DT_SoftwareVer);
        }
        if(lenth-6-tmp>0)
        {
            *(databuf+10)=BYTE0(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer);
            *(databuf+11)=BYTE1(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer);
            tmp=tmp+sizeof(ANO_DT_SendVerStruct->ANO_DT_BootloaderVer);
        }
        //和校验
        uint8_t sum=0;//和校验
        for(uint8_t i=0;i<lenth;i++)
        {
            sum=sum+*(databuf+i);
        }
        *(databuf+lenth-1)=sum;
        //发送
        for(uint8_t i=0;i<lenth;i++)
        {
            if(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!=RESET)
            {
                USART_SendData(USARTx,*(databuf+i));
            }
        }
        //释放内存
        free(databuf);
    }
    //如果算出来完全空的就啥都不发，我不知道有没有那么无聊的人
}