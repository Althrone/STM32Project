// /* Define to prevent recursive inclusion -------------------------------------*/
// #ifndef __ANO_DT_H
// #define __ANO_DT_H

// #ifdef __cplusplus
//     extern "C" {
// #endif

// /* Includes ------------------------------------------------------------------*/

// #include "stm32f4xx.h"

// #include "usart.h"

// #include "list.h"

// // // #include <stddef.h>
// // #include <stdlib.h>
// // #include <string.h>

// /* Exported types ------------------------------------------------------------*/

// typedef enum
// {
//     ANO_DT_Ver         =0x00,//版本信息
//     ANO_DT_Status      =0x01,
//     ANO_DT_Senser      =0x02,
//     ANO_DT_RCData      =0x03,
//     ANO_DT_GPSData     =0x04,
//     ANO_DT_Power       =0x05,
//     ANO_DT_Moto        =0x06,
//     ANO_DT_Senser2     =0x07,
//     ANO_DT_SenserLink  =0x08
// }ANO_DT_FuncByteTypeDef;//功能字节,不完整，待补充

// typedef enum
// {
//     ANO_DT_Computer    =0xAF,   //上位机
//     ANO_DT_OtherHDW    =0x00,   //其他设备
//     ANO_DT_4Axis       =0x01,   //匿名开拓者四轴
//     ANO_DT_6Axis       =0x02,   //匿名挑战者六轴
//     ANO_DT_8Axis       =0x03,   //匿名八轴
//     ANO_DT_Pilot       =0x04,   //匿名领航者
//     ANO_DT_ToKon       =0x05,   //匿名拓空者
//     ANO_DT_DDT         =0x10,   //匿名数传
//     ANO_DT_RC2         =0x11,   //匿名遥控2.0
//     ANO_DT_OpticalFlow =0x22,   //匿名光流
//     ANO_DT_OpenMV      =0x29,   //匿名OpenMV
//     ANO_DT_UWB         =0x30    //匿名无线定位
// }ANO_DT_DevByteTypeDef;//发送设备和目标设备

// typedef struct
// {
//     uint8_t ANO_DT_HardwareType;
//     uint16_t ANO_DT_HardwareVer;
//     uint16_t ANO_DT_SoftwareVer;
//     uint16_t ANO_DT_BootloaderVer;
// }ANO_DT_SendVerTypeDef;

// typedef struct
// {
//     int16_t ANO_DT_Roll;
//     int16_t ANO_DT_Pitch;
//     int16_t ANO_DT_Yaw;
//     int32_t ANO_DT_Altitude;    //海拔高度
//     uint8_t ANO_DT_FlyModel;    //飞行模式
//     uint8_t ANO_DT_Armed;       //安全锁状态
// }ANO_DT_SendStatusTypeDef;

// typedef struct
// {
//     int16_t ANO_DT_AccX;
//     int16_t ANO_DT_AccY;
//     int16_t ANO_DT_AccZ;
//     int16_t ANO_DT_GyroX;
//     int16_t ANO_DT_GyroY;
//     int16_t ANO_DT_GyroZ;
//     int16_t ANO_DT_MagX;
//     int16_t ANO_DT_MagY;
//     int16_t ANO_DT_MagZ;
// }ANO_DT_SendSenserTypeDef;

// typedef struct
// {
//     int16_t ANO_DT_RCThrottle;
//     int16_t ANO_DT_RCYaw;
//     int16_t ANO_DT_RCRoll;
//     int16_t ANO_DT_RCPitch;
//     int16_t ANO_DT_RCAUX1;
//     int16_t ANO_DT_RCAUX2;
//     int16_t ANO_DT_RCAUX3;
//     int16_t ANO_DT_RCAUX4;
//     int16_t ANO_DT_RCAUX5;
//     int16_t ANO_DT_RCAUX6;
// }ANO_DT_SendRCDataTypeDef;

// typedef struct
// {
//     uint8_t ANO_DT_PointStatus;
//     uint8_t ANO_DT_Satellite;
//     int32_t ANO_DT_Longitude;
//     int32_t ANO_DT_Latitude;
//     int16_t ANO_DT_ReturnAngle;
//     uint16_t ANO_DT_ReturnDistance;
// }ANO_DT_SendGPSDataTypeDef;

// typedef struct
// {
//     uint16_t ANO_DT_Voltage;
//     uint16_t ANO_DT_Current;
// }ANO_DT_SendPowerTypeDef;

// typedef struct
// {
//     uint16_t ANO_DT_Moto1;
//     uint16_t ANO_DT_Moto2;
//     uint16_t ANO_DT_Moto3;
//     uint16_t ANO_DT_Moto4;
//     uint16_t ANO_DT_Moto5;
//     uint16_t ANO_DT_Moto6;
//     uint16_t ANO_DT_Moto7;
//     uint16_t ANO_DT_Moto8;
// }ANO_DT_SendMotoTypeDef;

// typedef struct
// {
//     int32_t ANO_DT_ALT_BAR;
//     int32_t ANO_DT_ALT_ADD;
//     int16_t ANO_DT_SEN_TMP;
// }ANO_DT_SendSenser2TypeDef;


// /* Exported functions --------------------------------------------------------*/ 

// // uint8_t* ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,uint8_t* databuf);

// // void ANO_DT_SendVer(USART_TypeDef* USARTx,ANO_DT_SendVerTypeDef* ANO_DT_SendVerStruct);
// // void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct);
// // void ANO_DT_SendSenser(USART_TypeDef* USARTx,ANO_DT_SendSenserTypeDef* ANO_DT_SendSenserStruct);
// // void ANO_DT_SendRCData(USART_TypeDef* USARTx,ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct);
// // void ANO_DT_SendGPSData(USART_TypeDef* USARTx,ANO_DT_SendGPSDataTypeDef* ANO_DT_SendGPSDataStruct);
// // void ANO_DT_SendPower(USART_TypeDef* USARTx,ANO_DT_SendPowerTypeDef* ANO_DT_SendPowerStruct);
// // void ANO_DT_SendMoto(USART_TypeDef* USARTx,ANO_DT_SendMotoTypeDef* ANO_DT_SendMotoStruct);
// // void ANO_DT_SendSenser2(USART_TypeDef* USARTx,ANO_DT_SendSenser2TypeDef* ANO_DT_SendSenser2Struct);

// // void USART1_Send(const unsigned char *data,unsigned char len);

// void ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,
//                         List_NodeTypeDef* ANO_DT_List_HeadPointer);
// void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct);


// #ifdef __cplusplus
// }
// #endif

// #endif /*__ANO_DT_H */