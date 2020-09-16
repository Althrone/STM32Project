/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

typedef enum
{
    I2C_StateStart=0,
    I2C_StateStop=1
}I2C_StateTypeDef;


/* Exported functions --------------------------------------------------------*/ 



void I2C1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__I2C_H */