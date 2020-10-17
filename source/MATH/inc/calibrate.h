/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALIBRATE_H
#define __CALIBRATE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "arm_math.h"
#include "base.h"

#include "ppm.h"
#include "rgbled.h"

#include "mpu6050.h"
#include "ak8975.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    float_t rX;
    float_t rY;
    float_t rZ;
    float_t X0;
    float_t Y0;
    float_t Z0;
}CAL_EllipsoidParamTypeDef;


/* Exported functions --------------------------------------------------------*/ 
void CAL_Senser(void);
void CAL_Ellipsoid(float_t x,float_t y,float_t z,uint8_t i,
                   CAL_EllipsoidParamTypeDef* CAL_EllipsoidParamStruct);

#ifdef __cplusplus
}
#endif

#endif /*__CALIBRATE_H */