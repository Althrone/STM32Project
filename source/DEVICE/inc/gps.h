/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/**
 * GPS_(UBLOX/中科微)_功能_功能描述
 **/

#define GPS_UBLOX_SAVE_SEG1 0xB56206090D000000
#define GPS_UBLOX_SAVE_SEG2 0x0000FFFF00000000 
#define GPS_UBLOX_SAVE_SEG3 0x00001731BF

#define GPS_UBLOX_OUT_PUT_RATE_1HZ_SEG1 0xB56206080600E803
#define GPS_UBLOX_OUT_PUT_RATE_1HZ_SEG2 0x010001000139
/* Exported types ------------------------------------------------------------*/

typedef enum
{
    GPS_SatGPS     ='GP',//美帝
    GPS_SatBDS     ='BD',//北斗，天朝
    GPS_SatGNSS    ='GN',
    GPS_SatGLONASS ='GL',//格洛纳斯，毛子
    GPS_SatGALILEO ='GA',//伽利略，欧盟
    GPS_SatCC      ='CC',
    GPS_SatCF      ='CF',
}GPS_SatelliteID;//卫星标识

typedef enum
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

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /*__GPS_H */