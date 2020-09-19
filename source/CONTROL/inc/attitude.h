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
typedef struct
{
    float_t ATT_Quat0;
    float_t ATT_Quat1;
    float_t ATT_Quat2;
    float_t ATT_Quat3;
}ATT_QuatDataTypeDef;

typedef struct
{
    float_t ATT_AnglePhi;
    float_t ATT_AngleTheta;
    float_t ATT_AnglePsi;
}ATT_AngleDataTypeDef;



/** 
  * @brief  I2C Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 
void ATT_RawData(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
                 AK8975_FloatDataTypeDef* AK8975_FloatDataStruct,
                 ATT_AngleDataTypeDef* ATT_AngleDataStruct);
void ATT_Angle2Quat(ATT_AngleDataTypeDef* ATT_AngleDataStruct,
                    ATT_QuatDataTypeDef* ATT_QuatDataStruct);
void ATT_Quat2Angle(ATT_QuatDataTypeDef* ATT_QuatDataStruct,
                    ATT_AngleDataTypeDef* ATT_AngleDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__ATTITUDE_H */