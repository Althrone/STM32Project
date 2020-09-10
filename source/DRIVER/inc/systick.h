/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  TIM Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/

void SysTick_DelayUs(uint32_t microsecond);

#ifdef __cplusplus
}
#endif

#endif /*__SYSTICK_H */