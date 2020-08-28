/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/ 

/** 
  * @brief  I2C Init structure definition  
  */

void I2C1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__I2C_H */