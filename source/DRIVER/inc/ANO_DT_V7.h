// /* Define to prevent recursive inclusion -------------------------------------*/
// #ifndef __ANO_DT_H
// #define __ANO_DT_H

// #ifdef __cplusplus
//     extern "C" {
// #endif

// /* Includes ------------------------------------------------------------------*/

// #include "stm32f4xx.h"

// #include "usart.h"

// #include "mpu6050.h"
// #include "ak8975.h"
// #include "spl06.h"
// #include "gps.h"

// // #include <stddef.h>
// #include <stdlib.h>
// #include <string.h>

// /* Exported types ------------------------------------------------------------*/

// typedef enum
// {
// /***********************飞控相关信息类*****************/
//     ANO_DT_DataCheck    =0x00,//数据检查
//     ANO_DT_Sensor1      =0x01,//加速度计，陀螺仪数据
//     ANO_DT_Senser2      =0x02,//磁力计，气压计，温度传感器数据
//     ANO_DT_AttAngle     =0x03,//欧拉角飞控姿态
//     ANO_DT_AttQuat      =0x04,//四元数飞控姿态
//     ANO_DT_Alt          =0x05,//高度数据
//     ANO_DT_FCMode       =0x06,//飞控运行模式
//     ANO_DT_Speed        =0x07,//飞行速度
//     ANO_DT_PosOfs       =0x08,//位置偏移
//     ANO_DT_WSEst        =0x09,//风速估计
//     ANO_DT_TgtAtt       =0x0A,//目标姿态
//     ANO_DT_TgtSpeed     =0x0B,//目标速度
//                      // =0x0C,//回航信息
//                      // =0x0D,//电压电流
//                      // =0x0E,//外接模块工作状态
//                      // =0x0F,//RGB 亮度信息输出
// /***********************飞控控制量输出类***************/
//     ANO_DT_PWM          =0x20,//PWM控制量
//                      // =0x21,//姿态控制量
// /***********************飞控接收信息类*****************/
//     ANO_DT_GPS          =0x30,//GPS数据
//     ANO_DT_PosSensor    =0x32,//位置传感器
//     ANO_DT_SpeedSensor  =0x33,//速度传感器
//     ANO_DT_Dist         =0x34,//测距数据
// /***********************飞控接收控制指令类*************/
//     ANO_DT_RCData       =0x40,//遥控器数据
//                      // =0x41,//实时控制帧
// /***********************光流信息类********************/
//                      // =0x51,//匿名光流数据
// /***********************GPS 航点读写帧****************/
// /***********************功能触发类********************/
//  // ANO_DT_FlexFormat   =0xF1~0xFA,//灵活格式帧ID
// }ANO_DT_FuncByteTypeDef;//功能字节,和V6相差很大

// typedef enum
// {
//     ANO_DT_GeneralCall  =0xFF,//广播，V7新增
//     ANO_DT_MstrComputer =0xAF,//上位机
//  // ANO_DT_OtherHDW     =0x00,//其他设备，V7弃用
//  // ANO_DT_4Axis        =0x01,//匿名开拓者四轴，V7弃用
//  // ANO_DT_6Axis        =0x02,//匿名挑战者六轴，V7弃用
//  // ANO_DT_8Axis        =0x03,//匿名八轴，V7弃用
//  // ANO_DT_Pilot        =0x04,//匿名领航者，V7弃用
//     ANO_DT_ToKon        =0x05,//匿名拓空者
//     ANO_DT_DDT          =0x10,//匿名数传
//  // ANO_DT_RC2          =0x11,//匿名遥控2.0，V7弃用
//     ANO_DT_OpticalFlow  =0x22,//匿名光流
//  // ANO_DT_OpenMV       =0x29,//匿名OpenMV，V7弃用
//     ANO_DT_UWB          =0x30,//匿名无线定位
//     ANO_DT_CampsisIMU   =0x60,//匿名凌霄IMU，V7新增
//     ANO_DT_Campsis      =0x61,//匿名凌霄飞控，V7新增
// }ANO_DT_DevByteTypeDef;//发送设备和目标设备

// typedef struct
// {
//     uint8_t ANO_DT_HardwareType;
//     uint16_t ANO_DT_HardwareVer;
//     uint16_t ANO_DT_SoftwareVer;
//     uint16_t ANO_DT_BootloaderVer;
// }ANO_DT_SendVerTypeDef;

// typedef struct//数据校验帧
// {
//     uint8_t ANO_DT_FrameID;//需要校验的帧的ID
//     uint8_t ANO_DT_SumCheck;//需要校验的帧的和校验值
//     uint8_t ANO_DT_AddCheck;//需要校验的帧的附加校验值
// }ANO_DT_DataCheckTypeDef;

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

// void ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,uint8_t* databuf);

// void ANO_DT_SendDataCheck(USART_TypeDef* USARTx,ANO_DT_DataCheckTypeDef* ANO_DT_DataCheckStruct);
// // void ANO_DT_SendUserData(USART_TypeDef* USARTx,uint8_t FrameID,
// //                          ANO_DT_SendDataCheckTypeDef* ANO_DT_SendDataCheckStruct);//数据长度小于40个字节

// void ANO_DT_SendVer(USART_TypeDef* USARTx,ANO_DT_SendVerTypeDef* ANO_DT_SendVerStruct);
// void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct);

// void ANO_DT_SendSenser(USART_TypeDef* USARTx,
//                        MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
//                        AK8975_CalDataTypeDef* AK8975_CalDataStruct,
//                        SPL06_FloatDataTypeDef* SPL06_FloatDataStruct,
//                        GPS_DataTypeDef* GPS_DataStruct);

// void ANO_DT_SendRCData(USART_TypeDef* USARTx,ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct);
// void ANO_DT_SendGPSData(USART_TypeDef* USARTx,ANO_DT_SendGPSDataTypeDef* ANO_DT_SendGPSDataStruct);
// void ANO_DT_SendPower(USART_TypeDef* USARTx,ANO_DT_SendPowerTypeDef* ANO_DT_SendPowerStruct);
// void ANO_DT_SendMoto(USART_TypeDef* USARTx,ANO_DT_SendMotoTypeDef* ANO_DT_SendMotoStruct);
// void ANO_DT_SendSenser2(USART_TypeDef* USARTx,ANO_DT_SendSenser2TypeDef* ANO_DT_SendSenser2Struct);

// #ifdef __cplusplus
// }
// #endif

// #endif /*__ANO_DT_H */