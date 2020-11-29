/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

// typedef enum
// {
//     I2C_AddrMode_7Bit=0,
//     I2C_AddrMode_10Bit,
// }I2CAddrMode_TypeDef;

typedef enum
{
    I2C_Stat_Busy=0,      //总线忙，有可能别的玩意正在传输
    I2C_Stat_Start,     //传输开始
    I2C_Stat_RptStart,  //重复起始位
    I2C_Stat_Working,   //正在传输，本机工作中
    I2C_Stat_Finish,    //传输完成
    I2C_Stat_Err,       //传输出错
}I2CStat_TypeDef;

typedef enum
{
    I2C_Err_TimeOut,        //通信超时错误，包括总线忙超时
    I2C_Err_Hang,           //挂死
    I2C_Err_InvalidParam,   //参数无效
}I2CErr_TypeDef;

typedef struct
{
    uint8_t*        I2C_BufAddr;                //数据缓存区地址
    uint8_t         I2C_BufLen;                 //数据缓存区长度
    uint8_t         I2C_DataNum;                //当前发送/接收第几位数据，从零开始
    uint16_t        I2C_DevAddr;                //设备地址，不用SlaAddr是因为单片机不一定是主设备
    uint16_t        I2C_AcknowledgedAddress;    //设备地址模式
    uint8_t         I2C_Direction;              //I2C通讯方向
    I2CStat_TypeDef I2CStat_Enum;               //I2C状态
}I2C_MessageTypeDef;


/* Exported functions --------------------------------------------------------*/ 
void I2C_HangSlove(GPIO_TypeDef* GPIOx,I2C_TypeDef* I2Cx);
void I2C1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__I2C_H */