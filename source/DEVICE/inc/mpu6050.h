/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  MPU6050 Data structure definition  
  */

typedef struct
{
    int16_t MPU6050_RawAccelX;
    int16_t MPU6050_RawAccelY;
    int16_t MPU6050_RawAccelZ;
    int16_t MPU6050_RawTemp;
    int16_t MPU6050_RawGyroX;
    int16_t MPU6050_RawGyroY;
    int16_t MPU6050_RawGyroZ;
}MPU6050_RawDataTypeDef;

/* Exported functions --------------------------------------------------------*/ 

void MPU6050_Init(void);
void MPU6050_RawAccelRead(int16_t* );


#ifdef __cplusplus
}
#endif

#endif /*__MPU6050_H */