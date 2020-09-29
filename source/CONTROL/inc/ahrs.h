/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

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
void AHRS_EKF(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
              ATT_AngleDataTypeDef* ATT_AngleDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__AHRS_H */