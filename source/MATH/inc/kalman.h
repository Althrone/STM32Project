/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KALMAN_H
#define __KALMAN_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "mpu6050.h"
#include "ak8975.h"
#include "spl06.h"

#include "base.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

typedef struct
{
    float_t Kalman_MPU6050MeanAccelX;
    float_t Kalman_MPU6050MeanAccelY;
    float_t Kalman_MPU6050MeanAccelZ;
    float_t Kalman_MPU6050MeanGyroX;
    float_t Kalman_MPU6050MeanGyroY;
    float_t Kalman_MPU6050MeanGyroZ;
}Kalman_MPU6050MeanDataTypeDef;

typedef struct
{
    float_t Kalman_AK8975MeanMagX;
    float_t Kalman_AK8975MeanMagY;
    float_t Kalman_AK8975MeanMagZ;
}Kalman_AK8975MeanDataTypeDef;

typedef struct
{
    float_t Kalman_MPU6050VarAccelX;
    float_t Kalman_MPU6050VarAccelY;
    float_t Kalman_MPU6050VarAccelZ;
    float_t Kalman_MPU6050VarGyroX;
    float_t Kalman_MPU6050VarGyroY;
    float_t Kalman_MPU6050VarGyroZ;
}Kalman_MPU6050VarDataTypeDef;

typedef struct
{
    float_t Kalman_AK8975VarMagX;
    float_t Kalman_AK8975VarMagY;
    float_t Kalman_AK8975VarMagZ;
}Kalman_AK8975VarDataTypeDef;

/* Exported functions --------------------------------------------------------*/ 
void Kalman_GetMean(Kalman_MPU6050MeanDataTypeDef* Kalman_MPU6050MeanDataStruct,
                    Kalman_AK8975MeanDataTypeDef* Kalman_AK8975MeanDataStruct);

void Kalman_GetVar(Kalman_MPU6050VarDataTypeDef* Kalman_MPU6050VarDataStruct,
                   Kalman_AK8975VarDataTypeDef* Kalman_AK8975VarDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__KALMAN_H */