/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PPM_H
#define __PPM_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <string.h>

#include "stm32f4xx.h"

#include "tim.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  PPM Init structure definition  
  */

typedef struct
{
    uint8_t PPM_1msCnt;            /*!< 定时整1ms记录
                                         参数是xms，不超过20ms */
    uint16_t PPM_LastTime;          /*!< 用于计算两个PMM脉冲之间的时间长度
                                         时间片长度不超过1ms */
    uint16_t PPM_NewTime;              /*!< 用于计算两个PMM脉冲之间的时间长度
                                         时间片长度不超过1ms */
    uint16_t PPM_TimeSlce;
    
    uint16_t PPM_PWMCnt;            /*!< 目前是第几个PWM信号
                                         最多是10个 */
    uint8_t PPM_PWMMax;             /*!< 这个值是去遥控器和接收机之间支持通道的小的那个值
                                         最多是10个 */
}PPM_DemodTimeTypeDef;//PMM解调时间结构体

typedef struct
{
    int16_t PPM_RawCh1;
    int16_t PPM_RawCh2;
    int16_t PPM_RawCh3;
    int16_t PPM_RawCh4;
    int16_t PPM_RawCh5;
    int16_t PPM_RawCh6;
    int16_t PPM_RawCh7;
    int16_t PPM_RawCh8;
}PPM_RawDataTypeDef;


typedef struct
{
    uint8_t PPM_EndFlag;            /*!< PMM信号结束标志
                                         参数是0或者1 */
    uint8_t PPM_WorkFlag;           /*!< PMM信号解调标志
                                         参数是0或者1 */
}PPM_DemodFlagTypeDef;//PMM解调标志位结构体

/* Exported functions --------------------------------------------------------*/

void PPM_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__PPM_H */