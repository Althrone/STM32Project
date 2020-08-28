/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/ 

/** 
  * @brief  MPU6050 Data structure definition  
  */

typedef struct
{
    uint16_t MPU6050_RawAccleX;
    uint16_t MPU6050_RawAccleY;
    uint16_t MPU6050_RawAccleZ;
    uint16_t MPU6050_RawTemp;
    uint16_t MPU6050_RawGyroX;
    uint16_t MPU6050_RawGyroY;
    uint16_t MPU6050_RawGyroZ;
}MPU6050_DataTypeDef;

#ifdef __cplusplus
}
#endif

#endif /*__MPU6050_H */