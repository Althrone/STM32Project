/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <math.h>

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



#ifdef __cplusplus
}
#endif

#endif /*__PID_H */