/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANO_DT_H
#define __ANO_DT_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

#include "usart.h"

/* Exported types ------------------------------------------------------------*/

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

typedef enum
{
    ANO_Ver         =0x00,//版本信息
    ANO_Status      =0x01,
    ANO_Senser      =0x02,
    ANO_RCData      =0x03,
    ANO_GPSData     =0x04,
    ANO_Power       =0x05,
    ANO_Moto        =0x06,
    ANO_Senser2     =0x07,
    ANO_SenserLink  =0x08
}ANOFuncByte_TypeDef;//功能字节,不完整，待补充

typedef enum
{
    ANO_Computer    =0xAF,//上位机
    ANO_OtherHDW    =0x00,//其他设备
    ANO_4Axis       =0x01,//匿名开拓者四轴
    ANO_6Axis       =0x02,//匿名挑战者六轴
    ANO_8Axis       =0x03,//匿名八轴
    ANO_Pilot       =0x04,//匿名领航者
    ANO_ToKon       =0x05,//匿名拓空者
    ANO_DDT         =0x10,//匿名数传
    ANO_RC2         =0x11,//匿名遥控2.0
    ANO_OpticalFlow =0x22,//匿名光流
    ANO_OpenMV      =0x29,//匿名OpenMV
    ANO_UWB         =0x30 //匿名无线定位
}ANODev_TypeDef;//发送设备和目标设备
/* Exported functions --------------------------------------------------------*/ 





#ifdef __cplusplus
}
#endif

#endif /*__ANO_DT_H */