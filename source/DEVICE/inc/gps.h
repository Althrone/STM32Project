/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "math.h"

/**
 * GPS_(UBLOX/中科微)_功能_功能描述
 **/

#define GPS_UBLOX_SAVE_SEG1 0xB56206090D000000
#define GPS_UBLOX_SAVE_SEG2 0x0000FFFF00000000 
#define GPS_UBLOX_SAVE_SEG3 0x00001731BF

#define GPS_UBLOX_OUT_PUT_RATE_1HZ_SEG1 0xB56206080600E803
#define GPS_UBLOX_OUT_PUT_RATE_1HZ_SEG2 0x010001000139
/* Exported types ------------------------------------------------------------*/

typedef enum//不超过4个字符 没所谓
{
    GPS_SatGPS     ='GP',//美帝
    GPS_SatBDS     ='BD',//北斗，天朝
    GPS_SatGNSS    ='GN',
    GPS_SatGLONASS ='GL',//格洛纳斯，毛子
    GPS_SatGALILEO ='GA',//伽利略，欧盟
    GPS_SatCC      ='CC',
    GPS_SatCF      ='CF',
}GPS_SatelliteID;//卫星标识

typedef enum//不超过4个字符 没所谓
{
    GPS_ContZDA     ='ZDA',//UTC时间和日期
    GPS_ContGGA     ='GGZ',//时间，位置，定位数据
    GPS_ContGLL     ='GLL',//经纬度，UTC时间，定位状态
    GPS_ContVTG     ='VTG',//方位角，对地速度
    GPS_ContGSA     ='GSA',//精度因子，有效卫星，位置和水平/竖直稀释精度
    GPS_ContGSV     ='GSV',
    GPS_ContRMC     ='RMC',
    GPS_ContANT     ='ANT',
}GPS_ContentID;//内容标识

typedef enum
{
    GPS_FlagFloat,//获取的内容是纯数字，转浮点数
    GPS_FlagTime,//获取的内容是纯数字，但是是时间，转时间结构体
    GPS_Flag
}GPS_FlagTypeDef;

typedef enum
{
    GPS_StateStart,     //序列开始
    GPS_StatePrepEnd,   //序列预结束（0x0D）可能是数据
    GPS_StateEnd,       //序列结束
    GPS_StateDecode,    //开始解码
    GPS_StateFinish,    //解码完成
}GPS_StateTypeDef;

typedef struct
{
    uint8_t GPS_Year;//Year month day hour minute second
    uint8_t GPS_Month;
    uint8_t GPS_Day;
    uint8_t GPS_Hour;
    uint8_t GPS_Minute;
    uint8_t GPS_Second;
    uint16_t GPS_Millisecond;
}GPS_TimeTypeDef;




/* Exported functions --------------------------------------------------------*/
void GPS_Test(int* a);

#ifdef __cplusplus
}
#endif

#endif /*__GPS_H */