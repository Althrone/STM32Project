/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/ 

void Motor_Init(void);
void Motor_SetSpeed(void);



#ifdef __cplusplus
}
#endif

#endif /*__MOTOR_H */