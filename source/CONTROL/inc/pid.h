/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <math.h>

#include "ANO_DT.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

typedef struct
{
    float_t Error;          //e(k)
    float_t PrevError;      //e(k-1)
    float_t PrevPrevError;  //e(k-2)
    float_t Output;         //u(k)
    float_t PrevOutput;     //u(k-1)
}PID_InfoTypeDef;

typedef struct
{
    float_t P;
    float_t I;
    float_t D;
}PID_ParamTypeDef;


/* Exported functions --------------------------------------------------------*/ 
void PID_IncCtrl(PID_InfoTypeDef* PID_InfoStruct,
                 PID_ParamTypeDef* PID_ParamStruct,
                 ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct,
                 float_t target,float_t measure);
void PID_IncCtrlAngle(PID_InfoTypeDef* PID_InfoStruct,
                 PID_ParamTypeDef* PID_ParamStruct,
                 ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct,
                 float_t target,float_t measure);
void PID_ParamInit(void);


#ifdef __cplusplus
}
#endif

#endif /*__PID_H */