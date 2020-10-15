/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CALIBRATE_H
#define __CALIBRATE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
#include "arm_math.h"

#include "ppm.h"
#include "rgbled.h"


/* Exported functions --------------------------------------------------------*/ 
void CAL_Senser(void);
void CAL_Ellipsoid(float_t x,float_t y,float_t z);

#ifdef __cplusplus
}
#endif

#endif /*__CALIBRATE_H */