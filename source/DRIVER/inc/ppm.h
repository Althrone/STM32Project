/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PPM_H
#define __PPM_H

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct
{
    uint32_t PMM_1msCnt;            /*!< 定时整1ms记录
                                         参数是xms，不超过20ms */
    uint32_t PMM_LastTime;          /*!< 用于计算两个PMM脉冲之间的时间长度
                                         时间片长度不超过1ms */
    uint32_t PMM_NewTime;              /*!< 用于计算两个PMM脉冲之间的时间长度
                                         时间片长度不超过1ms */
    uint32_t PMM_TimeSlce;
    uint8_t PMM_EndFlag;            /*!< 一个PMM信号结束标志
                                         参数是0或者1 */
    uint16_t PPM_Databuf[10];
}PMM_DemodulateTypeDef;//PMM调制

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  PPM Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/

void PPM_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__PPM_H */