/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "rgbled.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 
void TIM6_Init(void);


#ifdef __cplusplus
}
#endif

#endif /*__TIM_H */