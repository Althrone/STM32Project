// /* Define to prevent recursive inclusion -------------------------------------*/
// #ifndef __GPS_H
// #define __GPS_H

// #ifdef __cplusplus
//     extern "C" {
// #endif

// /* Includes ------------------------------------------------------------------*/
// #include "stm32f4xx.h"

// #include "math.h"

// #include "usart.h"

// #include "list.h"

// #include "rgbled.h"

// /* Exported types ------------------------------------------------------------*/

// typedef enum//不超过4个字符 没所谓
// {
//     GPS_SatGPS     ='GP',//美帝
//     GPS_SatBDS     ='BD',//北斗，天朝
//     GPS_SatGNSS    ='GN',
//     GPS_SatGLONASS ='GL',//格洛纳斯，毛子
//     GPS_SatGALILEO ='GA',//伽利略，欧盟
//     GPS_SatCC      ='CC',
//     GPS_SatCF      ='CF',
// }GPS_SatelliteID;//卫星标识

// typedef enum//不超过4个字符 没所谓
// {
//     GPS_ContZDA     ='ZDA',//UTC时间和日期
//     GPS_ContGGA     ='GGA',//时间，位置，定位数据
//     GPS_ContGLL     ='GLL',//经纬度，UTC时间，定位状态
//     GPS_ContVTG     ='VTG',//方位角，对地速度
//     GPS_ContGSA     ='GSA',//精度因子，有效卫星，位置和水平/竖直稀释精度
//     GPS_ContGSV     ='GSV',
//     GPS_ContRMC     ='RMC',
//     GPS_ContANT     ='ANT',
// }GPS_ContentID;//内容标识

// typedef enum
// {
//     GPS_FlagFloat,//获取的内容是纯数字，转浮点数
//     GPS_FlagTime,//获取的内容是纯数字，但是是时间，转时间结构体
//     GPS_Flag
// }GPS_FlagTypeDef;

// typedef enum
// {
//     GPS_StateStart,     //序列开始
//     GPS_StatePrepEnd,   //序列预结束（0x0D）可能是数据
//     GPS_StateEnd,       //序列结束
//     GPS_StateDecode,    //开始解码
//     GPS_StateFinish,    //解码完成
// }GPS_StateTypeDef;

// typedef enum
// {
//     GPS_QltyFactorUnlocated,
//     GPS_QltyFactorRGPS,
//     GPS_QltyFactorDGPS,
//     GPS_QltyFactorPPS,
// }GPS_QltyFactorTypeDef;

// typedef struct
// {
//     uint8_t GPS_Year;//Year month day hour minute second
//     uint8_t GPS_Month;
//     uint8_t GPS_Day;
//     uint8_t GPS_Hour;
//     uint8_t GPS_Minute;
//     uint8_t GPS_Second;
//     uint16_t GPS_Millisecond;
// }GPS_TimeTypeDef;

// typedef struct
// {
//     float_t GPS_Longitude;  //经度
//     float_t GPS_Latitude;   //纬度
//     float_t GPS_Altitude;   //海拔高度
//     float_t GPS_Elevation;  //高程

//     float_t GPS_Azimuth;    //方位角
//     float_t GPS_TrueBearing;//真北方位角
//     float_t GPS_CompassBearing;//磁北方位角
//     float_t GPS_MagneticDeclination;//磁偏角

//     float_t GPS_GroundSpeed;//对地速度，m/s
// }GPS_LocationTypeDef;//GPS定位信息

// typedef struct
// {
//     uint8_t GPS_QltyFactor;//质量因子
//     float_t GPS_HDOP;//水平精度因子
//     uint8_t GPS_SatNum;//卫星数量
// }GPS_AccuracyTypeDef;//GPS精度信息


// typedef struct
// {
//     GPS_TimeTypeDef GPS_TimeStruct;
//     GPS_LocationTypeDef GPS_LocationStruct;
//     GPS_AccuracyTypeDef GPS_AccuracyStruct;
// }GPS_DataTypeDef;//GPS所有信息


// /* Exported functions --------------------------------------------------------*/
// void GPS_Init(void);
// void GPS_Decode(void);
// bool GPS_CheckSum(void);

// void GPS_DecodeZDA(List_NodeTypeDef* NodePointer);
// void GPS_DecodeRMC(List_NodeTypeDef* NodePointer);
// void GPS_DecodeGGA(List_NodeTypeDef* NodePointer);

// void GPS_ASCII2Time(List_NodeTypeDef* NodePointer,GPS_TimeTypeDef* GPS_TimeStruct);
// void GPS_ASCII2Date(List_NodeTypeDef* NodePointer,GPS_TimeTypeDef* GPS_TimeStruct);
// float_t GPS_ASCII2Float(List_NodeTypeDef* NodePointer);
// float_t GPS_ASCII2Angel(List_NodeTypeDef* NodePointer);
// uint8_t GPS_ASCII2Hex(uint8_t input);
// uint8_t GPS_IsValidData(List_NodeTypeDef* NodePointer);

// #ifdef __cplusplus
// }
// #endif

// #endif /*__GPS_H */