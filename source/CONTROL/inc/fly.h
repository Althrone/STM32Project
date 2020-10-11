/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLY_H
#define __FLY_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "pid.h"
#include "ANO_DT.h"
#include "attitude.h"

/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/ 
void FLY_DroneCtrl(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct,
                   ATT_AngleDataTypeDef* ATT_AngleDataStruct,
                   MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__FLY_H */