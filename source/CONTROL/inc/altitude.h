/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALTITUDE_H
#define __ALTITUDE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "mpu6050.h"
#include "spl06.h"
#include "gps.h"

#include "user_filter.h"

#include "attitude.h"

#include "matrix.h"

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/ 
void ALT_3OrderCompParamInit(void);

void ALT_3OrderCompUpdate(MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
                          SPL06_FloatDataTypeDef* SPL06_FloatDataStruct,
                          ATT_QuatDataTypeDef* ATT_QuatDataStruct);




#ifdef __cplusplus
}
#endif

#endif /*__ALTITUDE_H */