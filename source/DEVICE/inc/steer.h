/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEER_H
#define __STEER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "tim.h"

#include "ANO_DT.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Steer Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 

void Steer_Init(void);
void Steer_Test(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct);


#ifdef __cplusplus
}
#endif

#endif /*__STEER_H */