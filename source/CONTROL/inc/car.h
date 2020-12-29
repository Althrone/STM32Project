/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAR_H
#define __CAR_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "ANO_DT.h"

#include "steer.h"

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/ 
void Car_DroneCtrl(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__CAR_H */