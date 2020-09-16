/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ATTITUDE_H
#define __ATTITUDE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "mpu6050.h"
#include "ak8975.h"
#include "spl06.h"

#include "base.h"
#include <math.h>

#include "ANO_DT.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 
void ATT_RawData(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                 AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
                 ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct);

#ifdef __cplusplus
}
#endif

#endif /*__ATTITUDE_H */