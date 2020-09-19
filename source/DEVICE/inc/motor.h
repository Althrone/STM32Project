/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

#include "tim.h"

#include "ANO_DT.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Motor Init structure definition  
  */

typedef struct
{
    uint16_t PWM1;
    uint16_t PWM2;
    uint16_t PWM3;
    uint16_t PWM4;
}Motor_PWMTypeDef;


/* Exported functions --------------------------------------------------------*/ 

void Motor_Init(void);
void Motor_SetSpeed(void);
void Motor_PWM(Motor_PWMTypeDef* Motor_PWMStruct);
void Motor_RunTest(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct);


#ifdef __cplusplus
}
#endif

#endif /*__MOTOR_H */