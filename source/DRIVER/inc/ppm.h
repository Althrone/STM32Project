/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PPM_H
#define __PPM_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  PPM Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/

void PPM_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__PPM_H */