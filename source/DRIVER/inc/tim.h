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
  * @brief  TIM Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/

void TIM3_Init(void);
void TIM5_Init(void);
void TIM6_Init(void);
void TIM7_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__TIM_H */