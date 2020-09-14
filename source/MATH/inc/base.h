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



float_t FastInvSqrt(float_t input);


#ifdef __cplusplus
}
#endif

#endif /*__BASE_H */