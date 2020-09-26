/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BASE_H
#define __BASE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include <math.h>

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 



float_t Fast_InvSqrt(float_t input);
float_t Recursion_Mean(float_t mean,float_t x,uint16_t n);



#ifdef __cplusplus
}
#endif

#endif /*__BASE_H */