/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POSITION_H
#define __POSITION_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "gps.h"
#include "mpu6050.h"

#include "attitude.h"

#include "user_filter.h"
#include "matrix.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    float_t X;
    float_t Y;
}POS_DistTypeDef;



/* Exported functions --------------------------------------------------------*/

void POS_LongAndLat2Dist(GPS_DataTypeDef* GPS_DataStruct,
                         POS_DistTypeDef* POS_DistStruct);

#ifdef __cplusplus
}
#endif

#endif /*__POSITION_H */