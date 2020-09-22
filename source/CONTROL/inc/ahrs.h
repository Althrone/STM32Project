/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <math.h>

#include "mpu6050.h"
#include "ak8975.h"

#include "attitude.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

// typedef struct
// {
//     float_t AHRS_a11;
//     float_t AHRS_a12;
//     float_t AHRS_a13;
//     float_t AHRS_a14;
//     float_t AHRS_a21;
//     float_t AHRS_a22;
//     float_t AHRS_a23;
//     float_t AHRS_a24;
//     float_t AHRS_a31;
//     float_t AHRS_a32;
//     float_t AHRS_a33;
//     float_t AHRS_a34;
//     float_t AHRS_a41;
//     float_t AHRS_a42;
//     float_t AHRS_a43;
//     float_t AHRS_a44;
// }AHRS_MatrixATypeDef;




/* Exported functions --------------------------------------------------------*/ 



#ifdef __cplusplus
}
#endif

#endif /*__AHRS_H */