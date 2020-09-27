/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "arm_math.h"

#include "mpu6050.h"
#include "ak8975.h"
#include "at24c02.h"

#include "attitude.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */
typedef struct
{
    arm_matrix_instance_f32 X;  //状态矩阵，四元数
    arm_matrix_instance_f32 A;  //状态转移矩阵
    arm_matrix_instance_f32 P;  //状态协方差矩阵
    arm_matrix_instance_f32 Q;  //过程噪声方差
    arm_matrix_instance_f32 C;
    arm_matrix_instance_f32 H;  //卡尔曼增益
    arm_matrix_instance_f32 R;  //测量噪声方差
    arm_matrix_instance_f32 Z;  //观测矩阵
}AHRS_EKFParamTypeDef;


/* Exported functions --------------------------------------------------------*/
void AHRS_InitX(arm_matrix_instance_f32* X);
void AHRS_GetA(MPU6050_FloatDataTypeDef* MPU6050_FloatDataStruct,
               arm_matrix_instance_f32* A);
void AHRS_GetC(arm_matrix_instance_f32* A,
               arm_matrix_instance_f32* C);
void AHRS_InitP(arm_matrix_instance_f32* P);
void AHRS_InitR(arm_matrix_instance_f32* R);
void AHRS_EKF(AHRS_EKFParamTypeDef* AHRS_EKFParamStruct);

#ifdef __cplusplus
}
#endif

#endif /*__AHRS_H */