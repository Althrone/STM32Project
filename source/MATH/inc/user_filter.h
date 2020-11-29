/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILTER_H
#define __FILTER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include <math.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    float_t Kp;
    float_t Ki;
}Filter_CompParamTypeDef;   //互补滤波器参数

typedef struct
{
    float_t Output; //高阶输出
    float_t Input;  //低阶输入
    float_t Error;  //误差输入
    float_t ErrorInt;//误差积分项
}Filter_CompInfoTypeDef;;   //互补滤波器输入输出参数




/* Exported functions --------------------------------------------------------*/ 
void Filter_Comp(Filter_CompParamTypeDef* Filter_CompParamStruct,
                 Filter_CompInfoTypeDef* Filter_CompInfoStruct);
float_t Filter_Avg(float_t* bufAddr,uint16_t length,float_t val);

#ifdef __cplusplus
}
#endif

#endif /*__FILTER_H */