/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MATRIX_H
#define __MATRIX_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "arm_math.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    float_t Quat0;
    float_t Quat1;
    float_t Quat2;
    float_t Quat3;
}Mat_RotQuatTypeDef;

typedef struct
{
    float_t Phi;
    float_t Theta;
    float_t Psi;
}Mat_RotEulerTypeDef;

typedef struct
{
    float_t Data1;
    float_t Data2;
    float_t Data3;
}Mat_RotDataTypeDef;

/* Exported functions --------------------------------------------------------*/ 
void Mat_RotEuler(Mat_RotEulerTypeDef* Mat_RotEulerStruct,
                  Mat_RotDataTypeDef* Mat_RotDataStruct);
void Mat_RotQuat(Mat_RotQuatTypeDef* Mat_RotQuatStruct,
                 Mat_RotDataTypeDef* Mat_RotDataStruct);
void Mat_TransRotQuat(Mat_RotQuatTypeDef* Mat_RotQuatStruct,
                      Mat_RotDataTypeDef* Mat_RotDataStruct);

#ifdef __cplusplus
}
#endif

#endif /*__MATRIX_H */