/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_MAHONY_H
#define __AHRS_MAHONY_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mpu6050.h"

#include "arm_math.h"
#include "base.h"

#include "attitude.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */




/* Exported functions --------------------------------------------------------*/
void AHRS_MahonyUpdate(MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
                       AK8975_CalDataTypeDef* AK8975_CalDataStruct,
                       ATT_AngleDataTypeDef* ATT_AngleDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__AHRS_MAHONY_H */